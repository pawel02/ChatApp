#pragma once
#include <memory>
#include <iostream>

namespace common
{
    static constexpr int headerSize = 2 + sizeof(uint64_t);

    // All of the different message types
    enum class MessageType : int
    {
        plainText = 0, JSON, XML
    };

    struct Utils
    {
        /*
            Used for adding the headers to the data
        */
        static uint8_t* addHeader(uint8_t* data, uint64_t size, const MessageType& messageType) noexcept
        {
            // 10 is the size of the header read the spec for more details
            uint8_t* _data =  new uint8_t[size + headerSize];

            // by default this will only be the default message
            _data[0] = 0x00;
            _data[1] = static_cast<uint8_t>(messageType);

            // Add the message size
            std::memcpy(_data + 2, &size, sizeof(uint64_t));

            // Add the message
            std::memcpy(_data + headerSize, data, size);
            return _data;
        }

    };

}

