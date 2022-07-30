#pragma once
#include "channel.hpp"

namespace cr
{
    template <typename... Messages> //
    sender<Messages...>::sender(const sender &other) = default;

    template <typename... Messages> //
    sender<Messages...>::sender(sender &&other) noexcept = default;

    template <typename... Messages> //
    sender<Messages...>::sender(decltype(m_mutex) mutex, decltype(m_queue) queue, decltype(m_cond) cond)
        : m_mutex(std::move(mutex)), m_queue(std::move(queue)), m_cond(std::move(cond))
    {
    }

    template <typename... Messages> //
    template <typename T>
    void sender<Messages...>::send(T &&message)
    {
        {
            std::lock_guard guard(*m_mutex);
            m_queue->emplace(std::forward<T>(message));
        }
        m_cond->notify_one();
    }

    template <typename... Messages> //
    receiver<Messages...>::receiver(receiver &&other) noexcept = default;

    template <typename... Messages> //
    receiver<Messages...>::receiver(decltype(m_mutex) mutex, decltype(m_queue) queue, decltype(m_cond) cond)
        : m_mutex(std::move(mutex)), m_queue(std::move(queue)), m_cond(std::move(cond))
    {
    }

    template <typename... Messages> //
    template <typename T>
    T receiver<Messages...>::receive_as()
    {
        std::unique_lock lock(*m_mutex);
        m_cond->wait(lock, [this] { return !m_queue->empty(); });

        auto entry = std::move(m_queue->front());
        m_queue->pop();

        return std::get<T>(entry);
    }

    template <typename... Messages> //
    template <typename Callback>
    void receiver<Messages...>::receive(Callback &&callback)
    {
        std::unique_lock lock(*m_mutex);
        m_cond->wait(lock, [this] { return !m_queue->empty(); });

        auto entry = std::move(m_queue->front());
        m_queue->pop();

        std::visit(std::forward<Callback>(callback), std::move(entry));
    }

    template <typename... Messages> std::pair<sender<Messages...>, receiver<Messages...>> channel()
    {
        auto mutex = std::make_shared<std::mutex>();
        auto queue = std::make_shared<std::queue<std::variant<Messages...>>>();
        auto cond = std::make_shared<std::condition_variable>();

        return {sender<Messages...>{mutex, queue, cond}, receiver<Messages...>{mutex, queue, cond}};
    }
} // namespace cr
