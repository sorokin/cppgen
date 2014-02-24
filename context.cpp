#include "context.h"

context::context(size_t n, size_t max_nesting_depth, int seed)
    : tokens_left(n)
    , max_nesting_depth(max_nesting_depth)
    , nesting_depth()
    , indent()
    , rng(seed)
{}

void print_indent(context const& c)
{
    std::cout << std::string(c.indent * 4, ' ');
}

void print_linefeed(context const& c)
{
    std::cout << "\n";
    print_indent(c);
}
