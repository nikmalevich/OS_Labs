// Server.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <Windows.h>
#include <iostream>
#include <fstream>
#include "Employee.h"

using namespace std;


int main()
{
    wchar_t filename[20];
    int num_employees;

    cout << "Enter the name of the binary file: ";
    wscanf_s(L"%ls", filename, (unsigned)_countof(filename));
    cout << "Enter number of employees: ";
    cin >> num_employees;

    fstream file(filename, fstream::in | fstream::out | fstream::binary | fstream::trunc);

    cout << "Enter employee data:" << endl;

    for (int i = 0; i < num_employees; i++) {
        cout << "Employee " << i + 1 << ":" << endl;
        Employee* employee = new Employee;

        cout << "Number: ";
        cin >> employee->num;
        cout << "Name: ";
        cin >> employee->name;
        cout << "Hours: ";
        cin >> employee->hours;

        file.write(reinterpret_cast<char*>(employee), sizeof(Employee));
    }

    file.flush();
    file.seekp(0);

    Employee* employees = new Employee[num_employees];
    file.read(reinterpret_cast<char*>(employees), sizeof(Employee)* num_employees);

    cout << "Employees:" << endl;

    for (int i = 0; i < num_employees; i++)
        cout << employees[i].num << "; " << employees[i].name << "; " << employees[i].hours << endl;

    file.close();
    
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
