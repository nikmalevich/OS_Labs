#include "average_thread_param.h"
#include <Windows.h>
#include <iostream>

using namespace std;

DWORD WINAPI average(LPVOID thread_param) {
	average_thread_param* param = (average_thread_param*)thread_param;

	int sum = 0;

	for (int i = 0; i < param->num_elem; i++) {
		sum += param->array[i];
		Sleep(12);
	}

	param->average = sum / param->num_elem;

	cout << "Average: " << param->average << endl;

	return 0;
}