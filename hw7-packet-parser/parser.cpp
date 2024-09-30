#include "parser.h"

Parser::Parser(size_t packetSize) :
    m_packetSize{packetSize}
{
    m_packet.reserve(m_packetSize);
}

void Parser::parse(const std::string& cmd)
{
    if(!cmd.empty()) m_packet.push_back(cmd);
    if(m_packet.size() == m_packetSize ||
       cmd.empty())
    {
        notify();
        m_packet.clear();
    }
}

void Parser::subscribe(const std::shared_ptr<ISubscriber>& sub)
{
    m_subs.emplace_back(sub);
}

void Parser::notify()
{
    auto iter = m_subs.begin();
    while(iter != m_subs.end())
    {
        auto ptr = iter->lock();
        if(ptr)
        {
            ptr->update(m_packet);
            ++iter;
        }
        else
        {
            m_subs.erase(iter++);
        }
    }
}

ParseCommand::ParseCommand(const std::shared_ptr<Parser>& parser, const std::string& cmd) :
    m_parser{parser},
    m_cmd{cmd}
{}

void ParseCommand::execute()
{
    auto ptr = m_parser.lock();
    if(ptr)
    {
        ptr->parse(m_cmd);
    }
}