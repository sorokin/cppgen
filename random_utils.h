#ifndef RANDOM_UTILS_H
#define RANDOM_UTILS_H

#include <initializer_list>

#include <boost/random.hpp>
#include <boost/range/value_type.hpp>
#include <boost/range/size.hpp>
#include <boost/range/adaptors.hpp>

namespace random_utils
{
    namespace impl
    {
        template <typename Range, typename Engine>
        auto take_random_weighted(Engine& e, Range const& list);

        template <typename Range, typename Engine>
        auto take_random(Engine& e, Range const& list);
    }

    template <typename Engine, typename Int>
    Int rand_0n(Engine& e, Int limit)
    {
        assert(limit != 0);

        Int i = boost::uniform_int<Int>(static_cast<Int>(0), limit - 1)(e);
        assert(i < limit);
        return i;
    }

    template <typename Engine, typename Range>
    auto take_random_weighted(Engine& e, Range const& list)
    {
        return impl::take_random_weighted(e, list);
    }

    template <typename Engine>
    char const* take_random_weighted(Engine& e, std::initializer_list<std::pair<double, char const*> > list)
    {
        return impl::take_random_weighted(e, list);
    }

    template <typename Range, typename Engine>
    auto take_random(Engine& e, Range const& list)
    {
        return impl::take_random(e, list);
    }

    template <typename Engine>
    char const* take_random(Engine& e, std::initializer_list<char const*> list)
    {
        return impl::take_random(e, list);
    }

    template <typename Engine>
    bool probably(Engine& e, double probability)
    {
        boost::random::uniform_real_distribution<double> dist;

        return dist(e) <= probability;
    }

    template <typename Engine, typename F>
    void a_few_times(Engine& e, unsigned limit, F const& f)
    {
        for (unsigned i = 0, n = rand_0n(e, limit); i != n; ++i)
            f();
    }

    namespace impl
    {
        template <typename Range, typename Engine>
        auto take_random_weighted(Engine& e, Range const& list)
        {
            static_assert(std::is_same_v<typename boost::range_value<Range>::type::first_type, double>,
                          "probability must be a double");

            using namespace boost::adaptors;

            boost::random::discrete_distribution<ptrdiff_t> dist(list | map_keys);

            ptrdiff_t i = dist(e);
            assert(i >= 0 && i < boost::size(list));
            return (std::begin(list) + i)->second;
        }

        template <typename Range, typename Engine>
        auto take_random(Engine& e, Range const& list)
        {
            using namespace boost::adaptors;

            ptrdiff_t i = rand_0n(e, boost::size(list));
            assert(i >= 0 && i < boost::size(list));
            return *(std::begin(list) + i);
        }
    }
}

#endif // RANDOM_UTILS_H
