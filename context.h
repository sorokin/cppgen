#ifndef CONTEXT_H
#define CONTEXT_H

#include <cstdlib>
#include <boost/random.hpp>

struct context
{
    context(size_t n, size_t, int seed);

    size_t tokens_left;
    size_t max_nesting_depth;
    size_t nesting_depth;
    size_t indent;

    boost::mt19937 rng;
};

void print_indent(context const& c);
void print_linefeed(context const& c);

#endif // CONTEXT_H
