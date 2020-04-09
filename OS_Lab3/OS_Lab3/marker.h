#pragma once

#include <Windows.h>

struct marker_param {
	int* arr;
	int num_elems;
	int num_thread;
	HANDLE h_start_event;
	HANDLE h_finish_event;
	HANDLE* h_continue_close_events;
};

DWORD WINAPI marker(LPVOID);