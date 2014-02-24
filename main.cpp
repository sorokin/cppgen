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
    --c.tokens_left;

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

    if (c.tokens_left == 1)
    {
        generate_identifier(c);
        return;
    }

    if (rand_0n(c.rng, c.max_nesting_depth) < c.nesting_depth)
    {
        generate_identifier(c);
        return;
    }

    c.tokens_left -= 2;

    auto p = take_random(c.rng, parens);

    std::cout << p.first;
    ++c.nesting_depth;
    generate_some(c);
    --c.nesting_depth;
    std::cout << p.second;
}

void generate_braces(context& c)
{
    if (c.tokens_left == 1)
    {
        generate_identifier(c);
        return;
    }

    c.tokens_left -= 2;

    print_linefeed(c);
    std::cout << "{";
    ++c.nesting_depth;
    ++c.indent;

    print_linefeed(c);
    generate_some(c);

    --c.indent;
    --c.nesting_depth;

    print_linefeed(c);
    std::cout << "}";
    print_linefeed(c);
}

void generate_punctuator(context& c)
{
    static const char* const puncs[] = {",", ":", ";", "*", "=", "...", /*"#",*/ ".", "->", "++", "--", /*"##",*/
                                         "&", "+", "-", "~", "!", "/", "%", "<<", ">>", "!=",
                                         "<", ">", "<=", ">=", "==", "^", "|", "&&", "||", "?",
                                         "*=", "/=", "%=", "+=", "-=", "<<=", ">>=", "&=", "^=", "|=",
                                         "::", ".*", "->*"};

    --c.tokens_left;

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

    --c.tokens_left;

    std::cout << take_random(c.rng, kws) << " ";
}

void generate_integer_literal(context& c)
{
    static const char* const suffices[] = {"", "u", "l", "ul", "ll", "ull"};
    --c.tokens_left;

    std::cout << rand_0n(c.rng, 123) << take_random(c.rng, suffices) << " ";
}

void generate_float_literal(context& c)
{
    static const char* const suffices[] = {".f", "."};
    --c.tokens_left;

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
    unsigned n = rand_0n(c.rng, 8u);

    while (n != 0 && c.tokens_left != 0)
    {
        generate_once(c);
        --n;
    }

    print_linefeed(c);
}

void generate_all(context &c)
{
    while (c.tokens_left != 0)
    {
        generate_some(c);
    }
}

int main(int argc, char* argv[])
{
    try
    {
        size_t number_of_tokens;
        size_t max_nesting_depth;
        po::options_description desc("Allowed options");
        desc.add_options()
            ("help", "produce help message")
            ("number-of-tokens,n", po::value<size_t>(&number_of_tokens)->default_value(1000), "length of generated text in tokens")
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

        context ctx(number_of_tokens, max_nesting_depth, std::time(0));
        generate_all(ctx);
    }
    catch (std::exception const& e)
    {
        std::cout << "error: " << e.what() << std::endl;
    }

    return 0;
}

