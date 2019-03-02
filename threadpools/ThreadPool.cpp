#include "ThreadPool.h"
#include<Windows.h>
#include <iostream>


ThreadPool::ThreadPool(int threadN) :done(false), threadNum(threadN)
{
	remainThreadNum = threadN;
	try {
		for (int i = 0; i < threadNum; i++) {
			threadPool.emplace_back(&ThreadPool::worker_thread, this);
//			threadPool.back().detach();
		}
	}
	catch (...) {
		done = true;
		throw;
	}
}

void ThreadPool::worker_thread()
{
	while (!done) {
		std::unique_lock<std::mutex> wakeLock{ queueMutex };
		wakeCV.wait(wakeLock, [this](){ return !TaskList.empty()||done; });
		//std:: cout << "after wakeid:"<<std::this_thread::get_id()<<" "<<TaskList.size() ;
		if (done) {
			//std::cout <<"thread"<< std::this_thread::get_id() <<"exit "<< std::endl;
			break;
		}
		auto funcToExcu = TaskList.front();
		TaskList.pop();
		wakeLock.unlock();
		funcToExcu();
		//wakeLock.lock();
		//return;
	}
}

ThreadPool::~ThreadPool()
{
	done = true;
	wakeCV.notify_all();
	for (auto &th : threadPool) {
		if (th.joinable()) {
			th.join();
			std::cout << "join end" << std::endl;
			th.~thread();
		}
		else {
			th.~thread();
		}
	}
}

