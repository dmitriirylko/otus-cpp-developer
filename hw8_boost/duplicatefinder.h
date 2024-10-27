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

private:
    template<typename T>
    void iterateFolder(const std::string& path);
    bool isSubpath(const boost::filesystem::path &queryPath,
                   const boost::filesystem::path &excludedFolder);
    std::vector<boost::filesystem::path> m_filePool;
};