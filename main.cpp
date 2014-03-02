#include <iostream>

#include <boost/program_options.hpp>

#include "random_utils.h"
#include "context.h"
#include "gram.basic.h"

using namespace std;
using namespace random_utils;

namespace po = boost::program_options;

int main(int argc, char* argv[])
{
    try
    {
        size_t max_nesting_depth;
        po::options_description desc("Allowed options");
        desc.add_options()
            ("help", "produce help message")
            ("max-nesting-depth,d", po::value<size_t>(&max_nesting_depth)->default_value(10), "maximum depth of nesting");

        po::variables_map vm;
        po::store(po::command_line_parser(argc, argv).options(desc).run(), vm);
        po::notify(vm);

        if (vm.count("help"))
        {
            cout << "Usage: " << " [options]\n";
            cout << desc;
            return 0;
        }

        context ctx(max_nesting_depth);
        generate_translation_unit(ctx);
    }
    catch (std::exception const& e)
    {
        std::cout << "error: " << e.what() << std::endl;
    }

    return 0;
}

