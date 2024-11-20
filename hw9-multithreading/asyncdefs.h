#pragma once

#include <memory>
#include <vector>
#include <string>

#include "waitingqueue.h"

namespace async {
    using ConsoleQueueShared_t = std::shared_ptr<WaitingQueue<std::vector<std::string>>>;
    using ConsoleQueueWeak_t = std::weak_ptr<WaitingQueue<std::vector<std::string>>>;
}
