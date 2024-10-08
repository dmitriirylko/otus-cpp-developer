#include "filelogger.h"

void FileLogger::updatePacketReady(const std::vector<std::string>& cmdPack)
{
    std::cout << "file logger update" << std::endl;
}

void FileLogger::updatePacketStarted()
{
    std::cout << "file logger started" << std::endl;
}