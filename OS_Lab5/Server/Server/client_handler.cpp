#include <Windows.h>
#include "Employee.h"
#include <fstream>
#include <iostream>

using namespace std;

extern HANDLE* h_employee_mutex;
extern HANDLE* h_write_employee_mutex;
extern int* employees_id;
extern int num_employees;
extern char filename[20];
extern int* num_readers;

DWORD WINAPI client_handler(LPVOID pipe) {
	HANDLE* h_named_pipe = (HANDLE*)pipe;
	fstream file(filename, fstream::in | fstream::out | fstream::binary);

	while (true) {
		char operation;
		char complete;
		Employee employee;

		ReadFile(h_named_pipe, &operation, sizeof(char), NULL, NULL);

		if (operation == 'e')
			break;

		int employee_id;

		ReadFile(h_named_pipe, &employee_id, sizeof(int), NULL, NULL);

		int employee_index = 0;

		for (int i = 0; i < num_employees; i++) {
			if (employees_id[i] == employee_id) {
				employee_index = i;

				break;
			}
		}

		if (operation == 'r') {
			WaitForSingleObject(h_employee_mutex[employee_index], INFINITE);
			ReleaseMutex(h_employee_mutex[employee_index]);

			if (num_readers[employee_index] == 0)
				WaitForSingleObject(h_write_employee_mutex[employee_index], INFINITE);

			num_readers[employee_index]++;

			file.seekp(sizeof(Employee) * employee_index);
			file.read(reinterpret_cast<char*>(&employee), sizeof(Employee));

			WriteFile(h_named_pipe, &employee, sizeof(Employee), NULL, NULL);
			ReadFile(h_named_pipe, &complete, sizeof(char), NULL, NULL);

			ReleaseMutex(h_write_employee_mutex[employee_index]);

			num_readers[employee_index]--;
		}
		else {
			WaitForSingleObject(h_write_employee_mutex[employee_index], INFINITE);
			ReleaseMutex(h_write_employee_mutex[employee_index]);
			WaitForSingleObject(h_employee_mutex[employee_index], INFINITE);
			file.seekp(sizeof(Employee) * employee_index);
			file.read(reinterpret_cast<char*>(&employee), sizeof(Employee));

			WriteFile(h_named_pipe, &employee, sizeof(Employee), NULL, NULL);

			char new_name[10];
			double new_hours;

			cout << "Enter new field values for employee number " << employee_id << endl;
			cout << "Name: ";
			cin >> new_name;
			cout << "Hours: ";
			cin >> new_hours;

			WriteFile(h_named_pipe, &new_name, sizeof(char) * 10, NULL, NULL);
			WriteFile(h_named_pipe, &new_hours, sizeof(double), NULL, NULL);

			ReadFile(h_named_pipe, &employee, sizeof(Employee), NULL, NULL);
			
			file.seekp(sizeof(Employee) * employee_index);
			file.write(reinterpret_cast<char*>(&employee), sizeof(Employee));

			file.flush();

			ReadFile(h_named_pipe, &complete, sizeof(char), NULL, NULL);

			ReleaseMutex(h_employee_mutex[employee_index]);
		}

		file.seekp(0);
	}

	FlushFileBuffers(h_named_pipe);
	DisconnectNamedPipe(h_named_pipe);
	CloseHandle(h_named_pipe);
	file.close();

	return 0;
}