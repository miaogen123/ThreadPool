#pragma once
//TODO::这里的queue 应该改成thread_safe的 
#include<queue>
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

template<typename T>
inline void ThreadPool::submit(T f)
{
	TaskList.push(f);
	wakeCV.notify_one();
	std::cout << "end submit" << std::endl;
}
