#include <vector>
#include <thread>
#include <boost/asio.hpp>
#include "Core/Client.hpp"
#include "UI/basic.hpp"

int main()
{
    boost::asio::io_context context;

    std::shared_ptr<Client> client = std::make_shared<Client>( &context, "127.0.0.1", "8080" );
    client->connect();


    std::vector<std::thread> threadPool;
    int poolSize = 10;

    for (int i = 0; i < poolSize; i++)
    {
        threadPool.emplace_back(std::thread([&]() {
            context.run();
        }));
    }

    // spawn the UI thread
    std::thread uiThread([&]() {
        BasicUI ui{ client->receiveQueue(), client->writeQueue() };
        ui.start();
    });

    // wait for all the threads to join back in
    for (auto&& thread : threadPool)
    {
        thread.join();
    }

    // wait for the UI thread to finish as well
    uiThread.join();

    return 0;
}