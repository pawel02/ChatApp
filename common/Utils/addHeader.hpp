#pragma once
#include <memory>
#include <iostream>

namespace common
{
    static const int headerSize = 10;

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
        static std::shared_ptr<char[]> addHeader(char* data, unsigned long long size, const MessageType& messageType) noexcept
        {
            // 10 is the size of the header read the spec for more details
            std::shared_ptr<char[]> _data{ new char[size + headerSize] };

            // by default this will only be the default message
            _data[0] = 0x00;
            _data[1] = static_cast<int>(messageType);

            // Add the message size
            std::memcpy(_data.get() + 2, &size, 8);

            // Add the message
            std::memcpy(_data.get() + 10, data, size);
            std::cout << +_data.get() << "\n";
            return _data;
        }

    };

}

