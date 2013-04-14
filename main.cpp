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
Int generate_0_n(Engine& e, Int limit)
{
    assert(limit != 0);

    Int i = boost::uniform_int<Int>(static_cast<Int>(0), limit - 1)(e);
    assert(i < limit);
    return i;
}

void generate_some(context& c);

void identifier_generator(context& c)
{
    static const char* const ids[] = {"foo", "bar", "baz", "qux"};

    --c.tokens_left;

    ptrdiff_t i = generate_0_n(c.rng, boost::size(ids));
    assert(i >= 0 && i < boost::size(ids));

    std::cout << " " << ids[i] << generate_0_n(c.rng, 16) << " ";
}

void paren_generator(context& c)
{
    static const char* const left[]  = {"(", "[", " < "};
    static const char* const right[] = {")", "]", " > "};

    if (c.tokens_left == 1)
    {
        identifier_generator(c);
        return;
    }

    if (generate_0_n(c.rng, static_cast<size_t>(c.max_nesting_depth)) < c.nesting_depth)
    {
        identifier_generator(c);
        return;
    }

    c.tokens_left -= 2;

    ptrdiff_t i = generate_0_n(c.rng, boost::size(left));
    assert(i >= 0 && i < boost::size(left));

    std::cout << left[i];
    ++c.nesting_depth;
    generate_some(c);
    --c.nesting_depth;
    std::cout << right[i];
}

void braces_generator(context& c)
{
    if (c.tokens_left == 1)
    {
        identifier_generator(c);
        return;
    }

    if (generate_0_n(c.rng, static_cast<size_t>(c.max_nesting_depth)) < c.nesting_depth)
    {
        identifier_generator(c);
        return;
    }

    c.tokens_left -= 2;

    std::cout << "\n";
    print_indent(c);
    std::cout << "{\n";
    ++c.nesting_depth;
    ++c.indent;

    generate_some(c);
    std::cout << "\n";

    --c.indent;
    --c.nesting_depth;

    print_indent(c);
    std::cout << "}\n";
    print_indent(c);
}

void punctuator_generator(context& c)
{
    static const char* const puncs[] = {",", ":", ";", "*", "=", "...", "#", ".", "->", "++", "--", "##",
                                         "&", "+", "-", "~", "!", "/", "%", "<<", ">>", "!=",
                                         "<", ">", "<=", ">=", "==", "^", "|", "&&", "||", "?",
                                         "*=", "/=", "%=", "+=", "-=", "<<=", ">>=", "&=", "^=", "|=",
                                         "::", ".*", "->*"};

    --c.tokens_left;

    ptrdiff_t i = generate_0_n(c.rng, boost::size(puncs));
    assert(i >= 0 && i < boost::size(puncs));

    std::cout << " " << puncs[i] << " ";
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

    ptrdiff_t i = generate_0_n(c.rng, boost::size(kws));
    assert(i >= 0 && i < boost::size(kws));

    std::cout << " " << kws[i] << " ";
}

void generate_once(context &c)
{
    static const double probabilities[] = {0.25, 0.03, 0.5, 0.2, 0.02};
    void (*gens[])(context&) = {&identifier_generator, &paren_generator, &punctuator_generator, &generate_keyword, &braces_generator};

    boost::random::discrete_distribution<ptrdiff_t> dist(probabilities);

    ptrdiff_t i = dist(c.rng);
    assert(i >= 0 && i < boost::size(gens));

    gens[i](c);
}

void generate_all(context &c)
{
    while (c.tokens_left != 0)
    {
        generate_once(c);
    }
}

void generate_some(context &c)
{
    unsigned n = boost::uniform_int<unsigned>(0, 7)(c.rng);

    while (n != 0 && c.tokens_left != 0)
        generate_once(c);
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

