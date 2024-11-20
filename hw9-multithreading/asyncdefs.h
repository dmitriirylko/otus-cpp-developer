#pragma once

#include <memory>
#include <vector>
#include <string>

#include "waitingqueue.h"

namespace async {
    using ConsoleQueue_t = WaitingQueue<std::vector<std::string>>;
    using ConsoleQueueShared_t = std::shared_ptr<ConsoleQueue_t>;
    using ConsoleQueueWeak_t = std::weak_ptr<ConsoleQueue_t>;
}
