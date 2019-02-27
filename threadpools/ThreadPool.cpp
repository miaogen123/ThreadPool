#include "ThreadPool.h"
#include<Windows.h>
#include <iostream>


ThreadPool::ThreadPool(int threadN) :done(false), threadNum(threadN)
{
	remainThreadNum = threadN;
	try {
		for (int i = 0; i < threadNum; i++) {
			threadPool.emplace_back(&ThreadPool::worker_thread, this);
			threadPool.back().detach();
		}
	}
	catch (...) {
		done = true;
		throw;
	}
}

void ThreadPool::worker_thread()
{
	std::unique_lock<std::mutex> wakeLock{ queueMutex };
	while (!done) {
		wakeCV.wait(wakeLock, [this](){ return !TaskList.empty() ; });
		//std:: cout << "after wakeid:"<<std::this_thread::get_id()<<" "<<TaskList.size() ;
		auto funcToExcu = TaskList.front();
		TaskList.pop();
		wakeLock.unlock();
		funcToExcu();
		wakeLock.lock();
		//return;
	}
}

ThreadPool::~ThreadPool()
{
	done = true;
	for (auto &th : threadPool) {
		th.~thread();
	}
	//Sleep(10000);
}

