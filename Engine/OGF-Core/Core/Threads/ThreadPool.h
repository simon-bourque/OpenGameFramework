#pragma once
#include "ThreadSafeQueue.h"

#include "Core/Singleton.h"
#include "Core/Types.h"

#include <thread>
#include <functional>
#include <future>

class ThreadPool
{
	SINGLETON_DECLARATION(ThreadPool)
public:
	ThreadPool();
	ThreadPool(const int numberOfThreads);
	~ThreadPool();

	// Submit work to the threadpool
	template<typename F, typename... Args>
	auto submit(F&& f, Args&&... args)->std::future<decltype(f(args...))>
	{
		std::function<decltype(f(args...))()> func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
		
		auto taskPtr = std::make_shared<std::packaged_task<decltype(f(args...))()>>(func);

		std::function<void()> voidFunc = [taskPtr]() {
			(*taskPtr)();
		};

		m_queue.enqueue(voidFunc);

		m_conditionalLock.notify_one();

		return taskPtr->get_future();
	}

private:
	class ThreadWorker
	{
	public:
		ThreadWorker(ThreadPool* threadPool, const int threadIdx);
		void operator()();

	private:
		int m_threadIdx;
		ThreadPool* m_threadPool;
	};

	constexpr static int32 DEFAULT_NUMBER_THREADS = 8;

	bool m_shutdown;
	std::condition_variable m_conditionalLock;
	std::mutex m_conditionalMutex;
	ThreadSafeQueue<std::function<void()>> m_queue;
	std::vector<std::thread> m_threads;
};

SINGLETON_ACCESSOR(ThreadPool)
