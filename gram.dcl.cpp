#include "gram.dcl.h"

#include <iostream>

#include "context.h"
#include "random_utils.h"

#include "gram.decl.h"
#include "gram.class.h"
#include "gram.lex.h"
#include "gram.expr.h"

using namespace random_utils;

void generate_declaration_seq(context& c)
{
    // declaration
    // declaration-seq declaration

    do
        generate_declaration(c);
    while (probably(c.rng, 0.8));
}

void generate_declaration(context &c)
{
    // block-declaration
    // function-definition
    // template-declaration
    // explicit-instantiation
    // explicit-specialization
    // linkage-specification
    // namespace-definition
    // empty-declaration
    // attribute-declaration

    static std::pair<double, void (*)(context&)> options[] = {{100., &generate_block_declaration},
                                                              {100., &generate_function_definition},
                                                              {20., &generate_namespace_definition}
                                                              };

    take_random_weighted(c.rng, options)(c);
}

void generate_block_declaration(context& c)
{
    // simple-declaration
    // asm-definition
    // namespace-alias-definition
    // using-declaration
    // using-directive
    // static_assert-declaration
    // alias-declaration
    // opaque-enum-declaration

    static std::pair<double, void (*)(context&)> options[] = {{100., &generate_simple_declaration}
                                                              };

    take_random_weighted(c.rng, options)(c);
}

void generate_simple_declaration(context& c)
{
    // decl-specifier-seqopt init-declarator-listopt ;
    // attribute-specifier-seq decl-specifier-seqopt init-declarator-list ;

    generate_decl_specifier_seq(c);
    std::cout << " ";
    if (probably(c.rng, .98))
        generate_init_declarator_list(c);
    std::cout << ";";
    print_linefeed(c);
}

void generate_decl_specifier_seq(context& c)
{
    static std::pair<double, char const*> options[] = {{.2, "register"},
                                                       {1., "static"},
                                                       {1., "extern"},
                                                       {1., "mutable"},
                                                       {1., "inline"},
                                                       {1., "virtual"},
                                                       {.5, "explicit"},
                                                       {1., "friend"},
                                                       {1., "typedef"}};

    if (probably(c.rng, 0.1))
    {
        std::cout << take_random_weighted(c.rng, options) << " ";

        if (probably(c.rng, 0.1))
        {
            std::cout << take_random_weighted(c.rng, options) << " ";
        }
    }

    if (probably(c.rng, 0.99))
    {
        generate_type_specifier(c);
    }

    if (probably(c.rng, 0.1))
    {
        std::cout << " " << take_random_weighted(c.rng, options);

        if (probably(c.rng, 0.1))
        {
            std::cout << " " << take_random_weighted(c.rng, options);
        }
    }
}

void generate_class_specifier(context& c)
{
    // class-head { member-specification[opt] }
    generate_class_head(c);
    open_brace(c);
    if (probably(c.rng, 0.9))
        generate_member_specification(c);
    close_brace(c);
}

void generate_type_specifier(context& c)
{
    // trailing-type-specifier
    // class-specifier
    // enum-specifier

    static std::pair<double, void (*)(context&)> options[] = {{1., &generate_trailing_type_specifier},
                                                              {.04, &generate_class_specifier},
                                                              {.04, &generate_enum_specifier}};
    take_random_weighted(c.rng, options)(c);
}

void generate_trailing_type_specifier(context& c)
{
    // simple-type-specifier
    // elaborated-type-specifier
    // typename-specifier
    // cv-qualifier

    static std::pair<double, void (*)(context&)> options[] = {{1., &generate_simple_type_specifier},
                                                              {.04, &generate_elaborated_type_specifier}};
                                                            // typename-specifier
                                                            // cv-qualifier
    take_random_weighted(c.rng, options)(c);

}

void generate_simple_type_specifier(context& c)
{
    // ::opt nested-name-specifieropt type-name
    // ::opt nested-name-specifier template template-id
    static std::pair<double, char const*> options[] = {{1., "char"},
                                                       {1., "wchar_t"},
                                                       {1., "bool"},
                                                       {1., "short"},
                                                       {1., "int"},
                                                       {1., "long"},
                                                       {1., "signed"},
                                                       {1., "unsigned"},
                                                       {1., "float"},
                                                       {1., "double"},
                                                       {1., "void"}};

    std::cout << take_random_weighted(c.rng, options);
}

void generate_elaborated_type_specifier(context& c)
{
    // class-key attribute-specifier-seq[opt] nested-name-specifier[opt] identifier
    // class-key nested-name-specifier[opt] template[opt] simple-template-id
    // enum nested-name-specifier[opt] identifier

    static std::pair<double, void (*)(context&)> options[] = {
        {1., [](context& c) {
            generate_class_key(c);
            std::cout << " ";
            generate_identifier(c);
        }},
        {1., [](context& c) {
            std::cout << "enum ";
            generate_identifier(c);
        }}};

    take_random_weighted(c.rng, options)(c);
}

void generate_enum_specifier(context &c)
{
    // enum-head { enumerator-list[opt] }
    // enum-head { enumerator-list , }

    generate_enum_head(c);
    open_brace(c);

    if (probably(c.rng, 0.60))
    {
        if (probably(c.rng, 0.99))
            generate_enumerator_list(c);
    }
    else
    {
        generate_enumerator_list(c);
    }

    close_brace(c);
}

void generate_enum_head(context &c)
{
    // enum-key attribute-specifier-seq[opt] identifier[opt] enum-base[opt]
    // enum-key attribute-specifier-seq[opt] nested-name-specifier identifier enum-base[opt]


    generate_enum_key(c);

    if (probably(c.rng, 0.95))
    {
        std::cout << " ";
        generate_identifier(c);
    }
}

void generate_enum_key(context &c)
{
    // enum
    // enum class
    // enum struct

    static std::pair<double, char const*> options[] = {{1., "enum"},
                                                       {1., "enum class"},
                                                       {1., "enum struct"}};

    std::cout << take_random_weighted(c.rng, options);
}

void generate_enumerator_list(context& c)
{
    // enumerator-definition
    // enumerator-list , enumerator-definition

    if (probably(c.rng, 0.4))
        generate_enumerator_definition(c);
    else
    {
        generate_enumerator_list(c);
        std::cout << ",";
        print_linefeed(c);
        generate_enumerator_definition(c);
    }
}

void generate_enumerator_definition(context &c)
{
    // enumerator
    // enumerator = constant-expression

    generate_enumerator(c);

    if (probably(c.rng, 0.4))
    {
        std::cout << " = ";
        generate_constant_expression(c);
    }
}

void generate_enumerator(context& c)
{
    // identifier

    generate_identifier(c);
}

void generate_namespace_definition(context& c)
{
    // named-namespace-definition
    // unnamed-namespace-definition

    // original-namespace-definition:
    //     inline[opt] namespace identifier { namespace-body }
    // extension-namespace-definition:
    //     inline[opt] namespace original-namespace-name { namespace-body }
    // original-namespace-name:
    //     identifier

    if (probably(c.rng, 0.4))
        std::cout << "inline ";

    std::cout << "namespace ";
    generate_identifier(c);
    open_brace(c);
    generate_namespace_body(c);
    close_brace(c);
}

void generate_namespace_body(context& c)
{
    // declaration-seq[opt]

    generate_declaration_seq(c);
}
