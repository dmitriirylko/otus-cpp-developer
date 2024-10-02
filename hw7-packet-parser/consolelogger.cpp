#include "consolelogger.h"

void ConsoleLogger::updatePacketReady(const std::vector<std::string>& cmdPack)
{
    size_t cnt = 0;
    for(const auto& cmd : cmdPack)
    {
        if(cnt == 0)
        {
            std::cout << cmd;
        }
        else
        {
            std::cout << ", " << cmd;
        }
        ++cnt;
    }
    std::cout << std::endl;
}