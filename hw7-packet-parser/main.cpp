#include <iostream>

#include "source.h"
#include "parser.h"
#include "consolelogger.h"

int main()
{
    constexpr size_t defaultPacketSize = 3;
    Source source(std::make_unique<StdInputSource>());
    auto parser = std::make_shared<Parser>(defaultPacketSize);
    auto consoleLogger = std::make_shared<ConsoleLogger>();
    parser->subscribePacketReady(consoleLogger);
    parser->subscribePacketStarted(consoleLogger);
    std::string str;
    do
    {
        str = source.getCmd();
        ParseCommand command(parser, str);
        command.execute();
    }
    while(!str.empty());
    
    return 0;
}