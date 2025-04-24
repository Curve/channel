#pragma once

#include "sender.hpp"
#include "receiver.hpp"

namespace cr
{
    namespace impl
    {
        template <typename... Ts>
        struct deduce;

        template <typename... Ts>
        using deduce_t = deduce<Ts...>::type;
    } // namespace impl

    template <typename... T>
    struct recipe
    {
        using sender   = cr::sender<impl::deduce_t<T...>>;
        using receiver = cr::receiver<impl::deduce_t<T...>>;

      public:
        static constexpr bool is_recipe = true;
    };

    template <typename... T>
    auto channel();
} // namespace cr

#include "channel.inl"
