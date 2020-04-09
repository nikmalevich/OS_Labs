// OS_Lab3.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <Windows.h>
#include "marker.h"

using namespace std;

CRITICAL_SECTION cs;

int main()
{
    int num_elems;
    int num_threads;

    cout << "Enter the number of elements in the array: ";
    cin >> num_elems;
    cout << "Enter the number of threads: ";
    cin >> num_threads;

    int* arr = new int[num_elems]();

    HANDLE* h_threads = new HANDLE[num_threads];
    DWORD* dw_threads = new DWORD[num_threads];
    InitializeCriticalSection(&cs);
    HANDLE h_start_event = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (h_start_event == NULL)
        return GetLastError();
    HANDLE* h_finish_events = new HANDLE[num_threads];
    HANDLE** h_continue_close_events = new HANDLE*[num_threads];

    for (int i = 0; i < num_threads; i++) {
        h_finish_events[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
        h_continue_close_events[i] = new HANDLE[2];
        h_continue_close_events[i][0] = CreateEvent(NULL, FALSE, FALSE, NULL);
        h_continue_close_events[i][1] = CreateEvent(NULL, FALSE, FALSE, NULL);
        if (h_finish_events[i] == NULL || h_continue_close_events[i][0] == NULL || h_continue_close_events[i][1] == NULL)
            return GetLastError();

        marker_param* m_param = new marker_param;
        m_param->arr = arr;
        m_param->num_elems = num_elems;
        m_param->num_thread = i + 1;
        m_param->h_start_event = h_start_event;
        m_param->h_finish_event = h_finish_events[i];
        m_param->h_continue_close_events = h_continue_close_events[i];

        h_threads[i] = CreateThread(NULL, 0, marker, (LPVOID)m_param, 0, &dw_threads[i]);
        if (h_threads[i] == NULL)
            return GetLastError();
    }

    SetEvent(h_start_event);

    int num_thread;

    for (int j = 0; j < num_threads; j++) {
        for (int i = 0; i < num_threads; i++)
            if (h_threads[i] != NULL)
                WaitForSingleObject(h_finish_events[i], INFINITE);

        for (int i = 0; i < num_elems; i++)
            cout << arr[i] << ' ';

        cout << endl << "Enter the thread number to be completed: ";
        cin >> num_thread;

        num_thread--;

        SetEvent(h_continue_close_events[num_thread][0]);
        WaitForSingleObject(h_threads[num_thread], INFINITE);
        CloseHandle(h_threads[num_thread]);
        CloseHandle(h_finish_events[num_thread]);
        CloseHandle(h_continue_close_events[num_thread][0]);
        CloseHandle(h_continue_close_events[num_thread][1]);
        h_threads[num_thread] = NULL;

        for (int i = 0; i < num_elems; i++)
            cout << arr[i] << ' ';

        cout << endl;

        for (int i = 0; i < num_threads; i++)
            if (h_threads[i] != NULL)
                SetEvent(h_continue_close_events[i][1]);
    }

    DeleteCriticalSection(&cs);
    CloseHandle(h_start_event);
    delete[] h_threads;
    delete[] h_finish_events;
    delete[] h_continue_close_events;
    delete[] arr;

    return 0;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
