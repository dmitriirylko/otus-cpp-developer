#include <iostream>

#include "cmdsource.h"

int main()
{
    constexpr size_t defaultPacketSize = 3;
    CmdSource source(std::make_unique<CmdStdInputSource>());
    auto parser = std::make_shared<CmdParser>(defaultPacketSize);
    source.subscribe(parser);
    while(source.getCmd())
    {}
    return 0;
}