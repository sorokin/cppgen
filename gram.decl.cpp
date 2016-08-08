#include "gram.decl.h"
#include "context.h"
#include "random_utils.h"

#include "gram.lex.h"
#include "gram.dcl.h"
#include "gram.stmt.h"
#include "gram.expr.h"

using namespace random_utils;

void generate_init_declarator_list(context &c)
{
    // init-declarator
    // init-declarator-list , init-declarator

    if (probably(c.rng, 0.4))
        generate_init_declarator(c);
    else
    {
        generate_init_declarator_list(c);
        std::cout << ", ";
        generate_init_declarator(c);
    }
}

void generate_init_declarator(context& c)
{
    // declarator initializer[opt]

    generate_declarator(c, false);

    if (probably(c.rng, 0.2))
    {
        std::cout << " ";
        generate_initializer(c);
    }
}

void generate_declarator(context& c, bool functions_only)
{
    // ptr-declarator
    // noptr-declarator parameters-and-qualifiers trailing-return-type

    generate_ptr_declarator(c, functions_only);
}

void generate_ptr_declarator(context& c, bool functions_only)
{
    // noptr-declarator
    // ptr-operator ptr-declarator

    if (probably(c.rng, 0.5))
        generate_noptr_declarator(c, functions_only, !functions_only);
    else
    {
        generate_ptr_operator(c);
        generate_ptr_declarator(c, functions_only);
    }
}

void generate_noptr_declarator(context& c, bool functions_only, bool allow_declarator_id)
{
    // declarator-id attribute-specifier-seqopt
    // noptr-declarator parameters-and-qualifiers
    // noptr-declarator [ constant-expressionopt ] attribute-specifier-seqopt
    // ( ptr-declarator )


    if (allow_declarator_id && probably(c.rng, 0.8))
    {
        generate_declarator_id(c);
    }
    else if (probably(c.rng, 0.3))
    {
        generate_noptr_declarator(c, functions_only, true);
        generate_parameters_and_qualifiers(c);
    }
    else if (probably(c.rng, 0.7))
    {
        generate_noptr_declarator(c, functions_only, !functions_only);
        std::cout << "[";
        generate_constant_expression(c);
        std::cout << "]";
    }
    else
    {
        std::cout << "(";
        generate_ptr_declarator(c, functions_only);
        std::cout << ")";
    }
}

void generate_parameters_and_qualifiers(context& c)
{
    // ( parameter-declaration-clause ) attribute-specifier-seq[opt] cv-qualifier-seq[opt] ref-qualifier[opt] exception-specification[opt]

    std::cout << "(";
    generate_parameter_declaration_clause(c);
    std::cout << ")";
}

void generate_ptr_operator(context& c)
{
    // * cv-qualifier-seqopt
    // &
    // ::opt nested-name-specifier * cv-qualifier-seqopt

    static char const* options[] = {"*", "&"};
    std::cout << take_random(c.rng, options);
}

void generate_declarator_id(context& c)
{
    // id-expression
    // ::opt nested-name-specifieropt class-name

    generate_identifier(c);
}

void generate_type_id(context& c)
{
    // type-specifier-seq abstract-declarator[opt]

    generate_decl_specifier_seq(c);
    std::cout << " ";
    if (probably(c.rng, .8))
        generate_abstract_declarator(c);
}

void generate_abstract_declarator(context& c)
{
    // ptr-abstract-declarator
    // noptr-abstract-declarator[opt] parameters-and-qualifiers trailing-return-type
    // abstract-pack-declarator

    generate_ptr_abstract_declarator(c);
}

void generate_ptr_abstract_declarator(context& c)
{
    // noptr-abstract-declarator
    // ptr-operator ptr-abstract-declarator[opt]

    if (probably(c.rng, 0.5))
        generate_noptr_abstract_declarator(c);
    else
    {
        generate_ptr_operator(c);
        if (probably(c.rng, 0.5))
            generate_ptr_abstract_declarator(c);
    }
}

void generate_noptr_abstract_declarator(context& c)
{
    // noptr-abstract-declarator[opt] parameters-and-qualifiers
    // noptr-abstract-declarator[opt] [ constant-expression[opt] ] attribute-specifier-seq[opt]
    // ( ptr-abstract-declarator )

    if (probably(c.rng, .5))
    {
        if (probably(c.rng, .3))
            generate_noptr_abstract_declarator(c);

        if (probably(c.rng, .3))
            generate_parameters_and_qualifiers(c);
        else
        {
            std::cout << "[";
            if (probably(c.rng, .4))
                generate_constant_expression(c);
            std::cout << "]";
        }
    }
    else
    {
        std::cout << "(";
        generate_ptr_abstract_declarator(c);
        std::cout << ")";
    }
}

void generate_parameter_declaration(context& c)
{
    // decl-specifier-seq declarator
    // decl-specifier-seq declarator = assignment-expression
    // decl-specifier-seq abstract-declarator[opt]
    // decl-specifier-seq abstract-declarator[opt] = assignment-expression

    generate_decl_specifier_seq(c);
    std::cout << " ";
    generate_declarator(c, false);

    if (probably(c.rng, 0.3))
    {
        std::cout << " = ";
        generate_identifier(c);
    }
}

void generate_parameter_declaration_list(context& c)
{
    // parameter-declaration
    // parameter-declaration-list , parameter-declaration

    if (probably(c.rng, 0.4))
        generate_parameter_declaration(c);
    else
    {
        generate_parameter_declaration_list(c);
        std::cout << ", ";
        generate_parameter_declaration(c);
    }
}

void generate_parameter_declaration_clause(context& c)
{
    // parameter-declaration-list[opt] ...[opt]
    // parameter-declaration-list , ...

    if (probably(c.rng, 0.95))
    {
        if (probably(c.rng, 0.9))
            generate_parameter_declaration_list(c);
        if (probably(c.rng, 0.07))
            std::cout << " ...";
    }
    else
    {
        generate_parameter_declaration_list(c);
        std::cout << ", ...";
    }
}

void generate_initializer(context& c)
{
    // = initializer-clause
    // ( expression-list )

    std::cout << "= ";
    generate_initializer_clause(c);
}

void generate_initializer_clause(context& c)
{
    // assignment-expression
    // braced-init-list

    generate_assignment_expression(c);
}

void generate_function_definition(context& c)
{
    // attribute-specifier-seq[opt] decl-specifier-seq[opt] declarator virt-specifier-seq[opt] function-body

    generate_decl_specifier_seq(c);
    std::cout << " ";
    generate_declarator(c, true);
    generate_function_body(c);
}

void generate_function_body(context& c)
{
    // ctor-initializer[opt] compound-statement
    // function-try-block
    // = default ;
    // = delete ;

    generate_compound_statement(c);
}
