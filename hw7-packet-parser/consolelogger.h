#pragma once

#include "parser.h"

class ConsoleLogger : public ISubscriber
{
public:
    ConsoleLogger() = default;
    ~ConsoleLogger() = default;
    void update(const std::vector<std::string>& cmdPack) override;

private:
};