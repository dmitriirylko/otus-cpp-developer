#include "duplicatefinder.h"

void DuplicateFinder::createFilePool()
{
    m_filePool.clear();
    for(const auto path : Config::instance().getIncludedFolderPaths())
    {
        if(Config::instance().getScanningLevel() == 1)
        {
            iterateFolder<boost::filesystem::recursive_directory_iterator>(path);
        }
        else if(Config::instance().getScanningLevel() == 0)
        {
            iterateFolder<boost::filesystem::directory_iterator>(path);
        }
        else
        {
            std::invalid_argument("There is no such type of scanning level.");
        }
    }
}

template<typename T>
void DuplicateFinder::iterateFolder(const std::string& path)
{
    /* Iterate through folder (recursively or not). */
    for(T iter{path}; iter != T{}; ++iter)
    {
        /* Check if this path corresponds to regular file. */
        if(boost::filesystem::status(*iter).type() != boost::filesystem::regular_file)
        {
            continue;
        }
        /* Check if this path is not contains in excluded folders. */
        bool isIncluded = true;
        for(const auto &excludedPath : Config::instance().getExcludedFolderPaths())
        {
            if(isSubpath(iter->path(), excludedPath))
            {
                isIncluded = false;
                break;
            }
        }
        if(!isIncluded) continue;
        /* Check file size requirements. */
        if(boost::filesystem::file_size(iter->path()) < Config::instance().getMinFileSize()) continue;
        /* Check mask. */
        
        m_filePool.push_back(iter->path());
    }
}

bool DuplicateFinder::isSubpath(const boost::filesystem::path &queryPath,
                                const boost::filesystem::path &excludedFolder)
{
    auto relativePath = boost::filesystem::relative(queryPath, excludedFolder);
    return !relativePath.empty() && relativePath.native()[0] != '.';
}