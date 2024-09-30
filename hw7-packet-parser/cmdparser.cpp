#include "cmdparser.h"

CmdParser::CmdParser(size_t packetSize) :
    m_packetSize{packetSize}
{
    m_cmdPacket.reserve(m_packetSize);
}

void CmdParser::update(const std::string& cmd)
{
    if(!cmd.empty()) m_cmdPacket.push_back(cmd);
    if(m_cmdPacket.size() == m_packetSize ||
       cmd.empty())
    {
        print();
        m_cmdPacket.clear();
    }
}

void CmdParser::print()
{
    size_t cnt = 0;
    for(const auto& cmd : m_cmdPacket)
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