#pragma once
#include <algorithm>
#include <numeric>
#include <sstream>
#include <string>
#include <strstream>
#include <vector>

namespace yuzu
{
namespace ext
{
/**
 * @brief whether a string is in a string list
 *
 * @param target a string
 * @param str_list a string list
 * @return true if target is in str_list
 */
static bool equals(const std::string& target, std::initializer_list<std::string> str_list)
{
    return std::find(str_list.begin(), str_list.end(), target) != str_list.end();
}

/**
 * @brief split a string into a string list
 *
 * @param s a string
 * @param delim a delimiter
 * @return a string list
 */
static std::vector<std::string> split(const std::string& s, const char delim)
{
    std::vector<std::string> sv;
    std::istringstream iss(s);
    std::string temp;

    while (std::getline(iss, temp, delim))
    {
        sv.emplace_back(std::move(temp));
    }
    return sv;
}

/**
 * @brief merge a string list into a string
 *
 * @param str_vector a string list
 * @param delim a delimiter
 * @return a string
 */
template <class T>
static std::string join(std::vector<T> str_vector, std::string delim)
{
    std::string str = std::accumulate(std::begin(str_vector), std::end(str_vector), std::string(),
                                      [&delim](const std::string& ss, const T& s)
                                      { return ss.empty() ? std::to_string(s) : ss + delim + std::to_string(s); });
    return str;
}
/**
 * @brief trim a string from left
 *
 * @param str a string
 * @param trim_ch a character to be trimmed
 * @return a string
 */
static std::string ltrim(std::string str, char trim_ch)
{
    auto it = std::find_if(str.begin(), str.end(), [&trim_ch](char ch) { return ch != trim_ch; });
    str.erase(str.begin(), it);
    return str;
}

/**
 * @brief trim a string from right
 *
 * @param str a string
 * @param trim_ch a character to be trimmed
 * @return a string
 */
static std::string rtrim(std::string str, char trim_ch)
{
    auto it = std::find_if(str.rbegin(), str.rend(), [&trim_ch](char ch) { return ch != trim_ch; });
    str.erase(it.base(), str.end());
    return str;
}

/**
 * @brief trim a string from both sides
 *
 * @param str a string
 * @param trim_ch a character to be trimmed
 * @return a string
 */
static std::string trim(std::string str, char trim_ch) { return ltrim(rtrim(str, trim_ch), trim_ch); }

/**
 * @brief replace a string in a string
 *
 * @param str a string
 * @param from a string to be replaced
 * @param to a string to replace from
 * @return a string
 */
static std::string find_and_replace(std::string str, std::string from, std::string to)
{
    std::size_t start_pos = 0;

    while ((start_pos = str.find(from, start_pos)) != std::string::npos)
    {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }

    return str;
}

/**
 * @brief convert a string to lower case
 *
 * @param str a string
 * @return std::string
 */
static inline std::string to_lower(std::string str)
{
    std::transform(std::begin(str), std::end(str), std::begin(str),
                   [](const std::string::value_type& x) { return std::tolower(x, std::locale()); });
    return str;
}

/**
 * @brief convert a string to upper case
 *
 * @param str a string
 * @return std::string
 */
static inline std::string to_upper(std::string str)
{
    std::transform(std::begin(str), std::end(str), std::begin(str),
                   [](const std::string::value_type& x) { return std::toupper(x, std::locale()); });
    return str;
}

/**
 * @brief to check if a string starts with a certain string.
 *
 * @param str raw string
 * @param prefix specified string which starts with.
 * @return true
 * @return false
 */
static inline bool starts_with(const std::string& str, const std::string& prefix)
{
    return str.size() >= prefix.size() && str.compare(0, prefix.size(), prefix) == 0;
}
} // namespace ext
} // namespace yuzu