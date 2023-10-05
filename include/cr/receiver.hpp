#pragma once
#include "queue.hpp"

#include <memory>
#include <chrono>
#include <variant>
#include <optional>
#include <concepts>

namespace cr
{
    template <typename T, typename C>
    concept visitable = requires(T &t, C &c) { std::visit(c, t); };

    template <typename V, typename T>
    concept value_accessible = requires(V &v, T &t) { v = t, std::get<T>(v); };

    template <typename T>
    class receiver
    {
        std::shared_ptr<queue<T>> m_queue;

      public:
        ~receiver();

      public:
        receiver(std::shared_ptr<queue<T>>);

      public:
        receiver(receiver &&) noexcept;

      public:
        receiver(const receiver &)            = delete;
        receiver &operator=(const receiver &) = delete;

      public:
        T recv();
        std::optional<T> try_recv();
        std::optional<T> recv_timeout(std::chrono::milliseconds timeout);

      public:
        template <typename Callback>
            requires visitable<T, Callback>
        void recv(Callback &&);

      public:
        template <typename Callback>
            requires visitable<T, Callback>
        void try_recv(Callback &&);

      public:
        template <typename Callback>
            requires visitable<T, Callback>
        void recv_timeout(Callback &&, std::chrono::milliseconds timeout);

      public:
        template <typename O>
            requires value_accessible<T, O>
        O recv_as();

      public:
        template <typename O>
            requires value_accessible<T, O>
        std::optional<O> try_recv_as();

      public:
        template <typename O>
            requires value_accessible<T, O>
        std::optional<O> recv_timeout_as(std::chrono::milliseconds timeout);
    };
} // namespace cr

#include "receiver.inl"