#include "context.h"

#include <iostream>
#include <random>

context::context(size_t max_nesting_depth)
    : max_nesting_depth(max_nesting_depth)
    , indent()
{
    std::random_device dev;
    std::seed_seq seq{dev(), dev(), dev()};
    rng.seed(seq);
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

void print_linefeed_and_tab(context& c)
{
    ++c.indent;
    print_linefeed(c);
}

void print_linefeed_and_untab(context& c)
{
    --c.indent;
    print_linefeed(c);
}

void open_brace(context& c)
{
    print_linefeed(c);
    std::cout << "{";
    print_linefeed_and_tab(c);
}

void close_brace(context& c)
{
    print_linefeed_and_untab(c);
    std::cout << "}";
    print_linefeed(c);
}
