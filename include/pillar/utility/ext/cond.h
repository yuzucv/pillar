#pragma once
#include <algorithm>
#include <list>
#include <locale>
#include <string>
#include <vector>

namespace yuzu
{
namespace ext
{
/**
 * @brief verify that str consists of letters only
 *
 * @param str
 * @return true
 * @return false
 */
static inline bool isalpha(const std::string& str)
{
    return std::all_of(str.begin(), str.end(), [](char c) { return std::isalpha(c, std::locale()); });
}

/**
 * @brief verify that str consists of number only
 *
 * @param str
 * @return true
 * @return false
 */
static inline bool isnum(const std::string& str)
{
    return std::all_of(str.begin(), str.end(), [](char c) { return std::isalnum(c, std::locale()); });
}

template <typename Con, typename Pred>
Con filter(const Con& container, Pred predicate)
{
    Con result;
    std::copy_if(container.begin(), container.end(), std::back_inserter(result), predicate);
    return result;
}

template <typename T>
bool contains(const std::vector<T>& seq, const T& value)
{
    return std::find(seq.begin(), seq.end(), value) != seq.end();
}

template <typename T>
bool contains(const std::list<T>& seq, const T& value)
{
    return std::find(seq.begin(), seq.end(), value) != seq.end();
}

template <typename Map, typename K = typename Map::key_type>
bool contains(const Map& seq, const K& value)
{
    return seq.find(value) != seq.end();
}

template <typename Map, typename K = typename Map::key_type>
std::vector<K> key_set(Map mm)
{
    std::vector<K> keys;
    for (auto it = mm.begin(); it != mm.end(); ++it)
    {
        keys.push_back(it->first);
    }
    return keys;
}

template <typename Map, typename V = typename Map::mapped_type>
std::vector<V> value_set(Map mm)
{
    std::vector<V> values;
    for (auto it = mm.begin(); it != mm.end(); ++it)
    {
        values.push_back(it->second);
    }
    return values;
}
} // namespace ext
} // namespace yuzu