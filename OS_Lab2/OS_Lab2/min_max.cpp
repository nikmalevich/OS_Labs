#include "min_max_thread_param.h"
#include <Windows.h>
#include <iostream>

using namespace std;

DWORD WINAPI min_max(LPVOID thread_param) {
	min_max_thread_param* param = (min_max_thread_param*)thread_param;

	int min = INT_MAX;
	int max = INT_MIN;
	int cur_index_min = 0;
	int cur_index_max = 0;

	for (int i = 0; i < param->num_elem; i++) {
		if (param->array[i] < min) {
			Sleep(7);
			min = param->array[i];
			cur_index_min = i;
		}

		if (param->array[i] > max) {
			Sleep(7);
			max = param->array[i];
			cur_index_max = i;
		}
	}

	param->index_min = cur_index_min;
	param->index_max = cur_index_max;

	cout << "Min element: " << min << endl;
	cout << "Max element: " << max << endl;

	return 0;
}
