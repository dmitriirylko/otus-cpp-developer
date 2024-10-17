#pragma once

#include <vector>
#include <string>
#include <stdexcept>

#include <boost/filesystem/operations.hpp>

#include "config.h"

class DuplicateFinder
{
public:
    DuplicateFinder() = default;
    ~DuplicateFinder() = default;
    void createFilePool();
    template<typename T>
    void iterateFolder(const std::string& path);

private:
    std::vector<boost::filesystem::path> m_filePool;
};