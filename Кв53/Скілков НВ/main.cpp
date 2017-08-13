#include "Leks.h"

int main() {
	setlocale(LC_ALL, "Russian");
	cout << __cplusplus << endl;
	LeksList List;
	List.GetFromFile("TESTCOURSE.asm");
	List.PrintList();
	system("pause");
	return 0;
};