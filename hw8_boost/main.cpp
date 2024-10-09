#include <iostream>
#include <vector> 

#include <boost/filesystem/operations.hpp>
#include <boost/program_options.hpp>

int main(int argc, char* argv[])
{

    boost::program_options::options_description desc("Allowed options");
    desc.add_options()
    ("help", "produce help message")
    ("compression", boost::program_options::value<int>(), "set compression level");
    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
    boost::program_options::notify(vm); 
    if(vm.count("help"))
    {
        std::cout << desc << "\n";
        return 1;
    }

    if (vm.count("compression"))
    {
        std::cout << "Compression level was set to " << vm["compression"].as<int>() << ".\n";
    }
    else
    {
        std::cout << "Compression level was not set.\n";
    }

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