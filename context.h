#ifndef CONTEXT_H
#define CONTEXT_H

#include <cstdlib>
#include <boost/random.hpp>

struct context
{
    context(size_t);

    size_t max_nesting_depth;
    size_t indent;

    boost::mt19937 rng;
};

void print_indent(context const& c);
void print_linefeed(context const& c);
void print_linefeed_and_tab(context& c);
void print_linefeed_and_untab(context& c);
void open_brace(context& c);
void close_brace(context& c);

#endif // CONTEXT_H
