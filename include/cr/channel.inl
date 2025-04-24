#pragma once

#include "channel.hpp"

namespace cr
{
    namespace impl
    {
        template <typename T>
        concept ValidRecipe = requires() {
            requires T::is_recipe;
            requires not std::same_as<typename T::sender, void>;
            requires not std::same_as<typename T::receiver, void>;
        };

        template <typename... Ts>
        struct deduce
        {
            using type = std::variant<Ts...>;
        };

        template <typename T>
        struct deduce<T>
        {
            using type = T;
        };

        template <template <typename> typename Sender, typename T>
            requires std::is_base_of_v<sender<T>, Sender<T>>
        struct deduce<Sender<T>>
        {
            using type = T;
        };

        template <ValidRecipe T>
        struct deduce<T> : deduce<typename T::sender>
        {
        };
    } // namespace impl

    template <typename... T>
    auto channel()
    {
        auto queue = std::make_shared<cr::queue<impl::deduce_t<T...>>>();
        return std::make_pair(cr::sender{queue}, cr::receiver{queue});
    }
} // namespace cr
