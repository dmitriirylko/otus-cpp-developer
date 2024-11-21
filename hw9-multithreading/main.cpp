#include <iostream>
#include <thread>
#include <cstddef>

#include "async.h"

int main(int, char *[])
{
    std::size_t bulkSize = 5;

    auto h = async::connect(bulkSize);
    async::receive(h, "1", 1);
    async::receive(h, "\n2\n3\n4\n5\n6\n{\na\n", 15);
    async::receive(h, "b\nc\nd\n}\n89\n", 11);
    async::disconnect(h);

    auto h2 = async::connect(bulkSize);
    async::receive(h2, "1\n", 2);
    async::disconnect(h2);

    std::this_thread::sleep_for(std::chrono::seconds(1));

    async::stop();

    return 0;
}
