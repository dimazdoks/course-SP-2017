#include "Leks.h"
#include "FirstPass.h"
#include "SecondPass.h"

int main() {
	LoadFromFile("CO2.asm", "LeksFile.cod");
	FirstPass("FirstPass.cod");
	SecondPass("CO2.cod");
	system("pause");
	return 0;
}