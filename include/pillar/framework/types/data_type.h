#pragma once

#include <cstdint>

namespace yuzu
{
enum class DataType : int32_t
{
    //! 32-bit floating point format.
    kFLOAT = 0,

    //! IEEE 16-bit floating-point format.
    kHALF = 1,

    //! 8-bit integer representing a quantized floating-point value.
    kINT8 = 2,

    //! Signed 32-bit integer format.
    kINT32 = 3,

    //! 8-bit boolean. 0 = false, 1 = true, other values undefined.
    kBOOL = 4,

    kINT64 = 5,

    kFLOAT64 = 6,

    kNone,
};
} // namespace yuzu
