#include <iostream>
#include <vector> 

#include <boost/filesystem/operations.hpp>
#include <boost/program_options.hpp>

int main(int argc, char* argv[])
{

    // boost::program_options::option_description desc("");
    // desc.add_options()("opt", po::value<std::vector<int> >()->multitoken(), "description");
    

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