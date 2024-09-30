#pragma once

#include <string>
#include <iostream>
#include <memory>
#include <list>

#include "cmdparser.h"

/**
 * @brief Interface class for command source classes. Sources for
 *        commands can be standard input stream, files, network
 *        and so on.
 */
class ICmdSource
{
public:
    virtual std::string getCmd() = 0;
    virtual ~ICmdSource() = default;
};

class CmdStdInputSource : public ICmdSource
{
public:
    CmdStdInputSource() = default;
    ~CmdStdInputSource() = default;
    std::string getCmd() override;
};

class IPublisher
{
public:
    virtual void subscribe(const std::shared_ptr<ISubscriber>& sub) = 0;
    ~IPublisher() = default;
};

class CmdSource : IPublisher
{
public:
    CmdSource(std::unique_ptr<ICmdSource> sourcePtr);
    ~CmdSource() = default;
    bool getCmd();
    void subscribe(const std::shared_ptr<ISubscriber>& sub) override;

private:
    std::unique_ptr<ICmdSource> m_pSource;
    std::list<std::weak_ptr<ISubscriber>> m_subs;
};