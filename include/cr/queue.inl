#pragma once

#include "queue.hpp"

#include <cassert>

namespace cr
{
    template <typename T>
    T queue<T>::pop()
    {
        auto lock = std::unique_lock{m_mutex};

        assert(!(m_queue.empty() && senders == 0) && "No senders exist, pop will never terminate");
        m_cond.wait(lock, [this] { return !m_queue.empty(); });

        auto rtn = std::move(m_queue.front());
        m_queue.pop();

        return std::move(rtn);
    }

    template <typename T>
    std::optional<T> queue<T>::try_pop(std::chrono::milliseconds timeout)
    {
        auto lock = std::unique_lock{m_mutex};

        assert(!(m_queue.empty() && senders == 0) && "No senders exist, try_pop will timeout");
        const auto success = m_cond.wait_for(lock, timeout, [this] { return !m_queue.empty(); });

        if (!success)
        {
            return std::nullopt;
        }

        auto rtn = std::move(m_queue.front());
        m_queue.pop();

        return std::move(rtn);
    }

    template <typename T>
    template <typename... Args>
    void queue<T>::emplace(Args &&...args)
    {
        assert(!(receivers == 0) && "No receivers exist, message will never be read");
        {
            auto lock = std::lock_guard{m_mutex};
            m_queue.emplace(std::forward<Args>(args)...);
        }
        m_cond.notify_one();
    }
} // namespace cr
