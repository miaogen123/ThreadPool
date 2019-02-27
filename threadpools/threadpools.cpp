// threadpools.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <windows.h>
#include"ThreadPool.h"
//#include<system>

using std::cout;
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

void testF2(void) {
	int tmp = 10000;
	while (tmp >= 0) {
		tmp--;
	}
	cout << "in the testF2" << endl;
}

int main()
{
	ThreadPool threadPool;
	threadPool.submit<Task>(testF1);
	threadPool.submit<Task>(testF2);
	threadPool.submit<Task>(testF1);
	threadPool.submit<Task>(testF1);
	threadPool.submit<Task>(testF1);
	cout << "exit" << endl;
	Sleep(1000);
	return 0;
}
