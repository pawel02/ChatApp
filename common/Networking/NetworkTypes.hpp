#pragma once
#include <string>
#include <memory>
#include <vector>
#include "Serialization.hpp"
#include "HeaderHandlers.hpp"

namespace Networking
{
namespace DataTypes
{
    typedef uint64_t sizeType;
    /*
    Structure used for reading and writing to the socket
    T needs to inherit from serializable
    */
    template<typename T>
    struct CommunicateData
    {
        // stores the data on the heap as it could be big
        std::shared_ptr<T> _data;
        sizeType _size;

        /*
        Constructor will copy the data stored under the pointer to make
        sure that it is the one which owns it
        */
        CommunicateData(std::shared_ptr<T> data, sizeType size) noexcept
            :_data{ data }, _size{ size }
        {}

        CommunicateData(CommunicateData&& other) noexcept
            :_data{ std::move(other._data) },
            _size{ other._size }
        {}

        CommunicateData(const CommunicateData& other) noexcept
            :_data{ other._data },
            _size{ other._size }
        {};

        // These should not be used at least for now
        CommunicateData& operator=(const CommunicateData& other) = delete;
        CommunicateData& operator=(CommunicateData&& other) = delete;
    };

    struct basicData : public Serialization::ISerializable
    {
        std::vector<uint8_t> _data;

        basicData(std::vector<uint8_t> data) noexcept
            :_data{ data }
        {}

        basicData(const basicData& other) noexcept
            :_data{ other._data }
        {}

        basicData(basicData&& other) noexcept
            :_data{ std::move(other._data) }
        {}

        const uint8_t* Serialize() const override
        {
            return _data.data();
        }
    };

    struct stringSerialize : public Serialization::ISerializable
    {
        std::string _data;

        stringSerialize(const std::string& data) noexcept
            :_data{ data }
        {}

        stringSerialize(const stringSerialize& other) noexcept
            :_data{ other._data }
        {}

        stringSerialize(stringSerialize&& other) noexcept
            :_data{ std::move(other._data) }
        {}

        const uint8_t* Serialize() const override
        {
            return (const uint8_t*)(_data.c_str());
        }
    };
}
}
