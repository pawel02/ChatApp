#pragma once
#include <string>
#include <memory>

namespace common
{
    /*
    Represents the data that is sent over the wire
    at the moment we only have 1 type ClientData 
    in the future I can have different structs that will get serialized into
    */
    struct ClientData
    {
        std::string message;

        const char* serialize() const noexcept
        {
            return message.c_str();
        }

        void deserialize(const char* data) noexcept
        {
            // This should simply be text for now
            message = std::string{ data };
        }
    };

    /*
    Structure used for reading and writing to the socket
    */
    struct CommunicateData
    {
        // stores the data on the heap as it could be big
        std::unique_ptr<char[]> _data;
        unsigned long long _size;

        /*
        Constructor will copy the data stored under the pointer to make
        sure that it is the one which owns it
        */
        CommunicateData(char* data, unsigned long long size) noexcept
            :_data{ new char[size] }, _size{ size }
        {
            std::memcpy(_data.get(), data, size);
        }

        /*
        In case you are using smart pointers already you can avoid the copy
        */
        CommunicateData(std::unique_ptr<char[]>&& data, unsigned long long size) noexcept
            :_data{std::move(data)}, _size{size}
        {}

        CommunicateData(CommunicateData&& other) noexcept
            :_data{std::move(other._data)},
            _size{other._size}
        {}

        CommunicateData(const CommunicateData& other) noexcept
            :_data{new char[other._size]},
            _size{other._size}
        {
            std::memcpy(_data.get(), other._data.get(), other._size);
        };

        // These should not be used at least for now
        CommunicateData& operator=(const CommunicateData& other) = delete;
        CommunicateData& operator=(CommunicateData&& other) = delete;
    };
}
