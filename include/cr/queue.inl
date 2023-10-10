#pragma once
#include "queue.hpp"

#include <cassert>

namespace cr
{
    template <typename T>
    T queue<T>::pop()
    {
        std::unique_lock lock(m_mutex);

        assert(!(m_queue.empty() && senders == 0) && "No senders exist, pop will never terminate");
        m_cond.wait(lock, [this] { return !m_queue.empty(); });

        auto rtn = std::move(m_queue.front());
        m_queue.pop();

        return rtn;
    }

    template <typename T>
    std::optional<T> queue<T>::try_pop(std::chrono::milliseconds timeout)
    {
        std::unique_lock lock(m_mutex);
        m_cond.wait_for(lock, timeout, [this] { return !m_queue.empty(); });

        if (m_queue.empty())
        {
            return std::nullopt;
        }

        auto rtn = std::move(m_queue.front());
        m_queue.pop();
        return rtn;
    }

    template <typename T>
    template <typename... Args>
    void queue<T>::emplace(Args &&...args)
    {
        assert(!(receivers == 0) && "No receivers exist, message will never be read");
        {
            std::lock_guard lock(m_mutex);
            m_queue.emplace(std::forward<Args>(args)...);
        }
        m_cond.notify_one();
    }
} // namespace cr
