#include "dbmanager.h"

DbManager::DbManager() :
    m_dbName{"cpp_sqlite_db"},
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
    sqlite3_close(m_handle);
}

void DbManager::execute(const std::string &query)
{
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
        std::cout << "Can not execut query: " << errmsg << std::endl;
        sqlite3_free(errmsg);
        sqlite3_close(m_handle);
    }
}