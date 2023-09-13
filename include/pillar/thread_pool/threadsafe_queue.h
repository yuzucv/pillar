#pragma once
#include <algorithm>
#include <condition_variable>
#include <cstddef>
#include <memory>
#include <mutex>
#include <queue>

namespace yuzu
{
template <class T>
class ThreadsafeQueue
{
public:
    ThreadsafeQueue() = default;

    void push(T newValue)
    {
        std::shared_ptr<T> data(std::make_shared<T>(std::move(newValue)));
        std::lock_guard<std::mutex> lk(mMut);
        mData.push(data);
        mDataCond.notify_one();
    }

    void waitAndPop(T& value)
    {
        std::unique_lock<std::mutex> lk(mMut);
        mDataCond.wait(lk, [this] { return !mData.empty(); });
        value = std::move(*mData.front());
        mData.pop();
    }

    std::shared_ptr<T> waitAndPop()
    {
        std::unique_lock<std::mutex> lk(mMut);
        mDataCond.wait(lk, [this] { return !mData.empty(); });
        std::shared_ptr<T> res = mData.front();
        mData.pop();
        return res;
    }

    bool tryPop(T& value)
    {
        std::lock_guard<std::mutex> lk(mMut);
        if (mData.empty())
            return false;
        value = std::move(*mData.front());
        mData.pop();
        return true;
    }

    std::shared_ptr<T> tryPop()
    {
        std::lock_guard<std::mutex> lk(mMut);
        if (mData.empty())
            return std::shared_ptr<T>();
        std::shared_ptr<T> res = mData.front();
        mData.pop();
        return res;
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lk(mMut);
        return mData.empty();
    }

    size_t size() const
    {
        std::lock_guard<std::mutex> lk(mMut);
        return mData.size();
    }

private:
    std::queue<std::shared_ptr<T>> mData;
    mutable std::mutex mMut;
    std::condition_variable mDataCond;
};
} // namespace yuzu

namespace yuzu
{
template <class T = int>
class FineGrainedThreadsafeQueue
{
private:
    struct Node
    {
        std::shared_ptr<T> data;
        std::unique_ptr<Node> next;
    };

    std::mutex mHeadMut;
    std::mutex mTailMut;
    std::condition_variable mDataCond;
    std::unique_ptr<Node> mHead;
    Node* mTail;

public:
    FineGrainedThreadsafeQueue() : mHead(new Node), mTail(mHead.get()) {}
    FineGrainedThreadsafeQueue(const FineGrainedThreadsafeQueue&) = delete;
    FineGrainedThreadsafeQueue& operator=(const FineGrainedThreadsafeQueue&) = delete;

    std::shared_ptr<T> tryPop();
    bool tryPop(T& value);
    std::shared_ptr<T> waitAndPop();
    void waitAndPop(T& value);
    void push(T newValue);
    bool empty();
    size_t size();

private:
    Node* getTail()
    {
        std::lock_guard<std::mutex> tailLock(mTailMut);
        return mTail;
    }

    std::unique_ptr<Node> popHead()
    {
        std::unique_ptr<Node> oldHead = std::move(mHead);
        mHead = std::move(oldHead->next);
        --mCount;
        return oldHead;
    }

    std::unique_lock<std::mutex> waitForData()
    {
        std::unique_lock<std::mutex> headLock(mHeadMut);
        mDataCond.wait(headLock, [&] { return mHead.get() != getTail(); });
        return std::move(headLock);
    }

    std::unique_ptr<Node> waitPopHead()
    {
        std::unique_lock<std::mutex> headLock(waitForData());
        return popHead();
    }

    std::unique_ptr<Node> waitPopHead(T& value)
    {
        std::unique_lock<std::mutex> headLock(waitForData());
        value = std::move(*mHead->data);
        return popHead();
    }

    std::unique_ptr<Node> tryPopHead()
    {
        std::lock_guard<std::mutex> headLock(mHeadMut);
        if (mHead.get() == getTail())
        {
            return std::unique_ptr<Node>();
        }

        return popHead();
    }

    std::unique_ptr<Node> tryPopHead(T& value)
    {
        std::lock_guard<std::mutex> headLock(mHeadMut);
        if (mHead.get() == getTail())
        {
            return std::unique_ptr<Node>();
        }

        value = std::move(*mHead->data);
        return popHead();
    }

private:
    size_t mCount = 0;
};

template <class T>
void FineGrainedThreadsafeQueue<T>::push(T newValue)
{
    std::shared_ptr<T> newData(std::make_shared<T>(std::move(newValue)));
    std::unique_ptr<Node> p(new Node);

    {
        std::lock_guard<std::mutex> tailLock(mTailMut);
        mTail->data = newData;
        Node* const newTail = p.get();
        mTail->next = std::move(p);
        mTail = newTail;
        ++mCount;
    }

    mDataCond.notify_one();
}

template <class T>
std::shared_ptr<T> FineGrainedThreadsafeQueue<T>::waitAndPop()
{
    std::unique_ptr<Node> const oldHead = waitPopHead();
    return oldHead->data;
}

template <class T>
void FineGrainedThreadsafeQueue<T>::waitAndPop(T& value)
{
    std::unique_ptr<Node> const oldHead = waitPopHead(value);
}

template <class T>
std::shared_ptr<T> FineGrainedThreadsafeQueue<T>::tryPop()
{
    std::unique_ptr<Node> oldHead = tryPopHead();
    return oldHead ? oldHead->data : std::shared_ptr<T>();
}

template <class T>
bool FineGrainedThreadsafeQueue<T>::tryPop(T& value)
{
    std::unique_ptr<Node> const oldHead = tryPopHead(value);
    return oldHead != nullptr;
}

template <class T>
bool FineGrainedThreadsafeQueue<T>::empty()
{
    std::lock_guard<std::mutex> headLock(mHeadMut);
    return mHead.get() == getTail();
}
template <class T>
size_t FineGrainedThreadsafeQueue<T>::size()
{
    std::lock_guard<std::mutex> headLock(mHeadMut);
    std::lock_guard<std::mutex> tailLock(mTailMut);
    return mCount;
}
} // namespace yuzu
