#include <cstring>
#include <sstream>

#include "dbmanager.h"

DbManager::DbManager() :
    m_dbName{"join_server_db.sqlite"},
    m_handle{nullptr}
{
    if(sqlite3_open(m_dbName, &m_handle))
    {
        std::cout << "Can't open db: " << sqlite3_errmsg(m_handle) << std::endl;
        sqlite3_close(m_handle);
    }
    else
    {
        std::cout << "Db opened successfully" << std::endl;
    }
}

DbManager::~DbManager()
{
    std::cout << "Db closed" << std::endl;
    sqlite3_close(m_handle);
}

std::tuple<ErrorCode, std::string> DbManager::execute(ErrorCode cmd,
                                                        std::vector<std::string> &cmdTokens)
{
    std::string query = makeQuery(cmd, cmdTokens);
    // int columns;
    // char **data;
    // char **names;
    auto print_results = [](void*, int columns, char **data, char **names) -> int {
        for(int i = 0; i < columns; ++i)
        {
            std::cout << names[i] << " = " << (data[i] ? data[i] : "NULL") << std::endl;
        }
        std::cout << std::endl;
        return 0;
    };

    char *errmsg;
    int rc = sqlite3_exec(m_handle, query.c_str(), print_results, 0, &errmsg);
    if(rc != SQLITE_OK)
    {
        std::cout << "Can not execute query: " << errmsg << std::endl;
        sqlite3_free(errmsg);
        sqlite3_close(m_handle);
        std::stringstream ss;
        ss << errmsg;
        auto len = strlen(errmsg);
        std::string retStr(errmsg);
        std::string retStr1(ss.str());
        return {ErrorCode::ERROR, std::string(errmsg)};
    }
    return {ErrorCode::OK, ""};
}

std::string DbManager::makeQuery(ErrorCode &cmd, std::vector<std::string> &cmdTokens)
{
    std::string query;
    size_t queryLength;
    switch (cmd)
    {
    case ErrorCode::INSERT:
        queryLength = snprintf(NULL, 0, "INSERT INTO %s VALUES(%s, '%s')",
                                cmdTokens[1].c_str(),
                                cmdTokens[2].c_str(),
                                cmdTokens[3].c_str()) + 1;
        query = std::string(queryLength, 0x00);
        snprintf(&query[0], queryLength, "INSERT INTO %s VALUES(%s, '%s')",
                    cmdTokens[1].c_str(),
                    cmdTokens[2].c_str(),
                    cmdTokens[3].c_str());
        break;

    case ErrorCode::TRUNCATE:
        /* code */
        break;

    case ErrorCode::INTERSECTION:
        /* code */
        break;

    case ErrorCode::SYMMETRIC_DIFFERENCE:
        /* code */
        break;

    default:
        break;
    }
    return query;
}