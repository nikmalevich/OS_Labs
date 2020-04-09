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
	char* in_file_name = argv[1];
	char* out_file_name = argv[2];
	float payment = atof(argv[3]);

	ifstream in(in_file_name, ios::binary);
	ofstream out(out_file_name);

	int num_records = 0;
	in.read(reinterpret_cast<char*>(&num_records), sizeof(num_records));

	employee* employees = new employee[num_records];
	in.read(reinterpret_cast<char*>(employees), sizeof(employee) * num_records);

	out << in_file_name << "\n";

	for (int i = 0; i < num_records; i++) {
		out << employees[i].num << " " << employees[i].name << " " << employees[i].hours << " " << (employees[i].hours * payment) << "\n";
	}

	out.close();
	return 0;
}
