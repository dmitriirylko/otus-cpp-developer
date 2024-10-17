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
    int aa = 11;
}

template<typename T>
void DuplicateFinder::iterateFolder(const std::string& path)
{
    for(T iter{path};
        iter != T{};
        ++iter)
    {
        if(boost::filesystem::status(*iter).type() == boost::filesystem::regular_file)
        {
            m_filePool.push_back(iter->path());
        }
    }

}