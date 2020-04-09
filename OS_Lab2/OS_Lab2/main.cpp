#include "min_max_thread_param.h"
#include "average_thread_param.h"
#include <Windows.h>
#include <iostream>

using namespace std;

DWORD WINAPI min_max(LPVOID);
DWORD WINAPI average(LPVOID);

int main()
{
	int num_elem;

	cout << "Enter number of elements: ";
	cin >> num_elem;

	int* arr = new int[num_elem];
	
	cout << "Enter elements: ";
	for (int i = 0; i < num_elem; i++)
		cin >> arr[i];

	HANDLE	hMinMaxThread;
	DWORD	IDMinMaxThread;

	min_max_thread_param* min_max_param = new min_max_thread_param;
	min_max_param->array = arr;
	min_max_param->num_elem = num_elem;

	hMinMaxThread = CreateThread(NULL, 0, min_max, (LPVOID)min_max_param, 0, &IDMinMaxThread);
	if (hMinMaxThread == NULL)
		return GetLastError();

	WaitForSingleObject(hMinMaxThread, INFINITE);
	CloseHandle(hMinMaxThread);

	HANDLE	hAverageThread;
	DWORD	IDAverageThread;

	average_thread_param* average_param = new average_thread_param;
	average_param->array = arr;
	average_param->num_elem = num_elem;

	hAverageThread = CreateThread(NULL, 0, average, (LPVOID)average_param, 0, &IDAverageThread);
	if (hAverageThread == NULL)
		return GetLastError();

	WaitForSingleObject(hAverageThread, INFINITE);
	CloseHandle(hAverageThread);

	arr[min_max_param->index_min] = average_param->average;
	arr[min_max_param->index_max] = average_param->average;

	cout << "New array:";
	for (int i = 0; i < num_elem; i++)
		cout << " " << arr[i];

	return 0;
}