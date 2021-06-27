#include <iostream>
#include <thread>
#include <vector>
#include <boost/asio.hpp>
#include "Core/Server.hpp"

int main()
{
    boost::asio::io_context context;
    const unsigned short port = 8080;

    // Start the server
    Server server{ &context, port };
    server.start();

    std::vector<std::thread> threadPool;
    const int threadAmount = 10;
    for (int i = 0; i < threadAmount; i++)
    {
        threadPool.emplace_back([&]() {
            context.run();
        });
    }

    // wait for all the threads to join back in
    for (auto&& thread : threadPool)
    {
        thread.join();
    }
    
    return 0;
}