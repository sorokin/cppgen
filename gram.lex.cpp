#include "gram.lex.h"

#include <iostream>

#include "context.h"
#include "random_utils.h"

using namespace random_utils;

void generate_integer_literal(context& c)
{
    static const char* const suffices[] = {"", "u", "l", "ul", "ll", "ull"};

    std::cout << rand_0n(c.rng, 123) << take_random(c.rng, suffices) << " ";
}

void generate_identifier(context& c)
{
    const char* id = take_random_weighted(c.rng, {{20., "foo"}, {15., "bar"}, {7., "baz"}, {1., "qux"}, {0.1, "quux"},
                                                  {0.02, "corge"}, {0.02, "grault"}, {0.05, "garply"}, {0.05, "waldo"},
                                                  {0.02, "fred"}, {0.05, "plugh"}, {0.05, "xyzzy"}, {0.05, "thud"}});

    std::cout << id << rand_0n(c.rng, 7) << " ";
}

void generate_float_literal(context& c)
{
    static const char* const suffices[] = {".f", "."};

    std::cout << rand_0n(c.rng, 123) << take_random(c.rng, suffices) << " ";
}
