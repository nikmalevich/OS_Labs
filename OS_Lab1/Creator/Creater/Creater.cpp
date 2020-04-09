#include <iostream>
#include <fstream>

using namespace std;

struct employee {
	int num;
	char name[10];
	float hours;
};

int main(int argc, char* argv[])
{
	char* file_name = argv[1];
	int num_records = atoi(argv[2]);

	ofstream out(file_name, ios::binary);

	employee* employees = new employee[num_records];

	for (int i = 0; i < num_records; i++) {
		employee new_employee;

		cout << "Enter number of employee: ";
		cin >> new_employee.num;
		cout << "Enter name of employee: ";
		cin >> new_employee.name;
		cout << "Enter hours of employee: ";
		cin >> new_employee.hours;

		employees[i] = new_employee;
	}

	out.write(reinterpret_cast<char*>(&num_records), sizeof(num_records));
	out.write(reinterpret_cast<char*>(employees), sizeof(employee) * num_records);
	
	return 0;
}
