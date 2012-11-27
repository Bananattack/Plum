#pragma once

#include <cstdint>

namespace plum
{
    // Take SDL's typedefs, but make them our own.
    // That way if for some reason, SDL is removed, it's an easy replace here.
    typedef std::int8_t int8_t;
    typedef std::int16_t int16_t;
    typedef std::int32_t int32_t;
    typedef std::uint8_t uint8_t;
    typedef std::uint16_t uint16_t;
    typedef std::uint32_t uint32_t;
    typedef std::nullptr_t nullptr_t;
}