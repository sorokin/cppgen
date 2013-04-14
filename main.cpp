#include <iostream>
#include <boost/program_options.hpp>
#include <boost/random.hpp>
#include <boost/range/size.hpp>

using namespace std;

namespace po = boost::program_options;

struct context
{
    context(size_t n, size_t, int seed);

    size_t tokens_left;
    size_t max_nesting_depth;
    size_t nesting_depth;
    size_t indent;

    boost::mt19937 rng;
};

context::context(size_t n, size_t max_nesting_depth, int seed)
    : tokens_left(n)
    , max_nesting_depth(max_nesting_depth)
    , nesting_depth()
    , indent()
    , rng(seed)
{}

void print_indent(context const& c)
{
    std::cout << std::string(c.indent * 4, ' ');
}

template <typename Engine, typename Int>
Int rand_0n(Engine& e, Int limit)
{
    assert(limit != 0);

    Int i = boost::uniform_int<Int>(static_cast<Int>(0), limit - 1)(e);
    assert(i < limit);
    return i;
}

void generate_some(context& c);

void generate_identifier(context& c)
{
    const double weights[] = {20., 15., 7., 1., 0.1, 0.02,
                              0.02, 0.05, 0.05, 0.02, 0.05,
                              0.05, 0.05};
    static const char* const ids[] = {"foo", "bar", "baz", "qux", "quux", "corge",
                                      "grault", "garply", "waldo", "fred", "plugh",
                                      "xyzzy", "thud"};

    --c.tokens_left;

    boost::random::discrete_distribution<ptrdiff_t> dist(weights);

    ptrdiff_t i = dist(c.rng);
    assert(i >= 0 && i < boost::size(ids));

    std::cout << ids[i] << rand_0n(c.rng, 7) << " ";
}

void generate_paren(context& c)
{
    static const char* const left[]  = {"(", "[", " < "};
    static const char* const right[] = {")", "]", " > "};

    if (c.tokens_left == 1)
    {
        generate_identifier(c);
        return;
    }

    if (rand_0n(c.rng, static_cast<size_t>(c.max_nesting_depth)) < c.nesting_depth)
    {
        generate_identifier(c);
        return;
    }

    c.tokens_left -= 2;

    ptrdiff_t i = rand_0n(c.rng, boost::size(left));
    assert(i >= 0 && i < boost::size(left));

    std::cout << left[i];
    ++c.nesting_depth;
    generate_some(c);
    --c.nesting_depth;
    std::cout << right[i];
}

void generate_braces(context& c)
{
    if (c.tokens_left == 1)
    {
        generate_identifier(c);
        return;
    }

    c.tokens_left -= 2;

    std::cout << "\n";
    print_indent(c);
    std::cout << "{\n";
    ++c.nesting_depth;
    ++c.indent;

    print_indent(c);
    generate_some(c);
    std::cout << "\n";

    --c.indent;
    --c.nesting_depth;

    print_indent(c);
    std::cout << "}\n";
    print_indent(c);
}

void generate_punctuator(context& c)
{
    static const char* const puncs[] = {",", ":", ";", "*", "=", "...", /*"#",*/ ".", "->", "++", "--", /*"##",*/
                                         "&", "+", "-", "~", "!", "/", "%", "<<", ">>", "!=",
                                         "<", ">", "<=", ">=", "==", "^", "|", "&&", "||", "?",
                                         "*=", "/=", "%=", "+=", "-=", "<<=", ">>=", "&=", "^=", "|=",
                                         "::", ".*", "->*"};

    --c.tokens_left;

    ptrdiff_t i = rand_0n(c.rng, boost::size(puncs));
    assert(i >= 0 && i < boost::size(puncs));

    std::cout << puncs[i] << " ";
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

    ptrdiff_t i = rand_0n(c.rng, boost::size(kws));
    assert(i >= 0 && i < boost::size(kws));

    std::cout << kws[i] << " ";
}

void generate_integer_literal(context& c)
{
    static const char* const suffixes[] = {"", "u", "l", "ul", "ll", "ull"};
    --c.tokens_left;

    std::cout << rand_0n(c.rng, 123) << suffixes[rand_0n(c.rng, boost::size(suffixes))] << " ";
}

void generate_float_literal(context& c)
{
    static const char* const suffixes[] = {".f", "."};
    --c.tokens_left;

    std::cout << rand_0n(c.rng, 123) << suffixes[rand_0n(c.rng, boost::size(suffixes))] << " ";
}

void generate_once(context &c)
{
    const double weights[] = {5., 2., 10.,
                              4., double(c.max_nesting_depth - c.nesting_depth) / 2., 2.,
                              2.};
    void (*gens[])(context&) = {&generate_identifier, &generate_paren, &generate_punctuator,
                                &generate_keyword, &generate_braces, &generate_integer_literal,
                                &generate_float_literal};

    boost::random::discrete_distribution<ptrdiff_t> dist(weights);

    ptrdiff_t i = dist(c.rng);
    assert(i >= 0 && i < boost::size(gens));

    gens[i](c);
}

void generate_some(context &c)
{
    unsigned n = boost::uniform_int<unsigned>(0, 7)(c.rng);

    while (n != 0 && c.tokens_left != 0)
    {
        generate_once(c);
        --n;
    }

    std::cout << "\n";
    print_indent(c);
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

