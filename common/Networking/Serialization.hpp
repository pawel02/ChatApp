#pragma once
#include <cstdint>

namespace Networking
{
namespace Serialization
{
    struct ISerializable
    {
        virtual const uint8_t* Serialize() const = 0;
    };
}
}