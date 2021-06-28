#pragma once
#include <cstdint>

namespace Utils
{
    struct ISerializable
    {
        virtual const uint8_t* Serialize() const = 0;
    };
}