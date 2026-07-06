#pragma once

#include "receiver.hpp"

namespace cr
{
    template <typename T>
    receiver<T>::receiver(std::shared_ptr<queue<T>> queue) : m_queue(queue)
    {
        m_queue->receivers++;
    }

    template <typename T>
    receiver<T>::~receiver()
    {
        if (!m_queue)
        {
            return;
        }

        m_queue->receivers--;
    }

    template <typename T>
    res<T> receiver<T>::recv()
    {
        return m_queue->pop();
    }

    template <typename T>
    res<std::optional<T>> receiver<T>::try_recv(std::chrono::milliseconds timeout)
    {
        return m_queue->try_pop(timeout);
    }

    template <typename T>
    template <Visitable<T> Callback>
    res<void> receiver<T>::recv(Callback &&callback)
    {
        auto [value, remaining] = m_queue->pop();
        std::visit(std::forward<Callback>(callback), std::move(value));

        return {
            .remaining = remaining,
        };
    }

    template <typename T>
    template <Visitable<T> Callback>
    res<bool> receiver<T>::try_recv(Callback &&callback, duration timeout)
    {
        auto [value, remaining] = m_queue->try_pop(timeout);

        if (!value.has_value())
        {
            return {
                .value     = false,
                .remaining = {},
            };
        }

        std::visit(std::forward<Callback>(callback), std::move(*value));

        return {
            .value     = true,
            .remaining = remaining,
        };
    }

    template <typename T>
    template <ValueAccessible<T> O>
    res<O> receiver<T>::recv_as()
    {
        auto [value, remaining] = m_queue->pop();

        return {
            .value     = std::get<O>(std::move(value)),
            .remaining = remaining,
        };
    }

    template <typename T>
    template <ValueAccessible<T> O>
    res<std::optional<O>> receiver<T>::try_recv_as(duration timeout)
    {
        auto [value, remaining] = m_queue->try_pop(timeout);

        if (!value.has_value())
        {
            return {
                .value     = std::nullopt,
                .remaining = {},
            };
        }

        return {
            .value     = std::get<O>(std::move(*value)),
            .remaining = remaining,
        };
    }
} // namespace cr
