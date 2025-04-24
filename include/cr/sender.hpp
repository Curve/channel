#pragma once

#include "queue.hpp"

#include <memory>

namespace cr
{
    template <typename T>
    class sender
    {
        std::shared_ptr<queue<T>> m_queue;

      public:
        sender(std::shared_ptr<queue<T>>);

      public:
        sender(const sender &);
        sender(sender &&) noexcept;

      public:
        ~sender();

      public:
        void send(T = {});
    };
} // namespace cr

#include "sender.inl"
