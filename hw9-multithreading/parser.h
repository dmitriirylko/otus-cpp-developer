#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <list>
#include <stdexcept>

#include "asyncdefs.h"
#include "filelogger.h"

enum class CmdType
{
    PAYLOAD,
    END_OF_FILE,
    START_DYNAMIC,
    END_DYNAMIC
};

class Parser
{
public:
    Parser(const async::ConsoleQueueShared_t& consoleQueue,
           const async::FileQueueShared_t& fileQueue,
           size_t packetSize);
    
    ~Parser() = default;
    
    void receive(const char* data, size_t size);

private:
    async::ConsoleQueueWeak_t m_consoleQueue;
    async::FileQueueWeak_t m_fileQueue;
    
    /**
     * @brief Stores and assembles current cmd from characters.
     */
    std::string m_currentCmd;

    /**
     * @brief Stores and assembles current cmd packet.
     */
    std::vector<std::string> m_packet;

    /**
     * @brief Default (without of nesting) amount of cmds in packet.
     */
    size_t m_defaultPacketSize;
    
    /**
     * @brief Current nesting level.
     */
    int m_nestingLevel = 0;

    /**
     * @brief Last cmd receive time.
     */
    std::time_t m_lastCmdRecvTime;

    /**
     * @brief Finds type of cmd: empty cmd interprets as EOF, curly braces as start
     *          and stop markers for nesting, otherwise is payload (ordinary cmd).
     * @param cmd Command.
     * @return Cmd type.
     */
    CmdType findType(const std::string& cmd);
    
    /**
     * @brief Makes decisions about current cmd: accumulate cmd to packet, start/stop accumulation.
     * @param cmd Cmd.
     */
    void parse(const std::string& cmd);

    /**
     * @brief Notifies console logger thread that cmd packet is ready.
     */
    void notifyConsolePacketReady();

    /**
     * @brief Notifies file logger threads that cmd packet is ready.
     */
    void notifyFilePacketReady();
};