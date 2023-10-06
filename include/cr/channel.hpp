#pragma once
#include "sender.hpp"
#include "receiver.hpp"

namespace cr
{
    namespace internal
    {
        template <typename... T>
        consteval auto deduce();

        template <typename... T>
        using deduce_t = typename decltype(deduce<T...>())::type;
    } // namespace internal

    template <typename... T>
    struct recipe
    {
        using is_recipe = std::true_type;
        using sender    = sender<internal::deduce_t<T...>>;
        using receiver  = receiver<internal::deduce_t<T...>>;
    };

    template <typename... T>
    auto channel();
} // namespace cr

#include "channel.inl"