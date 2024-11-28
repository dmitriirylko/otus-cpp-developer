#include <iostream>
#include <thread>
#include <cstddef>

#include <boost/program_options.hpp>

#include "async.h"

int main(int argc, char** argv)
{
    boost::program_options::options_description desc;
    desc.add_options()
    ("unnamed", boost::program_options::value<std::vector<size_t>>(), "unnamed");

    boost::program_options::positional_options_description pos_desc;
    pos_desc.add("unnamed", 2);

    boost::program_options::variables_map vm;

    try
    {
        if(argc == 1)
        {
            throw std::invalid_argument("Incorrect number of input parameters");
        }
        
        boost::program_options::store(boost::program_options::command_line_parser(argc, argv).
                                                              options(desc).
                                                              positional(pos_desc).
                                                              run(), vm);
        boost::program_options::notify(vm);
        if(vm["unnamed"].as<std::vector<size_t>>().size() != 2) {
            throw std::invalid_argument("Incorrect number of input parameters");
        }
    }
    catch (boost::program_options::error& e)
    {
        std::cout << "Program options failed. Error: " << e.what() << std::endl;
        return 1;
    }
    catch (const std::invalid_argument& e)
    {
        std::cout << "Program options failed. Error: " << e.what() << std::endl;
        return 1;
    }

    size_t host(vm["unnamed"].as<std::vector<size_t>>()[0]);
    size_t bulkSize(vm["unnamed"].as<std::vector<size_t>>()[1]);

    std::cout << host << ", " << bulkSize << std::endl;

    // std::size_t bulk = 5;
    // auto h = async::connect(bulk);
    // auto h2 = async::connect(bulk);
    // async::receive(h, "1", 1);
    // async::receive(h2, "1\n", 2);
    // async::receive(h, "\n2\n3\n4\n5\n6\n{\na\n", 15);
    // async::receive(h, "b\nc\nd\n}\n89\n", 11);
    // async::disconnect(h);
    // async::disconnect(h2);

    // async::stop();

    return 0;
}
