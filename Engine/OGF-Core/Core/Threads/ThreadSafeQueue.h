#pragma once

#include <mutex>
#include <queue>

template<typename T>
class ThreadSafeQueue
{
public:
	ThreadSafeQueue() = default;

	~ThreadSafeQueue() = default;

	bool empty()
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		return m_queue.empty();
	}

	int size()
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		return m_queue.size();
	}

	void enqueue(T& elem)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_queue.push(elem);
	}

	bool dequeue(T& elem) {
		std::unique_lock<std::mutex> lock(m_mutex);

		if (m_queue.empty()) {
			return false;
		}
		elem = std::move(m_queue.front());

		m_queue.pop();
		return true;
	}

private:
	std::mutex m_mutex;
	std::queue<T> m_queue;
};