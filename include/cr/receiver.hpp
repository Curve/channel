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
        std::shared_ptr<queue<T>> m_queue;

      public:
        receiver(std::shared_ptr<queue<T>>);

      public:
        receiver(receiver &&) noexcept;

      public:
        receiver(const receiver &)            = delete;
        receiver &operator=(const receiver &) = delete;

      public:
        ~receiver();

      public:
        T recv();

      public:
        std::optional<T> try_recv();
        std::optional<T> recv_timeout(std::chrono::milliseconds);

      public:
        template <Visitable<T> Callback>
        void recv(Callback &&);

        template <Visitable<T> Callback>
        void try_recv(Callback &&);

        template <Visitable<T> Callback>
        void recv_timeout(Callback &&, std::chrono::milliseconds);

      public:
        template <ValueAccessible<T> O>
        O recv_as();

        template <ValueAccessible<T> O>
        std::optional<O> try_recv_as();

        template <ValueAccessible<T> O>
        std::optional<O> recv_timeout_as(std::chrono::milliseconds);
    };
} // namespace cr

#include "receiver.inl"
