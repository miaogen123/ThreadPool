// threadpools.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <windows.h>
#include"ThreadPool.h"
//#include<system>

using std::cout;
using std::string;
using std::endl;

void testF1(void) {
	static int count= 0;
	int countBackup = count;
	count++;
	int tmp = 10000;
	while (tmp >= 0) {
		tmp--;
	}
	cout << "in the testF" <<countBackup<< endl;
}

int testF2(int i) {
	int tmp = 10000;
	static int count = 10;
	while (tmp >= 0) {
		tmp--;
	}
	cout << "in the testF2: " << count+i<<endl;
	return count + i;
}

string hardOne(string a, string b, string c) {
	return a + b + c;
}

int main()
{
	ThreadPool threadPool;
	threadPool.submit<Task>(testF1);
	threadPool.submit<Task>(testF1);
	threadPool.submit<Task>(testF1);
	threadPool.submit<Task>(testF1);
	threadPool.submit<Task>(testF1);
	threadPool.submit<Task>(testF1);
	auto testF2_5_fu=threadPool.submit_future<>(testF2, 5);
	//using n_type = string(string, string, string);
	//这里面模板参数都不知道怎么写
	auto testF2_strs_fu=threadPool.submit_future<>(hardOne, "123", "hello", "world");
	cout << "get future: " << testF2_5_fu.get() <<endl;
	cout << "get future: " << testF2_strs_fu.get() <<endl;
	cout << "exit" << endl;
//	Sleep(1000);
	return 0;
}
