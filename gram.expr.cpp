#include "gram.expr.h"
#include "context.h"
#include "random_utils.h"

#include "gram.lex.h"
#include "gram.decl.h"

using namespace random_utils;

void generate_additive_expression(context& c)
{
    generate_identifier(c);
}

void generate_shift_expression(context& c)
{
    // additive-expression
    // shift-expression << additive-expression
    // shift-expression >> additive-expression

    generate_additive_expression(c);

    while (probably(c.rng, .1))
    {
        static std::pair<double, char const*> options[] = {
            {1., "<<"},
            {1., ">>"},
        };

        std::cout << " " << take_random_weighted(c.rng, options) << " ";
        generate_additive_expression(c);
    }
}

void generate_relational_expression(context& c)
{
    // shift-expression
    // relational-expression < shift-expression
    // relational-expression > shift-expression
    // relational-expression <= shift-expression
    // relational-expression >= shift-expression

    generate_shift_expression(c);

    while (probably(c.rng, .1))
    {
        static std::pair<double, char const*> options[] = {
            {1., "<"},
            {1., ">"},
            {1., "<="},
            {1., ">="},
        };

        std::cout << " " << take_random_weighted(c.rng, options) << " ";
        generate_shift_expression(c);
    }
}

void generate_equality_expression(context& c)
{
    // relational-expression
    // equality-expression == relational-expression
    // equality-expression != relational-expression

    generate_relational_expression(c);

    while (probably(c.rng, .1))
    {
        static std::pair<double, char const*> options[] = {
            {1., "=="},
            {1., "!="},
        };

        std::cout << " " << take_random_weighted(c.rng, options) << " ";
        generate_relational_expression(c);
    }
}

void generate_and_expression(context& c)
{
    // equality-expression
    // and-expression & equality-expression

    generate_equality_expression(c);

    while (probably(c.rng, .1))
    {
        std::cout << " & ";
        generate_equality_expression(c);
    }
}

void generate_exclusive_or_expression(context& c)
{
    // and-expression
    // exclusive-or-expression ˆ and-expression

    generate_and_expression(c);

    while (probably(c.rng, .1))
    {
        std::cout << " ^ ";
        generate_and_expression(c);
    }
}

void generate_inclusive_or_expression(context& c)
{
    // exclusive-or-expression
    // inclusive-or-expression | exclusive-or-expression

    generate_exclusive_or_expression(c);

    while (probably(c.rng, .1))
    {
        std::cout << " | ";
        generate_exclusive_or_expression(c);
    }
}

void generate_logical_and_expression(context& c)
{
    // inclusive-or-expression
    // logical-and-expression && inclusive-or-expression

    generate_inclusive_or_expression(c);

    while (probably(c.rng, .1))
    {
        std::cout << " || ";
        generate_inclusive_or_expression(c);
    }
}

void generate_logical_or_expression(context& c)
{
    // logical-and-expression
    // logical-or-expression || logical-and-expression

    generate_logical_and_expression(c);

    while (probably(c.rng, .1))
    {
        std::cout << " || ";
        generate_logical_and_expression(c);
    }
}

void generate_conditional_expression(context &c)
{
    // logical-or-expression
    // logical-or-expression ? expression : assignment-expression

    generate_logical_or_expression(c);

    if (probably(c.rng, .1))
    {
        std::cout << " ? ";
        generate_expression(c);
        std::cout << " : ";
        generate_assignment_expression(c);
    }
}

void generate_assignment_expression(context& c)
{
    // conditional-expression
    // logical-or-expression assignment-operator initializer-clause
    // throw-expression

    if (probably(c.rng, .9))
        generate_conditional_expression(c);
    else
    {
        generate_logical_or_expression(c);
        std::cout << " ";
        generate_assignment_operator(c);
        std::cout << " ";
        generate_initializer_clause(c);
    }
}

void generate_assignment_operator(context& c)
{
    // one of = *= /= %= += -= >>= <<= &= ˆ= |=

    static std::pair<double, char const*> options[] = {
        {1., "="},
        {1., "*="},
        {1., "/="},
        {1., "%="},
        {1., "+="},
        {1., "-="},
        {1., ">>="},
        {1., "<<="},
        {1., "&="},
        {1., "^="},
        {1., "|="},
    };

    std::cout << take_random_weighted(c.rng, options);
}

void generate_expression(context& c)
{
    // assignment-expression
    // expression , assignment-expression

    generate_assignment_expression(c);

    while (probably(c.rng, .2))
    {
        std::cout << ", ";
        generate_assignment_expression(c);
    }
}

void generate_constant_expression(context& c)
{
    // conditional-expression

    generate_conditional_expression(c);
}
