#pragma once
#include <algorithm>
#include <cmath>
#include <list>
#include <numeric>
#include <type_traits>
#include <vector>

namespace yuzu
{
namespace ext
{
/**
 * @brief function softmax
 *
 * @tparam T is number type
 * @param input a iterable container
 */
template <typename T = float>
void softmax(T& input)
{
    float rowmax = *std::max_element(input.begin(), input.end());
    std::vector<float> y(input.size());
    float sum = 0.0f;
    for (size_t i = 0; i != input.size(); ++i)
    {
        sum += y[i] = std::exp(float(input[i]) - rowmax);
    }
    for (size_t i = 0; i != input.size(); ++i)
    {
        input[i] = y[i] / sum;
    }
}

/**
 * @brief the maximum subscript of a iterable container
 *
 * @tparam ForwardIterator a forward iterator
 * @param first a iterator to the first element
 * @param last a iterator to the last element
 * @return the index of the maximum element, if more than one element has the same maximum value,
 * return the first subscript.
 */
template <class ForwardIterator>
inline int argmax(ForwardIterator first, ForwardIterator last)
{
    return std::distance(first, std::max_element(first, last));
}

/**
 * @brief the minimum subscript of a iterable container
 *
 * @tparam ForwardIterator a forward iterator
 * @param first a iterator to the first element
 * @param last a iterator to the last element
 * @return the index of the minimum element
 */
template <class ForwardIterator>
inline int argmin(ForwardIterator first, ForwardIterator last)
{
    return std::distance(first, std::min_element(first, last));
}

/**
 * @brief It then returns the clamped value which is guaranteed to be within the range `[min, max]`.
 *
 * @param x the value you want to clamp
 * @param min the minimum value
 * @param max the maximum value
 * @return const T&
 */
template <typename T>
inline const T& clamp(const T& x, const T& min, const T& max)
{
    return std::max(std::min(x, max), min);
}

template <typename T>
inline T sum(std::vector<T> lst)
{
    static_assert(std::is_arithmetic_v<T>, "T must be arithmetic type");
    return std::accumulate(lst.begin(), lst.end(), T());
}

template <typename T>
inline T sum(std::list<T> lst)
{
    static_assert(std::is_arithmetic_v<T>, "T must be arithmetic type");
    return std::accumulate(lst.begin(), lst.end(), T());
}

static float to_radians(float degrees) { return degrees * (M_PI / 180); }
static float to_degrees(float radians) { return radians * (180 / M_PI); }

/**
 * @brief Calculate the norm of a vector.
 *
 * @tparam T float point
 * @param v tensor
 * @return T norm result
 */
template <class T>
T norm(const std::vector<T>& v)
{
    T sum = 0.0;
    for (T x : v)
    {
        sum += x * x;
    }
    return std::sqrt(sum);
}

/**
 * @brief Calculate the dot product of vectors.
 *
 * @tparam T float point
 * @param v1 tensor
 * @param v2 tensor
 * @return T dot product result
 */
template <class T>
T dotProduct(const std::vector<T>& v1, const std::vector<T>& v2)
{
    T result = 0.0;
    for (size_t i = 0; i < v1.size(); ++i)
    {
        result += v1[i] * v2[i];
    }
    return result;
}
} // namespace ext
} // namespace yuzu