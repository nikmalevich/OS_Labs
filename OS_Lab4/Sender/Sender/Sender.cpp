// Sender.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <Windows.h>
#include <fstream>
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
    char* filename = argv[1];
    int num_process = atoi(argv[2]);
    wchar_t event_name[30];

    wsprintf(event_name, L"sender_start_event%d", num_process);

    HANDLE h_record_semaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, L"sender_record_semaphore");
    HANDLE h_empty_semaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, L"sender_empty_semaphore");
    HANDLE h_mutex = OpenMutex(SYNCHRONIZE, FALSE, L"sender_mutex");
    HANDLE h_start_event = OpenEvent(EVENT_MODIFY_STATE, FALSE, event_name);

    SetEvent(h_start_event);

    ofstream out_file(filename, ios::binary);
    char command;
    char message[20] = "zxcvbnmasdfghjklqwe";

    while (true) {
        cout << "Do you want to continue working? Enter y(yes) or n(no): ";
        cin >> command;

        if (command == 'y') {
            WaitForSingleObject(h_empty_semaphore, INFINITE);
            WaitForSingleObject(h_mutex, INFINITE);

            out_file.write(reinterpret_cast<char*>(message), sizeof(char)* 20);

            ReleaseMutex(h_mutex);
            ReleaseSemaphore(h_record_semaphore, 1, NULL);
        }
        else
            break;
    }

    out_file.close();

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
