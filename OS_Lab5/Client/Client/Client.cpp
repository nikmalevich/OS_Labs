// Client.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <Windows.h>
#include <iostream>
#include <fstream>
#include "Employee.h"

using namespace std;

int main()
{
	HANDLE h_named_pipe = CreateFile(L"\\\\.\\pipe\\example_pipe", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);

    while (true) {
        char operation;
        char complete = 'c';
        char new_employee = 'n';

        cout << "Enter the operation. Write(w), read(r), exit(e): ";
        cin >> operation;

        int employee_id;

        cout << "Enter employee ID: ";
        cin >> employee_id;

        WriteFile(h_named_pipe, &operation, sizeof(char), NULL, NULL);

        if (operation == 'e') {
            break;
        }

        WriteFile(h_named_pipe, &employee_id, sizeof(int), NULL, NULL);

        Employee employee;

        ReadFile(h_named_pipe, &employee, sizeof(Employee), NULL, NULL);

        cout << employee.num << "; " << employee.name << "; " << employee.hours << endl;

        if (operation == 'w') {
            char new_name[10];
            double new_hours;

            ReadFile(h_named_pipe, &new_name, sizeof(char) * 10, NULL, NULL);
            ReadFile(h_named_pipe, &new_hours, sizeof(double), NULL, NULL);

            strcpy_s(employee.name, new_name);
            employee.hours = new_hours;

            system("pause");

            WriteFile(h_named_pipe, &employee, sizeof(Employee), NULL, NULL);
        }

        system("pause");

        WriteFile(h_named_pipe, &complete, sizeof(char), NULL, NULL);
    }

    CloseHandle(h_named_pipe);
    
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
