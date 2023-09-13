#pragma once
#include <type_traits>

namespace std
{
template <class T>
struct remove_cvref
{
    using type = typename remove_cv<typename remove_reference<T>::type>::type;
};

template <class T>
using remove_cvref_t = typename remove_cvref<T>::type;
} // namespace std