#include "gram.expr.h"
#include "context.h"
#include "random_utils.h"

#include "gram.lex.h"

using namespace random_utils;

void generate_assignment_expression(context& c)
{
    generate_identifier(c);
}

void generate_expression(context& c)
{
    generate_identifier(c);
}

void generate_constant_expression(context& c)
{
    generate_identifier(c);
}
