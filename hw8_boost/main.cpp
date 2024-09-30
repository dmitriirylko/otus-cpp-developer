#include <iostream>
#include <vector> 

#include <boost/filesystem/operations.hpp>

int main()
{
    std::string directoryPath{"/home/dmitriirylko/Develop/otus-cpp-developer/hw8_boost/test_folder"};
    std::vector<boost::filesystem::path> filePaths;
    for(boost::filesystem::recursive_directory_iterator iter{directoryPath};
        iter != boost::filesystem::recursive_directory_iterator{};
        ++iter)
    {
        if(boost::filesystem::status(*iter).type() == boost::filesystem::regular_file)
        {
            filePaths.push_back(iter->path());
        }
    }
    
    for(const auto& path : filePaths)
    {
        std::cout << path << std::endl;
    }
}