#pragma once
#include <queue>
#include <mutex>
#include <chrono>
#include <atomic>
#include <optional>
#include <condition_variable>

namespace cr
{
    template <typename T>
    class queue
    {
        template <typename>
        friend class sender;

        template <typename>
        friend class receiver;

      private:
        std::queue<T> m_queue;

      private:
        std::mutex m_mutex;
        std::condition_variable m_cond;

      protected:
        std::atomic_size_t senders;
        std::atomic_size_t receivers;

      public:
        T pop();

      public:
        std::optional<T> try_pop(std::chrono::milliseconds);

      public:
        template <typename... Args>
        void emplace(Args &&...);
    };
} // namespace cr

#include "queue.inl"