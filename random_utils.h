#pragma once

#include <cassert>
#include <initializer_list>
#include <random>
#include <ranges>

namespace random_utils
{
    namespace impl
    {
        template <typename Range>
        auto take_random_weighted(std::uniform_random_bit_generator auto& e, Range const& list);

        template <typename Range>
        auto take_random(std::uniform_random_bit_generator auto& e, Range const& list);
    }

    template <typename Int>
    Int rand_0n(std::uniform_random_bit_generator auto& e, Int limit)
    {
        assert(limit != 0);

        Int i = std::uniform_int_distribution<Int>(static_cast<Int>(0), limit - 1)(e);
        assert(i < limit);
        return i;
    }

    template <typename Range>
    auto take_random_weighted(std::uniform_random_bit_generator auto& e, Range const& list)
    {
        return impl::take_random_weighted(e, list);
    }

    char const* take_random_weighted(std::uniform_random_bit_generator auto& e, std::initializer_list<std::pair<double, char const*> > list)
    {
        return impl::take_random_weighted(e, list);
    }

    template <typename Range>
    auto take_random(std::uniform_random_bit_generator auto& e, Range const& list)
    {
        return impl::take_random(e, list);
    }

    char const* take_random(std::uniform_random_bit_generator auto& e, std::initializer_list<char const*> list)
    {
        return impl::take_random(e, list);
    }

    bool probably(std::uniform_random_bit_generator auto& e, double probability)
    {
        std::uniform_real_distribution<double> dist;

        return dist(e) <= probability;
    }

    template <typename F>
    void a_few_times(std::uniform_random_bit_generator auto& e, unsigned limit, F const& f)
    {
        for (unsigned i = 0, n = rand_0n(e, limit); i != n; ++i)
            f();
    }

    namespace impl
    {
        template <typename Range>
        auto take_random_weighted(std::uniform_random_bit_generator auto& e, Range const& list)
        {
            static_assert(std::is_same_v<typename std::ranges::range_value_t<Range>::first_type, double>,
                          "probability must be a double");

            auto weights = list | std::ranges::views::keys;
            std::discrete_distribution<ptrdiff_t> dist(weights.begin(), weights.end());

            ptrdiff_t i = dist(e);
            assert(i >= 0 && i < std::size(list));
            return (std::begin(list) + i)->second;
        }

        template <typename Range>
        auto take_random(std::uniform_random_bit_generator auto& e, Range const& list)
        {
            ptrdiff_t i = rand_0n(e, std::size(list));
            assert(i >= 0 && i < std::size(list));
            return *(std::begin(list) + i);
        }
    }
}
