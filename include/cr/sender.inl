#pragma once
#include "sender.hpp"

namespace cr
{
    template <typename T> sender<T>::~sender()
    {
        if (!m_queue)
        {
            return;
        }

        m_queue->senders--;
    }

    template <typename T> sender<T>::sender(std::shared_ptr<queue<T>> queue) : m_queue(queue)
    {
        m_queue->senders++;
    }

    template <typename T> sender<T>::sender(const sender &) = default;

    template <typename T> sender<T>::sender(sender &&other) noexcept : m_queue(std::move(other.m_queue))
    {
        other.m_queue = nullptr;
    }

    template <typename T> void sender<T>::send(T &&message)
    {
        m_queue->emplace(std::forward<T>(message));
    }
} // namespace cr