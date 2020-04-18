// Receiver.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <Windows.h>
#include <iostream>
#include <fstream>

using namespace std;

int main()
{
    wchar_t filename[20];
    int num_records;
    int num_processes;

    cout << "Enter the name of the binary file: ";
    wscanf_s(L"%ls", filename, (unsigned)_countof(filename));
    cout << "Enter number of records: ";
    cin >> num_records;
    cout << "Enter number of processes: ";
    cin >> num_processes;

    wchar_t sender_com_line[100];
    wchar_t event_name[30];
    STARTUPINFO* senders_si = new STARTUPINFO[num_processes];
    PROCESS_INFORMATION* senders_pi = new PROCESS_INFORMATION[num_processes];
    HANDLE h_mutex = CreateMutex(NULL, FALSE, L"sender_mutex");
    HANDLE* h_start_events = new HANDLE[num_processes];
    HANDLE h_record_semaphore = CreateSemaphore(NULL, 0, num_records, L"sender_record_semaphore");
    HANDLE h_empty_semaphore = CreateSemaphore(NULL, num_records, num_records, L"sender_empty_semaphore");

    WaitForSingleObject(h_mutex, INFINITE);

    for (int i = 0; i < num_processes; i++) {
        ZeroMemory(&senders_si[i], sizeof(STARTUPINFO));
        senders_si[i].cb = sizeof(STARTUPINFO);
        wsprintf(sender_com_line, L"Sender.exe %ls %d", filename, i);
        wsprintf(event_name, L"sender_start_event%d", i);

        h_start_events[i] = CreateEvent(NULL, FALSE, FALSE, event_name);
        CreateProcess(NULL, sender_com_line, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &senders_si[i], &senders_pi[i]);
    }

    WaitForMultipleObjects(num_processes, h_start_events, TRUE, INFINITE);
    ReleaseMutex(h_mutex);

    char command;
    char message[20];
    int num_readed_messages = 0;
    fstream file(filename, fstream::binary | fstream::in | fstream::out | fstream::trunc);

    file.write(reinterpret_cast<char*>(&num_readed_messages), sizeof(num_readed_messages));
    file.seekp(sizeof(num_readed_messages));
    file.write(reinterpret_cast<char*>(&num_readed_messages), sizeof(num_readed_messages));

    file.close();
    
    while (true) {
        cout << "Do you want to continue working? If you want, enter 'y': ";
        cin >> command;

        if (command == 'y') {
            WaitForSingleObject(h_record_semaphore, INFINITE);
            WaitForSingleObject(h_mutex, INFINITE);

            fstream file(filename, fstream::binary | fstream::in | fstream::out);

            file.seekp(sizeof(num_readed_messages));
            file.read(reinterpret_cast<char*>(&num_readed_messages), sizeof(num_readed_messages));
            file.seekp(2 * sizeof(num_readed_messages) + num_readed_messages * sizeof(message));
            file.read(message, sizeof(message));

            cout << "Message: " << message << endl;
            
            num_readed_messages++;

            file.seekp(sizeof(num_readed_messages));
            file.write(reinterpret_cast<char*>(&num_readed_messages), sizeof(num_readed_messages));

            file.close();

            ReleaseMutex(h_mutex);
            ReleaseSemaphore(h_empty_semaphore, 1, NULL);
        }
        else {
            for (int i = 0; i < num_processes; i++)
                TerminateProcess(senders_pi[i].hProcess, 0);

            break;
        }

    }

    for (int i = 0; i < num_processes; i++) {
        CloseHandle(senders_pi[i].hThread);
        CloseHandle(senders_pi[i].hProcess);
        CloseHandle(h_start_events[i]);
    }
    CloseHandle(h_mutex);
    CloseHandle(h_record_semaphore);
    CloseHandle(h_empty_semaphore);
    delete[] senders_si;
    delete[] senders_pi;
    delete[] h_start_events;
    
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
