#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <list>
#include <stdexcept>

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
    Parser(size_t packetSize);
    ~Parser() = default;
    void receive(const char* data, size_t size);
    // void subscribePacketStarted(const std::shared_ptr<ISubscriber>& sub) override;
    // void subscribePacketReady(const std::shared_ptr<ISubscriber>& sub) override;

protected:
    // void notifyPacketStarted() override;
    // void notifyPacketReady() override;

private:
    CmdType findType(const std::string& cmd);

    /**
     * @brief Stores and assembles current cmd from characters.
     */
    std::string m_currentCmd;

    /**
     * Stores and assembles current cmd packet.
     */
    std::vector<std::string> m_packet;

    size_t m_defaultPacketSize;
    // std::list<std::weak_ptr<ISubscriber>> m_subsPacketStarted;
    // std::list<std::weak_ptr<ISubscriber>> m_subsPacketReady;
    int m_nestingLevel = 0;

    void parse(const std::string& cmd);
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