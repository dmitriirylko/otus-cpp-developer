#include <iostream>

#include <boost/filesystem/operations.hpp>

#include "config.h"

int main(int argc, char* argv[])
{
    CommandLineArgsParser parser(argc, argv);
    // boost::program_options::variables_map vm;
    // boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
    // boost::program_options::notify(vm);

    // if(vm.count("help"))
    // {
    //     std::cout << desc << "\n";
    //     return 1;
    // }

    // if(vm.count("compression"))
    // {
    //     std::cout << "Compression level was set to " << vm["compression"].as<std::vector<int>>().empty() << ".\n";
    // }
    // else
    // {
    //     std::cout << "Compression level was not set.\n";
    // }

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
    std::vector<std::string> vec;
    vec.emplace_back("path1");
    vec.emplace_back("path2");
    Config::instance().setIncludedFolderPaths(vec);
    Config::instance().setExcludedFolderPaths(vec);
}