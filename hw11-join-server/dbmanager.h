#pragma once

#include <iostream>

#include "sqlite3.h"

class DbManager
{
public:
    DbManager();
    ~DbManager();
    void execute(const std::string &query);

private:
    const char* m_dbName;
    sqlite3* m_handle;
};