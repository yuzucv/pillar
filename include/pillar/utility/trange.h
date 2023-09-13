#pragma once

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <vector>

namespace yuzu
{

#pragma region internal range impl
namespace internal
{
template <typename T>
class RangeImpl
{
    class Iterator;

public:
    RangeImpl(T begin, T end, int step) : mBegin(begin), mEnd(end), mStep(step)
    {
        // May cause errors in some cases
        // if (step == 0)
        //     throw std::logic_error("step can not be zero.");
        // if ((step > 0) && (begin > end))
        //     throw std::logic_error("end must greater than begin.");
        // if ((step < 0) && (begin < end))
        //     throw std::logic_error("start must greater than end.");

        mCount = std::ceil(float(end - begin) / step);
    }

    Iterator begin() { return Iterator{0, *this}; }
    Iterator end() { return Iterator{mCount, *this}; }

    T operator[](int idx) const { return mBegin + mStep * idx; }

    size_t size() const { return mCount; }

    std::vector<T> vectorize() const
    {
        std::vector<T> v(this->size());
        std::generate(v.begin(), v.end(), [n = 0, this]() mutable { return (*this)[n++]; });
        return v;
    }

private:
    T mBegin;
    T mEnd;
    int mStep;
    int mCount;

    class Iterator
    {
    public:
        Iterator(int position, RangeImpl& rangeObj) : mPosition(position), mRangeObj(rangeObj)
        {
            mCurrentValue = rangeObj.mBegin + position * rangeObj.mStep;
        }

        T operator*() { return mCurrentValue; }

        const Iterator* operator++() ///< 正向遍历
        {
            mCurrentValue += mRangeObj.mStep;
            mPosition++;
            return this;
        }
        const Iterator* operator--() ///< 反向遍历
        {
            mCurrentValue -= mRangeObj.mStep;
            mPosition--;
            return this;
        }

        bool operator==(const Iterator& other) { return mPosition == other.mPosition; }
        bool operator!=(const Iterator& other) { return mPosition != other.mPosition; }

    private:
        T mCurrentValue;
        int mPosition;
        RangeImpl& mRangeObj;
    };
};
#pragma endregion
} // namespace internal

template <typename T>
auto trange(T start, T end, int step)
{
    return internal::RangeImpl<T>(start, end, step);
};
template <typename T>
auto trange(T end)
{
    return internal::RangeImpl<T>(T(), end, 1);
};
template <typename T>
auto trange(T start, T end)
{
    return internal::RangeImpl<T>(start, end, 1);
}
} // namespace yuzu
