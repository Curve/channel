#pragma once
#include <queue>
#include <memory>
#include <variant>
#include <condition_variable>

#include "utils/annotations.hpp"
namespace cr
{
    template <typename... Messages> class sender
    {
        using variant_t = std::variant<Messages...>;

      private:
        std::shared_ptr<std::mutex> m_mutex;
        std::shared_ptr<std::queue<variant_t>> m_queue;
        std::shared_ptr<std::condition_variable> m_cond;

      public:
        sender(const sender &);
        sender(sender &&) noexcept;
        sender(decltype(m_mutex), decltype(m_queue), decltype(m_cond));

      public:
        template <typename T> //
        [[thread_safe]] void send(T && = {});
    };

    template <typename... Messages> class receiver
    {
        using variant_t = std::variant<Messages...>;

      private:
        std::shared_ptr<std::mutex> m_mutex;
        std::shared_ptr<std::queue<variant_t>> m_queue;
        std::shared_ptr<std::condition_variable> m_cond;

      public:
        receiver(receiver &&) noexcept;
        receiver(const receiver &) = delete;
        receiver(decltype(m_mutex), decltype(m_queue), decltype(m_cond));

      public:
        template <typename T> //
        [[blocking]] [[thread_safe]] T receive_as();

        template <typename Callback> //
        [[blocking]] [[thread_safe]] void receive(Callback &&);
    };

    template <typename... Messages> std::pair<sender<Messages...>, receiver<Messages...>> channel();
} // namespace cr
#include "utils/annotations.hpp"

#include "channel.inl"