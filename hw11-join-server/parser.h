#pragma once

#include <iostream>
#include <vector>
#include <tuple>
#include <map>

enum class ParserErrorCode
{
    OK,
    ERROR,
    SKIP
};

enum class CmdType
{
    INSERT,
    TRUNCATE,
    INTERSECTION,
    SYMMETRIC_DIFFERENCE
};

class Parser
{
public:

    Parser() = default;
    ~Parser() = default;
    std::tuple<ParserErrorCode, std::string> process(const char* data, size_t length);

private:
    std::string parse();
    std::string m_currentCmd;
    std::vector<std::string> m_cmdTokens;
    std::map<std::string, CmdType> m_cmdTypes = {{"insert", CmdType::INSERT},
                                                 {"truncate", CmdType::TRUNCATE},
                                                 {"intersection", CmdType::INTERSECTION},
                                                 {"symmetric_difference", CmdType::SYMMETRIC_DIFFERENCE}};
};