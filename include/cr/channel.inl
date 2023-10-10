#pragma once
#include "channel.hpp"

namespace cr
{
    namespace internal
    {
        template <typename T>
        concept valid_recipe = requires(T) {
            requires not std::same_as<typename T::sender, void>;
            requires not std::same_as<typename T::receiver, void>;
            requires std::same_as<typename T::is_recipe, std::true_type>;
        };

        template <typename... T>
        consteval auto deduce()
        {
            constexpr auto extract_first = []<typename F, typename... O>(F, O...)
            {
                return std::type_identity<F>{};
            };

            using first = typename decltype(extract_first(std::declval<T>()...))::type;

            if constexpr (valid_recipe<first>)
            {
                return []<template <typename> typename sender, typename O>(sender<O> *)
                {
                    return std::type_identity<O>{};
                }(static_cast<first::sender *>(nullptr));
            }
            else if constexpr (sizeof...(T) > 1)
            {
                return std::type_identity<std::variant<T...>>{};
            }
            else
            {
                return std::type_identity<first>{};
            }
        }
    } // namespace internal

    template <typename... T>
    auto channel()
    {
        auto queue = std::make_shared<cr::queue<internal::deduce_t<T...>>>();
        return std::make_pair(cr::sender{queue}, cr::receiver{queue});
    }
} // namespace cr
