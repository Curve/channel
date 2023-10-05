#pragma once
#include "receiver.hpp"

namespace cr
{
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
    receiver<T>::receiver(std::shared_ptr<queue<T>> queue) : m_queue(queue)
    {
        m_queue->receivers++;
    }

    template <typename T>
    receiver<T>::receiver(receiver &&other) noexcept : m_queue(std::move(other.m_queue))
    {
        other.m_queue = nullptr;
    }

    template <typename T>
    T receiver<T>::recv()
    {
        return m_queue->pop();
    }

    template <typename T>
    std::optional<T> receiver<T>::try_recv()
    {
        return m_queue->try_pop(std::chrono::milliseconds(0));
    }

    template <typename T>
    std::optional<T> receiver<T>::recv_timeout(std::chrono::milliseconds timeout)
    {
        return m_queue->try_pop(timeout);
    }

    template <typename T>
    template <typename Callback>
        requires visitable<T, Callback>
    void receiver<T>::recv(Callback &&callback)
    {
        std::visit(std::forward<Callback>(callback), m_queue->pop());
    }

    template <typename T>
    template <typename Callback>
        requires visitable<T, Callback>
    void receiver<T>::try_recv(Callback &&callback)
    {
        auto rtn = m_queue->try_pop(std::chrono::milliseconds(0));

        if (!rtn.has_value())
        {
            return;
        }

        std::visit(std::forward<Callback>(callback), rtn.value());
    }

    template <typename T>
    template <typename Callback>
        requires visitable<T, Callback>
    void receiver<T>::recv_timeout(Callback &&callback, std::chrono::milliseconds timeout)
    {
        auto rtn = m_queue->try_pop(timeout);

        if (!rtn.has_value())
        {
            return;
        }

        std::visit(std::forward<Callback>(callback), rtn.value());
    }

    template <typename T>
    template <typename O>
        requires value_accessible<T, O>
    O receiver<T>::recv_as()
    {
        return std::get<O>(m_queue->pop());
    }

    template <typename T>
    template <typename O>
        requires value_accessible<T, O>
    std::optional<O> receiver<T>::try_recv_as()
    {
        auto rtn = m_queue->try_pop(std::chrono::milliseconds(0));

        if (!rtn.has_value())
        {
            return std::nullopt;
        }

        return std::get<O>(rtn.value());
    }

    template <typename T>
    template <typename O>
        requires value_accessible<T, O>
    std::optional<O> receiver<T>::recv_timeout_as(std::chrono::milliseconds timeout)
    {
        auto rtn = m_queue->try_pop(timeout);

        if (!rtn.has_value())
        {
            return std::nullopt;
        }

        return std::get<O>(rtn.value());
    }
} // namespace cr