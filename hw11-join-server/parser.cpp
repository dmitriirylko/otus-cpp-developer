#include <sstream>
#include <algorithm>
#include <cassert>

#include "parser.h"

namespace ErrorMsgs
{
    constexpr const char* INVALID_QUERY_LENGTH = "invalid query length";
    constexpr const char* INVALID_CMD_TYPE = "invalid command type";
    constexpr const char* INVALID_ARGUMENTS_AMOUNT = "invalid arguments amount";
};

std::tuple<ParserErrorCode, std::string> Parser::process(const char* data, size_t length)
{
    for(size_t i = 0; i < length; ++i)
    {
        /* End of cmd. */
        if(data[i] == '\n' || data[i] == '\0')
        {
            std::string errorMsg = parse();
            m_currentCmd.clear();
            if(errorMsg.empty()) return {ParserErrorCode::OK, ""};
            else return {ParserErrorCode::ERROR, std::move(errorMsg)};
        }
        /* Append character to cmd. */
        else
        {
            m_currentCmd.push_back(data[i]);
        }
    }
    return {ParserErrorCode::SKIP, ""};
}

std::string Parser::parse()
{
    m_cmdTokens.clear();
    std::istringstream iss(m_currentCmd);
    std::string s;

    while(getline(iss, s, ' ')) m_cmdTokens.push_back(s);
    
    for_each(m_cmdTokens[0].begin(), m_cmdTokens[0].end(), [](char& c){
        c = tolower(c);
    });

    CmdType cmdType;
    try
    {
        /* Throws std::out_of_range if there is no such element in map, i.e. cmd is invalid. */
        cmdType =  m_cmdTypes.at(m_cmdTokens[0]);
        /* Check number of passed arguments. If arguments amount doesn't match particular command
           then throw std::invalid_argument. */
        switch (cmdType)
        {
        case CmdType::INSERT:
            if(m_cmdTokens.size() != 4) throw std::invalid_argument(ErrorMsgs::INVALID_ARGUMENTS_AMOUNT);
            break;

        case CmdType::TRUNCATE:
            if(m_cmdTokens.size() != 2) throw std::invalid_argument(ErrorMsgs::INVALID_ARGUMENTS_AMOUNT);
            break;

        case CmdType::INTERSECTION:
            if(m_cmdTokens.size() != 1) throw std::invalid_argument(ErrorMsgs::INVALID_ARGUMENTS_AMOUNT);
            break;

        case CmdType::SYMMETRIC_DIFFERENCE:
            if(m_cmdTokens.size() != 1) throw std::invalid_argument(ErrorMsgs::INVALID_ARGUMENTS_AMOUNT);
            break;

        default:
            assert(0 && "There is no such type of command");
        }
    }
    catch(const std::out_of_range &e)
    {
        return ErrorMsgs::INVALID_CMD_TYPE;
    }
    catch(const std::invalid_argument &e)
    {
        return e.what();
    }
    
    return "";
}