#pragma once
#include <queue>
#include <condition_variable>
#include <mutex>

namespace common
{
/*
Simple threadsafe queue that which can be used to await messages
*/
template<typename T>
class tsQueue
{
public:
    tsQueue() noexcept
    {}

    /*Adds data T to the queue*/
    void push(T&& data)
    {
        std::unique_lock<std::mutex> lk{ _mutex };
        _queue.push(data);

        lk.unlock();
        // notify only 1 thread that can send the data
        _ready = true;
        _cv.notify_one();
    }

    /*
    Blocks the current thread until a new message has been added
    */
    T pop()
    {
        std::unique_lock<std::mutex> lk{ _mutex };

        // only wait if there is nothing in the queue
        if (_queue.size() == 0)
        {
            _cv.wait(lk, [this] { return _ready; });
        }

        _ready = false;
        T lastItem{ std::move(_queue.back()) };
        _queue.pop();
        return lastItem;
    }

private:
    std::queue<T> _queue;
    std::condition_variable _cv;
    std::mutex _mutex;

    bool _ready = false;
};
}
