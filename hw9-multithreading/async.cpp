#include <mutex>
#include <list>

#include "async.h"
#include "asyncdefs.h"
#include "parser.h"
#include "waitingqueue.h"
#include "consolelogger.h"

namespace async {

/**
 * @brief One parser for one specific context.
 */
std::list<std::unique_ptr<Parser>> m_contexts;

/**
 * @brief Mutex for thread-safe context addition (connect), deletion (disconnect)
 *          and using (receive).
 */
std::mutex mtxContext; 

ConsoleQueueShared_t consoleQueue{};

ConsoleLogger consoleLogger(consoleQueue);
ConsoleLogger consoleLogger1(consoleQueue);

handle_t connect(std::size_t bulkSize)
{
    std::lock_guard<std::mutex> lck{mtxContext};
    std::cout << "connecting: " << std::this_thread::get_id() << std::endl;
    m_contexts.emplace_back(std::make_unique<Parser>(consoleQueue, bulkSize));
    return m_contexts.back().get();
}

void receive(handle_t handle, const char *data, std::size_t size)
{
    std::lock_guard<std::mutex> lck{mtxContext};
    auto it = m_contexts.begin();
    while(it != m_contexts.end())
    {
        if(it->get() == handle)
        {
            it->get()->receive(data, size);
        }
        ++it;
    }
}

void disconnect(handle_t handle)
{
    std::lock_guard<std::mutex> lck{mtxContext};
    std::cout << "disconnecting: " << std::this_thread::get_id() << std::endl;
    auto it = m_contexts.begin();
    while(it != m_contexts.end())
    {
        if(it->get() == handle)
        {
            it->get()->receive("", 1);
            m_contexts.erase(it++);
        }
        else
        {
            ++it;
        }
    }
}

}
