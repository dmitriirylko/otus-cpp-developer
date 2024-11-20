#include "parser.h"

Parser::Parser(const async::ConsoleQueueShared_t& queue, size_t packetSize) :
    m_queue{queue},
    m_defaultPacketSize{packetSize}
{
    m_packet.reserve(m_defaultPacketSize);
}

void Parser::receive(const char* data, size_t size)
{
    for(size_t i = 0; i < size; ++i)
    {
        /* End of cmd. */
        if(data[i] == '\n' || data[i] == '\0')
        {
            parse(m_currentCmd);
            m_currentCmd.clear();
        }
        /* Append character to cmd. */
        else
        {
            m_currentCmd.push_back(data[i]);
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

void Parser::parse(const std::string& cmd)
{
    std::cout << cmd << std::endl;

    CmdType cmdType = findType(cmd);
    
    switch (cmdType)
    {
    case CmdType::START_DYNAMIC:
        ++m_nestingLevel;
        if(m_nestingLevel > 1)
        {
            return;
        }
        if(m_packet.size())
        {
            // notifyPacketReady();
            notifyConsolePacketReady();
        }
        m_packet.clear();
        return;

    case CmdType::END_DYNAMIC:
        if(m_nestingLevel == 0)
        {
            return;
        }
        --m_nestingLevel;
        if(m_nestingLevel)
        {
            return;
        }
        // notifyPacketReady();
        notifyConsolePacketReady();
        m_packet.clear();
        return;

    case CmdType::PAYLOAD:
        if(m_packet.size() == 0)
        {
            // notifyPacketStarted();
        }
        m_packet.push_back(cmd);
        if((m_packet.size() == m_defaultPacketSize) && (!m_nestingLevel))
        {
            // notifyPacketReady();
            notifyConsolePacketReady();
            m_packet.clear();
        }
        return;

    case CmdType::END_OF_FILE:
        if(m_nestingLevel > 0)
        {
            m_nestingLevel = 0;
            m_packet.clear();
            return;
        }
        if(m_packet.size())
        {
            // notifyPacketReady();
            notifyConsolePacketReady();
            m_packet.clear();
        }
        return;

    default:
        throw std::logic_error("There is no that type of command in parser.");
    }
}

void Parser::notifyConsolePacketReady()
{
    auto queue = m_queue.lock();
    if(queue) queue->push(m_packet);
}

void Parser::notifyFilePacketReady()
{
    
}

// void Parser::subscribePacketStarted(const std::shared_ptr<ISubscriber>& sub)
// {
//     m_subsPacketStarted.emplace_back(sub);
// }

// void Parser::subscribePacketReady(const std::shared_ptr<ISubscriber>& sub)
// {
//     m_subsPacketReady.emplace_back(sub);
// }

// void Parser::notifyPacketStarted()
// {
//     auto iter = m_subsPacketStarted.begin();
//     while(iter != m_subsPacketStarted.end())
//     {
//         auto ptr = iter->lock();
//         if(ptr)
//         {
//             ptr->updatePacketStarted();
//             ++iter;
//         }
//         else
//         {
//             m_subsPacketReady.erase(iter++);
//         }
//     }
// }

// ParseCommand::ParseCommand(const std::shared_ptr<Parser>& parser, const std::string& cmd) :
//     m_parser{parser},
//     m_cmd{cmd}
// {}

// void ParseCommand::execute()
// {
//     auto ptr = m_parser.lock();
//     if(ptr)
//     {
//         ptr->parse(m_cmd);
//     }
// }