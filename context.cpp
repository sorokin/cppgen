#include "context.h"

#include <boost/random/random_device.hpp>

context::context(size_t max_nesting_depth)
    : max_nesting_depth(max_nesting_depth)
    , nesting_depth()
    , indent()
{
    boost::random::random_device dev;
    rng.seed(dev);
}

void print_indent(context const& c)
{
    std::cout << std::string(c.indent * 4, ' ');
}

void print_linefeed(context const& c)
{
    std::cout << "\n";
    print_indent(c);
}

void open_brace(context& c)
{
    print_linefeed(c);
    std::cout << "{";
    ++c.nesting_depth;
    ++c.indent;

    print_linefeed(c);
}

void close_brace(context& c)
{
    --c.indent;
    --c.nesting_depth;

    print_linefeed(c);
    std::cout << "}";
    print_linefeed(c);
}
