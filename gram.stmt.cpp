#include "gram.stmt.h"
#include "context.h"
#include "random_utils.h"

#include "gram.lex.h"
#include "gram.dcl.h"
#include "gram.decl.h"
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
        {1., &generate_compound_statement},
        {1., &generate_selection_statement},
        {1., &generate_iteration_statement},
        {1., &generate_jump_statement},
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

    do
        generate_statement(c);
    while (probably(c.rng, 0.6));
}

void generate_selection_statement(context& c)
{
    // if ( condition ) statement
    // if ( condition ) statement else statement
    // switch ( condition ) statement

    if (probably(c.rng, 0.8))
    {
        std::cout << "if (";
        generate_condition(c);
        std::cout << ")";

        print_linefeed_and_tab(c);
        generate_statement(c);
        print_linefeed_and_untab(c);

        if (probably(c.rng, 0.4))
        {
            std::cout << "else";
            print_linefeed_and_tab(c);
            generate_statement(c);
            print_linefeed_and_untab(c);
        }
    }
    else
    {
        std::cout << "switch (";
        generate_condition(c);
        std::cout << ")";

        print_linefeed_and_tab(c);
        generate_statement(c);
        print_linefeed_and_untab(c);
    }
}

void generate_condition(context& c)
{
    // expression
    // attribute-specifier-seqopt decl-specifier-seq declarator = initializer-clause
    // attribute-specifier-seqopt decl-specifier-seq declarator braced-init-list

    if (probably(c.rng, 0.5))
        generate_expression(c);
    else
    {
        generate_decl_specifier_seq(c);
        std::cout << " ";
        generate_declarator(c, false);
        std::cout << " = ";
        generate_initializer_clause(c);
    }
}

void generate_iteration_statement(context& c)
{
    // while ( condition ) statement
    // do statement while ( expression ) ;
    // for ( for-init-statement condition[opt] ; expression[opt] ) statement
    // for ( for-range-declaration : for-range-initializer ) statement

    static std::pair<double, void (*)(context&)> options[] = {
        {1., [](context& c) {
            std::cout << "while (";
            generate_condition(c);
            print_linefeed_and_tab(c);
            generate_statement(c);
            print_linefeed_and_untab(c);
        }},
        {1., [](context& c) {
            std::cout << "do";
            print_linefeed_and_tab(c);
            generate_statement(c);
            print_linefeed_and_untab(c);
            std::cout << "while (";
            generate_expression(c);
            std::cout << ")";
            print_linefeed(c);
        }},
        {1., [](context& c) {
            std::cout << "for (";
            generate_for_init_statement(c);
            if (probably(c.rng, .5))
                generate_condition(c);
            std::cout << "; ";
            if (probably(c.rng, .5))
                generate_expression(c);
            std::cout << ")";
            print_linefeed_and_tab(c);
            generate_statement(c);
            print_linefeed_and_untab(c);
        }},
        {1., [](context& c) {
            std::cout << "for (";
            generate_for_range_declaration(c);
            std::cout << " : ";
            generate_for_range_initializer(c);
            std::cout << ")";
            print_linefeed_and_tab(c);
            generate_statement(c);
            print_linefeed_and_untab(c);
        }}};

    take_random_weighted(c.rng, options)(c);
}

void generate_for_init_statement(context& c)
{
    // expression-statement
    // simple-declaration

    if (probably(c.rng, 0.4))
        generate_expression_statement(c);
    else
        generate_simple_declaration(c);
}

void generate_for_range_declaration(context& c)
{
    // attribute-specifier-seq[opt] decl-specifier-seq declarator

    generate_decl_specifier_seq(c);
    std::cout << " ";
    generate_declarator(c, false);
}

void generate_for_range_initializer(context& c)
{
    // expression
    // braced-init-list

    generate_expression(c);
}

void generate_jump_statement(context& c)
{
    // break ;
    // continue ;
    // return expression[opt] ;
    // return braced-init-list ;
    // goto identifier ;

    static std::pair<double, void (*)(context&)> options[] = {
        {1., [](context& c) {
            std::cout << "break;";
            print_linefeed(c);
        }},
        {1., [](context& c) {
            std::cout << "continue;";
            print_linefeed(c);
        }},
        {1., [](context& c) {
            std::cout << "return";
            if (probably(c.rng, .6))
            {
               std::cout << " ";
               generate_expression(c);
            }
            print_linefeed(c);
        }},
        {1., [](context& c) {
            std::cout << "goto ";
            generate_identifier(c);
            std::cout << ";";
            print_linefeed(c);
        }}};

    take_random_weighted(c.rng, options)(c);
}

void generate_declaration_statement(context& c)
{
    generate_block_declaration(c);
}
