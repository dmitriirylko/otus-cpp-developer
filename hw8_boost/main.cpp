#include <iostream>

#include "config.h"
#include "duplicatefinder.h"

int main(int argc, char* argv[])
{
    try
    {
        Config::instance().setData(argc, argv);
        DuplicateFinder dupFinder;
        dupFinder.createFilePool();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    // std::string directoryPath{"/home/dmitriirylko/Develop/otus-cpp-developer/hw8_boost/test_folder"};
    // std::vector<boost::filesystem::path> filePaths;
    // for(boost::filesystem::recursive_directory_iterator iter{directoryPath};
    //     iter != boost::filesystem::recursive_directory_iterator{};
    //     ++iter)
    // {
    //     if(boost::filesystem::status(*iter).type() == boost::filesystem::regular_file)
    //     {
    //         filePaths.push_back(iter->path());
    //     }
    // }
    
    // for(const auto& path : filePaths)
    // {
    //     std::cout << path << std::endl;
    // }

    
    // std::vector<std::string> vec;
    // vec.emplace_back("path1");
    // vec.emplace_back("path2");
    // Config::instance().setIncludedFolderPaths(vec);
    // Config::instance().setExcludedFolderPaths(vec);
}