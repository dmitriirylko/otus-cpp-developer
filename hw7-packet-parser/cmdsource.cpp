#include "cmdsource.h"

std::string CmdStdInputSource::getCmd()
{
    std::string str;
    std::cin >> str;
    return str;
}

CmdSource::CmdSource(std::unique_ptr<ICmdSource> sourcePtr) :
    m_pSource{std::move(sourcePtr)}
{}

bool CmdSource::getCmd()
{
    /* Get command from source. */
    std::string cmd = m_pSource->getCmd();
    /* Notify subscribers that command has been received. */
    auto iter = m_subs.begin();
    while(iter != m_subs.end())
    {
        auto ptr = iter->lock();
        if(ptr)
        {
            ptr->update(cmd);
            ++iter;
        }
        else
        {
            m_subs.erase(iter++);
        }
    }
    return cmd.empty() ? false : true;
}

void CmdSource::subscribe(const std::shared_ptr<ISubscriber>& sub)
{
    m_subs.emplace_back(sub);
}