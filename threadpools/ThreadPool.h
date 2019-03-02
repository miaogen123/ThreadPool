#pragma once
//TODO::这里的queue 应该改成thread_safe的 
#include<queue>
#include<future>
#include<memory>
#include<iostream>
#include<condition_variable>
#include<mutex>
#include<atomic>
#include<thread>
#include<functional>
	using Task= std::function<void(void)>;

class ThreadPool
{
	using Task= std::function<void(void)>;
public:
	ThreadPool(int threadN=4);
	//骚操作不建议工程使用，容易被打
	template<typename T, class... Args>
	auto submit_future(T&& f, Args&&... args)->std::future< typename std::result_of<T(Args...)>::type>;
	template<typename T>
	void submit(T f);
	void worker_thread();
	~ThreadPool();

private:
	const int threadNum;
	std::atomic_int remainThreadNum;
	std::vector<std::thread> threadPool;
	std::queue<Task> TaskList;
	std::atomic_bool done;
	std::mutex queueMutex;
	std::condition_variable wakeCV;
};

template<typename T, class ...Args>
inline auto ThreadPool::submit_future(T&& f, Args&& ...args) -> std::future<typename std::result_of<T(Args...)>::type>
{
	using return_type = typename std::result_of<T(Args...)>::type;
	auto task = std::make_shared<std::packaged_task<return_type()>>(
		std::bind(std::forward<T>(f) ,std::forward<Args>(args)...)
	);
	std::future<return_type> ret = task->get_future();
	{
		std::lock_guard<std::mutex> scope_mutex(queueMutex);
		TaskList.emplace([task]() {(*task)(); });
	}
	wakeCV.notify_one();
	return ret;
}

template<typename T>
inline void ThreadPool::submit(T f)
{
	{
		std::lock_guard<std::mutex> scope_mutex(queueMutex);
		TaskList.push(f);
	}
	wakeCV.notify_one();
	std::cout << "end submit" << std::endl;
}
