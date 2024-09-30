#include "parser.h"

Parser::Parser(size_t packetSize) :
    m_packetSize{packetSize}
{
    m_packet.reserve(m_packetSize);
}

void Parser::parse(const std::string& cmd)
{
    CmdType cmdType = findType(cmd);
    
    if(cmdType == CmdType::START_DYNAMIC)
    {
        if(m_isDynamicStarted)
        {
            m_isNested = true;
            return;
        }

        if(m_packet.size())
        {
            notify();
        }
        
        m_packet.clear();
        m_isDynamicStarted = true;
        return;
    }

    if(cmdType == CmdType::END_DYNAMIC)
    {
        if(m_isNested)
        {
            m_isNested = false;
            return;
        }
        notify();
        m_packet.clear();
        m_isDynamicStarted = false;
        m_isNested = false;
        return;
    }
    
    if(cmdType == CmdType::PAYLOAD)
    {
        m_packet.push_back(cmd);
    }

    if((m_packet.size() == m_packetSize) && (m_isDynamicStarted == false))
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

CmdType Parser::findType(const std::string& cmd)
{
    if(cmd.empty())
    {
        return CmdType::END_OF_FILE;
    }
    else if(cmd == "{")
    {
        return CmdType::START_DYNAMIC;
    }
    else if(cmd == "}")
    {
        return CmdType::END_DYNAMIC;
    }
    else
    {
        return CmdType::PAYLOAD;
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