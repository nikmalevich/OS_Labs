#include "marker.h"
#include <iostream>

using namespace std;

extern CRITICAL_SECTION cs;

DWORD WINAPI marker(LPVOID param) {
	marker_param* m_param = (marker_param*)param;
	WaitForSingleObject(m_param->h_start_event, INFINITE);

	srand(m_param->num_thread);

	int num_marked_elems = 0;
	int* indexes = new int[m_param->num_elems];

	while (true) {
		int index = rand() % m_param->num_elems;

		EnterCriticalSection(&cs);
		if (m_param->arr[index] == 0) {
			Sleep(5);

			m_param->arr[index] = m_param->num_thread;
			indexes[num_marked_elems] = index;
			num_marked_elems++;

			Sleep(5);

			LeaveCriticalSection(&cs);
		}
		else {
			cout << "Number of thread: " << m_param->num_thread << "; number of marked elems: " << num_marked_elems << "; index: " << index << endl;

			LeaveCriticalSection(&cs);
			SetEvent(m_param->h_finish_event);

			if (WaitForMultipleObjects(2, m_param->h_continue_close_events, FALSE, INFINITE) == WAIT_OBJECT_0) {
				for (int i = 0; i < num_marked_elems; i++)
					m_param->arr[indexes[i]] = 0;

				return 0;
			}
		}
	}
}