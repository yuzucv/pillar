#pragma once
#include <chrono>
#include <iostream>
#include <string_view>
#include <type_traits>

namespace yuzu
{
struct Timer
{
    std::chrono::time_point<std::chrono::steady_clock> start, end;
    std::chrono::duration<float> duration;
    std::string_view mPrefix = "Timer elapsed ";
    Timer() { start = std::chrono::steady_clock::now(); }
    Timer(std::string_view prefix) : mPrefix(prefix) { start = std::chrono::steady_clock::now(); }
    ~Timer()
    {
        end = std::chrono::steady_clock::now();
        duration = end - start;
        float ms = duration.count() * 1000.0f;
        std::cout << mPrefix << ms << "ms" << std::endl;
    }

    float Elapsed()
    {
        end = std::chrono::steady_clock::now();
        duration = end - start;
        float ms = duration.count() * 1000.0f;
        return ms;
    }
};

template <typename T, typename... Args>
static inline auto timeit(T&& func, Args&&... args)
{
    static_assert(std::is_invocable_v<T, Args...>, "Function is not invocable");

    auto timer = Timer();
    if constexpr (std::is_same_v<decltype(func(std::forward<Args>(args)...)), void>)
    {
        func(std::forward<Args>(args)...);
    }
    else
    {
        auto ret = func(std::forward<Args>(args)...);
        return ret;
    }
}

template <typename T, typename U, typename... Args>
auto timeit(T* thisPtr, U (T::*func)(Args...), Args&&... args)
{
    static_assert(std::is_invocable_v<U (T::*)(Args...), T*, Args...>, "Function is not invocable");

    auto timer = Timer();
    if constexpr (std::is_same_v<decltype((thisPtr->*func)(std::forward<Args>(args)...)), void>)
    {
        (thisPtr->*func)(std::forward<Args>(args)...);
    }
    else
    {
        auto ret = (thisPtr->*func)(std::forward<Args>(args)...);
        return ret;
    }
}

template <typename T, typename U, typename... Args>
auto timeit(T* thisPtr, U (T::*func)(Args...) const, Args&&... args)
{
    static_assert(std::is_invocable_v<U (T::*)(Args...), T*, Args...>, "Function is not invocable");

    auto timer = Timer();
    if constexpr (std::is_same_v<decltype((thisPtr->*func)(std::forward<Args>(args)...)), void>)
    {
        (thisPtr->*func)(std::forward<Args>(args)...);
    }
    else
    {
        auto ret = (thisPtr->*func)(std::forward<Args>(args)...);
        return ret;
    }
}
} // namespace yuzu