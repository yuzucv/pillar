#pragma once

#include <array>
#include <iostream>
#include <random>

namespace yuzu
{
namespace test
{
template <typename... T>
void print(T&&... args)
{
    if constexpr (sizeof...(args) > 0)
    {
        ((std::cout << args << " "), ...);
    }
    std::cout << "\n";
}

template <int N>
std::array<float, N> createTensor()
{
    static std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist;
    std::array<float, N> tensor;
    for (int i = 0; i < N; i++)
    {
        tensor[i] = dist(gen);
    }
    return tensor;
};
} // namespace test
} // namespace yuzu