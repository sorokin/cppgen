#include <iostream>

#include <boost/program_options.hpp>

#include "random_utils.h"
#include "context.h"

using namespace std;
using namespace random_utils;

namespace po = boost::program_options;

void generate_some(context& c);

void generate_identifier(context& c)
{
    const char* id = take_random_weighted(c.rng, {{20., "foo"}, {15., "bar"}, {7., "baz"}, {1., "qux"}, {0.1, "quux"},
                                                  {0.02, "corge"}, {0.02, "grault"}, {0.05, "garply"}, {0.05, "waldo"},
                                                  {0.02, "fred"}, {0.05, "plugh"}, {0.05, "xyzzy"}, {0.05, "thud"}});

    std::cout << id << rand_0n(c.rng, 7) << " ";
}

void generate_paren(context& c)
{
    static std::pair<const char*, const char*> const parens[]  = {{"(", ")"},
                                                                  {"[", "]"},
                                                                  {" < ", " > "}};

    if (rand_0n(c.rng, c.max_nesting_depth) < c.nesting_depth)
    {
        generate_identifier(c);
        return;
    }

    auto p = take_random(c.rng, parens);

    std::cout << p.first;
    ++c.nesting_depth;
    generate_some(c);
    --c.nesting_depth;
    std::cout << p.second;
}

void open_brace(context& c)
{
    print_linefeed(c);
    std::cout << "{";
    ++c.nesting_depth;
    ++c.indent;

    print_linefeed(c);
}

void close_brace(context& c)
{
    --c.indent;
    --c.nesting_depth;

    print_linefeed(c);
    std::cout << "}";
    print_linefeed(c);
}

void generate_braces(context& c)
{
    open_brace(c);
    generate_some(c);
    close_brace(c);
}

void generate_punctuator(context& c)
{
    static const char* const puncs[] = {",", ":", ";", "*", "=", "...", /*"#",*/ ".", "->", "++", "--", /*"##",*/
                                         "&", "+", "-", "~", "!", "/", "%", "<<", ">>", "!=",
                                         "<", ">", "<=", ">=", "==", "^", "|", "&&", "||", "?",
                                         "*=", "/=", "%=", "+=", "-=", "<<=", ">>=", "&=", "^=", "|=",
                                         "::", ".*", "->*"};

    std::cout << take_random(c.rng, puncs) << " ";
}

void generate_keyword(context& c)
{
    static const char* const kws[] = {"alignas", "alignof", "asm", "auto", "bool", "break", "case", "catch",
                                      "char", "char16_t", "char32_t", "class", "const", "constexpr", "const_cast",
                                      "continue", "decltype", "default", "delete", "do", "double", "dynamic_cast",
                                      "else", "enum", "explicit", "extern", "false", "float", "for", "friend",
                                      "goto", "if", "inline", "int", "long", "mutable", "namespace", "new",
                                      "noexcept", "nullptr", "operator", "private", "protected", "public",
                                      "register", "reinterpret_cast", "return", "short", "signed", "sizeof",
                                      "static", "static_assert", "static_cast", "struct", "switch", "template",
                                      "this", "thread_local", "throw", "true", "try", "typedef", "typeid",
                                      "typename", "union", "unsigned", "using", "virtual", "void", "volatile",
                                      "wchar_t", "while", "override", "final"};

    std::cout << take_random(c.rng, kws) << " ";
}

void generate_integer_literal(context& c)
{
    static const char* const suffices[] = {"", "u", "l", "ul", "ll", "ull"};

    std::cout << rand_0n(c.rng, 123) << take_random(c.rng, suffices) << " ";
}

void generate_float_literal(context& c)
{
    static const char* const suffices[] = {".f", "."};

    std::cout << rand_0n(c.rng, 123) << take_random(c.rng, suffices) << " ";
}

void generate_once(context &c)
{
    static std::pair<double, void (*)(context& c)> funcs[] = {{5., &generate_identifier}, {2., &generate_paren}, {10., &generate_punctuator},
                                                              {4., &generate_keyword}, {double(c.max_nesting_depth - c.nesting_depth) / 2., &generate_braces},
                                                              {2., &generate_integer_literal}, {2., &generate_float_literal}};
    take_random_weighted(c.rng, funcs)(c);
}

void generate_some(context &c)
{
    a_few_times(c.rng, 8u, [&]() { generate_once(c); });

    print_linefeed(c);
}

void generate_declarator(context& c);
void generate_decl_specifier_seq(context& c);

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

    generate_declarator(c);
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
    static std::pair<double, void (*)(context&)> options[] = {{1., &generate_simple_type_specifier},
                                                              {.04, &generate_class_specifier}};
                                                            // enum-specifier
                                                            // elaborated-type-specifier
                                                            // typename-specifier
                                                            // cv-qualifier
    take_random_weighted(c.rng, options)(c);
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

void generate_parameter_declaration(context& c)
{
    // decl-specifier-seq declarator
    // decl-specifier-seq declarator = assignment-expression
    // decl-specifier-seq abstract-declarator[opt]
    // decl-specifier-seq abstract-declarator[opt] = assignment-expression

    generate_decl_specifier_seq(c);
    std::cout << " ";
    generate_declarator(c);

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

void generate_direct_declarator(context& c)
{
    // declarator-id
    // direct-declarator ( parameter-declaration-clause ) cv-qualifier-seqopt exception-specificationopt
    // direct-declarator [ constant-expressionopt ]
    // ( declarator )

    static std::pair<double, void (*)(context&)> options[] = {{10., &generate_declarator_id},
                                                              {1., [](context& c) {
                                                                  generate_direct_declarator(c);
                                                                  std::cout << "(";
                                                                  generate_parameter_declaration_clause(c);
                                                                  std::cout << ")";
                                                              }},
                                                              {1., [](context& c) {
                                                                  generate_direct_declarator(c);
                                                                  std::cout << "[";
                                                                  generate_integer_literal(c);
                                                                  std::cout << "]";
                                                              }},
                                                              {.5, [](context& c) {
                                                                  std::cout << "(";
                                                                  generate_declarator(c);
                                                                  std::cout << ")";
                                                              } }};

    take_random_weighted(c.rng, options)(c);
}

void generate_declarator(context& c)
{
    if (probably(c.rng, 0.5))
        generate_direct_declarator(c);
    else
    {
        generate_ptr_operator(c);
        generate_declarator(c);
    }
}

void generate_initializer(context& c)
{
    // = initializer-clause
    // ( expression-list )

    std::cout << "= ";
    generate_identifier(c);
}

void generate_init_declarator(context& c)
{
    // declarator initializer[opt]

    generate_declarator(c);

    if (probably(c.rng, 0.2))
    {
        std::cout << " ";
        generate_initializer(c);
    }
}

void generate_simple_declaration(context& c)
{
    generate_decl_specifier_seq(c);
    std::cout << " ";
    if (probably(c.rng, .98))
    {
        generate_init_declarator(c);

        a_few_times(c.rng, 4u, [&]() {
            std::cout << ", ";
            generate_init_declarator(c);
        });
    }
    std::cout << ";";
    print_linefeed(c);
}

void generate_block_declaration(context& c)
{
    static std::pair<double, void (*)(context&)> options[] = {{100., &generate_simple_declaration},
                                                            // asm-definition
                                                            // namespace-alias-definition
                                                            // using-declaration
                                                            // using-directive
                                                            // static_assert-declaration
                                                              {.5, &generate_some}};

    take_random_weighted(c.rng, options)(c);
}

void generate_declaration(context &c)
{
    static std::pair<double, void (*)(context&)> options[] = {{100., &generate_block_declaration},
                                                            // function-definition
                                                            // template-declaration
                                                            // explicit-instantiation
                                                            // explicit-specialization
                                                            // linkage-specification
                                                            // namespace-definition
                                                              {.5, &generate_some}};

    take_random_weighted(c.rng, options)(c);
}

void generate_file(context &c, size_t number_of_top_level_declarations)
{
    for (size_t i = 0; i != number_of_top_level_declarations; ++i)
        generate_declaration(c);
}

int main(int argc, char* argv[])
{
    try
    {
        size_t number_of_top_level_declarations;
        size_t max_nesting_depth;
        po::options_description desc("Allowed options");
        desc.add_options()
            ("help", "produce help message")
            ("number-of-top-level-declarations,n", po::value<size_t>(&number_of_top_level_declarations)->default_value(1000), "number of top-level declarations")
            ("max-nesting-depth,d", po::value<size_t>(&max_nesting_depth)->default_value(10), "maximum depth of nesting");

        po::variables_map vm;
        po::store(po::command_line_parser(argc, argv).options(desc).run(), vm);
        po::notify(vm);

        if (vm.count("help"))
        {
            cout << "Usage: " << " [options]\n";
            cout << desc;
            return 0;
        }

        context ctx(max_nesting_depth, std::time(0));
        generate_file(ctx, number_of_top_level_declarations);
    }
    catch (std::exception const& e)
    {
        std::cout << "error: " << e.what() << std::endl;
    }

    return 0;
}

