#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <list>

enum class CmdType
{
    PAYLOAD,
    END_OF_FILE,
    START_DYNAMIC,
    END_DYNAMIC
};

class ISubscriber
{
public:
    virtual void update(const std::vector<std::string>& cmdPack) = 0;
    virtual ~ISubscriber() = default;
};

class IPublisher
{
public:
    virtual void subscribe(const std::shared_ptr<ISubscriber>& sub) = 0;
    ~IPublisher() = default;
};

class Parser : public IPublisher
{
public:
    Parser(size_t packetSize);
    ~Parser() = default;
    void parse(const std::string& cmd);
    void subscribe(const std::shared_ptr<ISubscriber>& sub) override;

private:
    void notify();
    CmdType findType(const std::string& cmd);

    std::vector<std::string> m_packet;
    size_t m_packetSize;
    std::list<std::weak_ptr<ISubscriber>> m_subs;
    bool m_isDynamicStarted = false;
    bool m_isNested = false;
};

class ICommand
{
public:
    virtual void execute() = 0;
    virtual ~ICommand() = default;
};

class ParseCommand : public ICommand
{
public:
    ParseCommand(const std::shared_ptr<Parser>& parser, const std::string& cmd);
    ~ParseCommand() = default;
    void execute() override;

private:
    std::weak_ptr<Parser> m_parser;
    std::string m_cmd;
};