#pragma once

#include <string>
#include <iostream>
#include <vector>

class ISubscriber
{
public:
    virtual void update(const std::string& cmd) = 0;
    virtual ~ISubscriber() = default;
};

class CmdParser : public ISubscriber
{
public:
    CmdParser(size_t packetSize);
    ~CmdParser() = default;
    void update(const std::string& cmd) override;

private:
    void print();
    std::vector<std::string> m_cmdPacket;
    size_t m_packetSize;
};