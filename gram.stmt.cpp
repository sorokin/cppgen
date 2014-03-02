#include "gram.stmt.h"
#include "context.h"
#include "random_utils.h"

#include "gram.lex.h"
#include "gram.dcl.h"
#include "gram.expr.h"

using namespace random_utils;

void generate_statement(context& c)
{
    // labeled-statement
    // attribute-specifier-seq[opt] expression-statement
    // attribute-specifier-seq[opt] compound-statement
    // attribute-specifier-seq[opt] selection-statement
    // attribute-specifier-seq[opt] iteration-statement
    // attribute-specifier-seq[opt] jump-statement
    // declaration-statement
    // attribute-specifier-seq[opt] try-block

    static std::pair<double, void (*)(context&)> options[] = {
        {1., &generate_labeled_statement},
        {1., &generate_expression_statement},
        {1., &generate_declaration_statement}
    };

    take_random_weighted(c.rng, options)(c);
}

void generate_labeled_statement(context& c)
{
    // attribute-specifier-seq[opt] identifier : statement
    // attribute-specifier-seq[opt] case constant-expression : statement
    // attribute-specifier-seq[opt] default : statement

    static std::pair<double, void (*)(context&)> options[] = {
        {1., [](context& c) {
             generate_identifier(c);
             std::cout << ": ";
        }},
        {1., [](context& c) {
             std::cout << "case ";
             generate_constant_expression(c);
             std::cout << ": ";
        }},
        {1., [](context&) {
             std::cout << "default: ";
        }}
    };

    take_random_weighted(c.rng, options)(c);
    generate_statement(c);
}

void generate_expression_statement(context& c)
{
    // expression[opt] ;

    if (probably(c.rng, 0.95))
        generate_expression(c);

    std::cout << ";";
    print_linefeed(c);
}

void generate_compound_statement(context& c)
{
    // { statement-seq[opt] }

    open_brace(c);
    if (probably(c.rng, 0.95))
        generate_statement_seq(c);
    close_brace(c);
}

void generate_statement_seq(context& c)
{
    // statement
    // statement-seq statement

    if (probably(c.rng, 0.4))
        generate_statement(c);
    else
    {
        generate_statement_seq(c);
        generate_statement(c);
    }
}

void generate_declaration_statement(context& c)
{
    generate_block_declaration(c);
}
