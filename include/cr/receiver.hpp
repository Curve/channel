#pragma once

#include "queue.hpp"

#include <memory>
#include <chrono>

#include <variant>
#include <optional>

namespace cr
{
    template <typename C, typename T>
    concept Visitable = requires(T &t, C &c) { std::visit(c, t); };

    template <typename T, typename V>
    concept ValueAccessible = requires(V &v, T &t) { v = t, std::get<T>(v); };

    template <typename T>
    class receiver
    {
        using duration = std::chrono::milliseconds;

      private:
        std::shared_ptr<queue<T>> m_queue;

      public:
        receiver(std::shared_ptr<queue<T>>);

      public:
        receiver(receiver &&) noexcept            = default;
        receiver &operator=(receiver &&) noexcept = default;

      public:
        receiver(const receiver &)            = delete;
        receiver &operator=(const receiver &) = delete;

      public:
        ~receiver();

      public:
        res<T> recv();
        res<std::optional<T>> try_recv(duration = duration{0});

      public:
        template <Visitable<T> Callback>
        res<void> recv(Callback &&);

        template <Visitable<T> Callback>
        res<bool> try_recv(Callback &&, duration = duration{0});

      public:
        template <ValueAccessible<T> O>
        res<O> recv_as();

        template <ValueAccessible<T> O>
        res<std::optional<O>> try_recv_as(duration = duration{0});
    };
} // namespace cr

#include "receiver.inl"
