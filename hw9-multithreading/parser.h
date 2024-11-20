#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <list>
#include <stdexcept>

#include "asyncdefs.h"

enum class CmdType
{
    PAYLOAD,
    END_OF_FILE,
    START_DYNAMIC,
    END_DYNAMIC
};

// class ISubscriber
// {
// public:
//     virtual void updatePacketReady(const std::vector<std::string>& cmdPack) = 0;
//     virtual void updatePacketStarted()
//     {}
//     virtual ~ISubscriber() = default;
// };

// class IPublisher
// {
// public:
//     virtual void subscribePacketStarted(const std::shared_ptr<ISubscriber>& sub) = 0;
//     virtual void subscribePacketReady(const std::shared_ptr<ISubscriber>& sub) = 0;

// protected:
//     virtual void notifyPacketStarted() = 0;
//     virtual void notifyPacketReady() = 0;
//     ~IPublisher() = default;
// };

class Parser /*: public IPublisher*/
{
public:
    Parser(const async::ConsoleQueueShared_t& queue, size_t packetSize);
    
    ~Parser() = default;
    
    void receive(const char* data, size_t size);
    
    // void subscribePacketStarted(const std::shared_ptr<ISubscriber>& sub) override;
    // void subscribePacketReady(const std::shared_ptr<ISubscriber>& sub) override;

protected:
    // void notifyPacketStarted() override;

private:
    const async::ConsoleQueueWeak_t& m_queue;
    
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

    // std::list<std::weak_ptr<ISubscriber>> m_subsPacketStarted;
    // std::list<std::weak_ptr<ISubscriber>> m_subsPacketReady;
    
    /**
     * @brief Current nesting level.
     */
    int m_nestingLevel = 0;

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

// class ICommand
// {
// public:
//     virtual void execute() = 0;
//     virtual ~ICommand() = default;
// };

// class ParseCommand : public ICommand
// {
// public:
//     ParseCommand(const std::shared_ptr<Parser>& parser, const std::string& cmd);
//     ~ParseCommand() = default;
//     void execute() override;

// private:
//     std::weak_ptr<Parser> m_parser;
//     std::string m_cmd;
// };