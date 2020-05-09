// Server.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <Windows.h>
#include <iostream>
#include <fstream>
#include "Employee.h"


using namespace std;

DWORD WINAPI client_handler(LPVOID);

HANDLE* h_employee_mutex;
HANDLE* h_write_employee_mutex;
int* employees_id;
int num_employees;
char filename[20];
int* num_readers;

int main()
{
    cout << "Enter the name of the binary file: ";
    cin >> filename;
    cout << "Enter number of employees: ";
    cin >> num_employees;

    fstream file(filename, fstream::in | fstream::out | fstream::binary | fstream::trunc);

    cout << "Enter employee data:" << endl;

    for (int i = 0; i < num_employees; i++) {
        cout << "Employee " << i + 1 << ":" << endl;
        Employee employee;

        cout << "Number: ";
        cin >> employee.num;
        cout << "Name: ";
        cin >> employee.name;
        cout << "Hours: ";
        cin >> employee.hours;

        file.write(reinterpret_cast<char*>(&employee), sizeof(Employee));
    }

    file.flush();
    file.seekp(0);

    employees_id = new int[num_employees];
    Employee* employees = new Employee[num_employees];
    file.read(reinterpret_cast<char*>(employees), sizeof(Employee) * num_employees);

    cout << "Employees:" << endl;

    for (int i = 0; i < num_employees; i++) {
        employees_id[i] = employees[i].num;

        cout << employees[i].num << "; " << employees[i].name << "; " << employees[i].hours << endl;
    }

    int num_clients;

    cout << "Enter number of clients: ";
    cin >> num_clients;

    h_employee_mutex = new HANDLE[num_employees];
    h_write_employee_mutex = new HANDLE[num_employees];
    num_readers = new int[num_employees]();

    for (int i = 0; i < num_employees; i++) {
        h_employee_mutex[i] = CreateMutex(NULL, FALSE, NULL);
        h_write_employee_mutex[i] = CreateMutex(NULL, FALSE, NULL);
    }

    wchar_t client_com_line[20];
    STARTUPINFO* clients_si = new STARTUPINFO[num_clients];
    PROCESS_INFORMATION* clients_pi = new PROCESS_INFORMATION[num_clients];
    HANDLE* h_client_threads = new HANDLE[num_clients];
    DWORD* id_client_threads = new DWORD[num_clients];

    wsprintf(client_com_line, L"Client.exe");

    for (int i = 0; i < num_clients; i++) {
        HANDLE h_named_pipe = CreateNamedPipe(L"\\\\.\\pipe\\example_pipe", PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_WAIT, num_clients, 0, 0, INFINITE, (LPSECURITY_ATTRIBUTES)NULL);

        ZeroMemory(&clients_si[i], sizeof(STARTUPINFO));
        clients_si[i].cb = sizeof(STARTUPINFO);

        CreateProcess(NULL, client_com_line, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &clients_si[i], &clients_pi[i]);

        ConnectNamedPipe(h_named_pipe, NULL);

        h_client_threads[i] = CreateThread(NULL, 0, client_handler, (LPVOID)h_named_pipe, 0, &id_client_threads[i]);
    }

    WaitForMultipleObjects(num_clients, h_client_threads, TRUE, INFINITE);

    file.seekp(0);
    file.read(reinterpret_cast<char*>(&employees), sizeof(Employee) * num_employees);

    cout << "Employees:" << endl;

    for (int i = 0; i < num_employees; i++) {
        employees_id[i] = employees[i].num;

        cout << employees[i].num << "; " << employees[i].name << "; " << employees[i].hours << endl;
    }

    system("pause");

    for (int i = 0; i < num_employees; i++) {
        CloseHandle(h_employee_mutex[i]);
        CloseHandle(h_write_employee_mutex[i]);
    }

    for (int i = 0; i < num_clients; i++) {
        CloseHandle(h_client_threads[i]);
        CloseHandle(clients_pi[i].hThread);
        CloseHandle(clients_pi[i].hProcess);
    }

    delete[] h_employee_mutex;
    delete[] h_write_employee_mutex;
    delete[] employees_id;
    delete[] employees;
    delete[] clients_si;
    delete[] clients_pi;
    delete[] h_client_threads;
    delete[] id_client_threads;

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
