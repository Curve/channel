#pragma once
#include "channel.hpp"

namespace cr
{
    namespace internal
    {
        template <typename T>
        concept is_recipe = requires(T &t) { []<typename... O>(recipe<O...> &) {}(t); };

        template <typename... T> consteval auto deduce()
        {
            constexpr auto extract_first = []<typename F, typename... O>(F, O...) { return std::type_identity<F>{}; };
            using first_t = typename decltype(extract_first(std::declval<T>()...))::type;

            if constexpr (sizeof...(T) > 1)
            {
                return std::type_identity<std::variant<T...>>{};
            }
            else if constexpr (is_recipe<first_t>)
            {
                using sender_t = typename first_t::sender_t;
                return []<typename O>(sender<O> *) { return std::type_identity<O>{}; }(static_cast<sender_t *>(nullptr));
            }
            else
            {
                return std::type_identity<first_t>{};
            }
        }
    } // namespace internal

    template <typename... T> auto channel()
    {
        auto queue = std::make_shared<cr::queue<internal::deduce_t<T...>>>();
        return std::make_pair(cr::sender(queue), cr::receiver(queue));
    }
} // namespace cr