#include "gram.class.h"

#include <iostream>

#include "context.h"
#include "random_utils.h"

#include "gram.lex.h"
#include "gram.decl.h"
#include "gram.dcl.h"

using namespace random_utils;

void generate_class_key(context& c)
{
    static std::pair<double, char const*> options[] = {{1., "class"},
                                                       {10., "struct"},
                                                       {0.3, "union"}};

    std::cout << take_random_weighted(c.rng, options);
}

void generate_class_head(context& c)
{
    // class-key identifieropt base-clauseopt
    // class-key nested-name-specifier identifier base-clauseopt
    // class-key nested-name-specifieropt template-id base-clauseopt

    generate_class_key(c);
    if (probably(c.rng, 0.98))
    {
        std::cout << " ";
        generate_identifier(c);
    }
}

void generate_member_declarator(context& c)
{
    // declarator pure-specifier[opt]
    // declarator constant-initializer[opt]
    // identifier[opt] : constant-expression

    generate_declarator(c, false);
}

void generate_member_declarator_list(context& c)
{
    // member-declarator
    // member-declarator-list , member-declarator

    if (probably(c.rng, 0.5))
        generate_member_declarator(c);
    else
    {
        generate_member_declarator_list(c);
        std::cout << ", ";
        generate_member_declarator(c);
    }
}

void generate_member_declaration(context& c)
{
    // decl-specifier-seq[opt] member-declarator-list[opt] ;
    // function-definition ;opt
    // ::opt nested-name-specifier template opt unqualified-id ;
    // using-declaration
    // static_assert-declaration
    // template-declaration

    if (probably(c.rng, 0.98))
    {
        generate_decl_specifier_seq(c);
        std::cout << " ";
    }
    if (probably(c.rng, 0.98))
        generate_member_declarator_list(c);
    std::cout << ";";
    print_linefeed(c);
}

void generate_access_specifier(context& c)
{
    // private
    // protected
    // public

    static char const* options[] = {"private", "protected", "public"};
    std::cout << take_random(c.rng, options);
}

void generate_member_specification(context& c)
{
    // member-declaration member-specification[opt]
    // access-specifier : member-specification[opt]

    if (probably(c.rng, 0.8))
        generate_member_declaration(c);
    else
    {
        generate_access_specifier(c);
        std::cout << ":";
        print_linefeed(c);
    }

    if (probably(c.rng, 0.9))
        generate_member_specification(c);
}
