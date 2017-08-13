//Set_type.cpp

#include "stdafx.h"

extern char* data_registers_8[];
extern char* data_registers_16[];
extern char* data_registers_offset[];
extern char* directives[];
extern char* minstructions[];
extern char* types[];
extern char symbols[];
extern char* segment_registers[];

int binary_num(const char* str)
{
	int i;
	for (i = 0; i < strlen(str) - 1; i++)
		if (str[i] != '0' && str[i] != '1')
			return 0;
	if (str[i] != 'B')
		return 0;
	return 1;
}

int sixteens_num(const char* str)
{
	int i;
	if (str[strlen(str) - 1] == 'H')
		if (str[0] > 64 && str[0] < 71)
			return 0;
		else
			for (i = 0; i < strlen(str) - 1; i++)
			{
				if (!((str[i] > 64 && str[i] < 71) || (str[i] > 47 && str[i] < 58)))
					return 0;
				return 1;
			}
	return 0;
}

int tens_num(const char* str)
{
	int i;
	for (i = 0; i < strlen(str); i++)
		if (str[i] < 48 || str[i] > 57)
			return 0;
	return 1;
}

void incaps_string(char* str)
{
	if (str != NULL)
	{
		for (int i = 0; i < strlen(str); i++)
			if (str[i] > 96 && str[i] < 123)
				str[i] -= 32;
	}
}

int is_variable(const char* str)
{
	int i;
	size_t len = strlen(str);
	if (str[0] > 64 && str[0] < 91 && len < 7)
	{
		for (i = 1; i < len; i++)
		{
			if (!((str[i] > 64 && str[i] < 91) || (str[i] > 47 && str[i] < 58)))
				return 0;
		}
		return 1;
	}
	return 0;
}

int set_type(char* str)
{
	int i;
	char* new_str = new char[strlen(str) + 1];
	strcpy_s(new_str, strlen(str) + 1, str);
	//symbols
	if (!check_on_res_symbol(str[0]))
		return 11;
	incaps_string(new_str);

	// numbers
	if (binary_num(new_str))
		return 9;
	if (sixteens_num(new_str))
		return 10;
	if (tens_num(new_str))
		return 1;

	// data_registers
	for (i = 0; i < 8; i++)
		if (!strcmp(new_str, data_registers_8[i]))
		{
			delete[] new_str;
			return 21;
		}

	for (i = 0; i < 4; i++)
		if (!strcmp(new_str, data_registers_16[i]))
		{
			delete[] new_str;
			return 22;
		}

	for (i = 0; i < 4; i++)
		if (!strcmp(new_str, data_registers_offset[i]))
		{
			delete[] new_str;
			return 23;
		}

	// segment_registers
	for (i = 0; i < 5; i++)
		if (!strcmp(new_str, segment_registers[i]))
		{
			delete[] new_str;
			return 3;
		}
	// directives
	for (i = 0; i < 14; i++)
		if (!strcmp(new_str, directives[i]))
		{
			delete[] new_str;
			return 4;
		}
	// minstructions
	for (i = 0; i < 6; i++)
		if (!strcmp(new_str, minstructions[i]))
		{
			delete[] new_str;
			return 5;
		}

	// types
	for (i = 0; i < 3; i++)
		if (!strcmp(new_str, types[i]))
		{
			delete[] new_str;
			return 6;
		}
	// variables
	if (is_variable(new_str))
	{
		delete[] new_str;
		return 7;
	}

	//const_string
	if (new_str[0] == '\'' && new_str[strlen(new_str) - 1] == '\'' && strlen(new_str) > 2)
	{
		delete[] new_str;
		return 8;
	}

	//

	delete[] new_str;
	return 0;
}

char* get16(int num)
{
	char res[50] = { 0 };
	char* fres;
	int x, y;
	int ind = 0;
	int ind1;
	x = num;
	do
	{
		num = x;
		ind1 = ind;
		x = x / 16;
		y = num - 16 * x;
		//cout << "x= " << x << "y= " << y << endl;
		while (ind1 != -1)
		{
			res[ind1 + 1] = res[ind1];
			ind1--;
		}
		switch (y)
		{
		case 0:
			res[0] = '0';
			break;
		case 1:
			res[0] = '1';
			break;
		case 2:
			res[0] = '2';
			break;
		case 3:
			res[0] = '3';
			break;
		case 4:
			res[0] = '4';
			break;
		case 5:
			res[0] = '5';
			break;
		case 6:
			res[0] = '6';
			break;
		case 7:
			res[0] = '7';
			break;
		case 8:
			res[0] = '8';
			break;
		case 9:
			res[0] = '9';
			break;
		case 10:
			res[0] = 'A';
			break;
		case 11:
			res[0] = 'B';
			break;
		case 12:
			res[0] = 'C';
			break;
		case 13:
			res[0] = 'D';
			break;
		case 14:
			res[0] = 'E';
			break;
		case 15:
			res[0] = 'F';
			break;
		}
		ind++;
	} while (x > 16);
	ind1 = ind;
	while (ind1 != -1)
	{
		res[ind1 + 1] = res[ind1];
		ind1--;
	}
	ind++;
	switch (x)
	{
	case 0:
		res[0] = '0';
		break;
	case 1:
		res[0] = '1';
		break;
	case 2:
		res[0] = '2';
		break;
	case 3:
		res[0] = '3';
		break;
	case 4:
		res[0] = '4';
		break;
	case 5:
		res[0] = '5';
		break;
	case 6:
		res[0] = '6';
		break;
	case 7:
		res[0] = '7';
		break;
	case 8:
		res[0] = '8';
		break;
	case 9:
		res[0] = '9';
		break;
	case 10:
		res[0] = 'A';
		break;
	case 11:
		res[0] = 'B';
		break;
	case 12:
		res[0] = 'C';
		break;
	case 13:
		res[0] = 'D';
		break;
	case 14:
		res[0] = 'E';
		break;
	case 15:
		res[0] = 'F';
		break;
	}
	fres = new char[ind];
	for (int i = 0; i < ind; i++)
		fres[i] = res[i];
	fres[ind] = '\0';
	return fres;
}