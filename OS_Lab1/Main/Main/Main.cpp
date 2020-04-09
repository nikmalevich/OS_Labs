#include <iostream>
#include <fstream>
#include <conio.h>
#include <Windows.h>

using namespace std;

struct employee {
	int num;
	char name[10];
	float hours;
};

int main()
{
	wchar_t bin_file_name[20];
	int num_records;

	cout << "Enter the name of the binary file: ";
	wscanf_s(L"%ls", bin_file_name, (unsigned)_countof(bin_file_name));

	cout << "Enter the number of records: ";
	cin >> num_records;

	wchar_t creater_com_line[100];

	STARTUPINFO creater_si;
	PROCESS_INFORMATION creater_pi;

	ZeroMemory(&creater_si, sizeof(STARTUPINFO));
	creater_si.cb = sizeof(STARTUPINFO);

	wsprintf(creater_com_line, L"Creater.exe %ls %d", bin_file_name, num_records);
	
	CreateProcess(NULL, creater_com_line, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &creater_si, &creater_pi);

	WaitForSingleObject(creater_pi.hProcess, INFINITE);

	CloseHandle(creater_pi.hThread);
	CloseHandle(creater_pi.hProcess);

	ifstream in(bin_file_name, ios::binary);

	in.read(reinterpret_cast<char*>(&num_records), sizeof(num_records));

	employee* employees = new employee[num_records];
	in.read(reinterpret_cast<char*>(employees), sizeof(employee) * num_records);

	in.close();

	for (int i = 0; i < num_records; i++) {
		cout << employees[i].num << " " << employees[i].name << " " << employees[i].hours << "\n";
	}

	wchar_t report_file_name[20];
	wchar_t payment[10];

	cout << "Enter the name of the report file: ";
	wscanf_s(L"%ls", report_file_name, (unsigned)_countof(report_file_name));

	cout << "Enter the payment: ";
	wscanf_s(L"%ls", payment, (unsigned)_countof(payment));

	wchar_t reporter_com_line[100];

	STARTUPINFO reporter_si;
	PROCESS_INFORMATION reporter_pi;

	ZeroMemory(&reporter_si, sizeof(STARTUPINFO));
	reporter_si.cb = sizeof(STARTUPINFO);

	wsprintf(reporter_com_line, L"Reporter.exe %ls %ls %ls", bin_file_name, report_file_name, payment);

	CreateProcess(NULL, reporter_com_line, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &reporter_si, &reporter_pi);

	WaitForSingleObject(reporter_pi.hProcess, INFINITE);

	CloseHandle(reporter_pi.hThread);
	CloseHandle(reporter_pi.hProcess);

	in.open(report_file_name);

	char str[10];
	in >> str;

	for (int i = 0; i < num_records; i++) {
		int num;
		char name[10];
		float hours;
		float payment;

		in >> num;
		in >> name;
		in >> hours;
		in >> payment;

		cout << num << " " << name << " " << hours << " " << payment << "\n";
	}

	in.close();

	system("pause");
	return 0;
}