#include <mutex>
#include <list>

#include "async.h"
#include "asyncdefs.h"
#include "parser.h"
#include "waitingqueue.h"
#include "consolelogger.h"
#include "filelogger.h"

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

ConsoleQueueShared_t consoleQueue = std::make_shared<ConsoleQueue_t>();
FileQueueShared_t fileQueue = std::make_shared<FileQueue_t>();

ConsoleLogger consoleLogger(consoleQueue);
FileLogger fileLogger{fileQueue};

handle_t connect(std::size_t bulkSize)
{
    std::lock_guard<std::mutex> lck{mtxContext};
    m_contexts.emplace_back(std::make_unique<Parser>(consoleQueue, fileQueue, bulkSize));
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

void stop()
{
    consoleQueue->stop();
    fileQueue->stop();
}

}
