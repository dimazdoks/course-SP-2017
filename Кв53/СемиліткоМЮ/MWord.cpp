//Word.cpp

#include "stdafx.h"

char* data_registers_16[] =
{
	"AX","BX","CX","DX"
};

char* data_registers_8[] =
{
	"AH","BH","CH","DH",
	"AL","BL","CL","DL"
};

char* data_registers_offset[] =
{
	
	"SI","SP","BP","DI"
};

char* segment_registers[] = { "CS", "DS", "ES", "FS", "GS", "SS" };
//                       1      1        1         1      1      1         1      1      1        1     1     1      1
char* directives[] = { "END", "EQU", "SEGMENT", "ENDS", "PROC","ENDP", "ASSUME","FAR","NEAR", "DWORD","PTR","WORD","BYTE", "OFFSET" };

char* minstructions[] = { "RET", "PUSH", "POP", "MOV", "JNE", "CALL" };

char* types[] = { "DB","DD","DW" };

char symbols[] = { '+','-','[',']',',','?',':','*','(',')','/' };

int check_on_res_symbol(char symbol)
{
	int i;
	for (i = 0; i < sizeof(symbols); i++)
		if (symbols[i] == symbol)
			return 0;
	return 1;
}

void MWord::Set_word(char* str)
{
	strncpy_s(word, sizeof(word), str, strlen(str)+1);
	
}

void MWord::Set_type(char str)
{
	if (str != 'y')
		type = set_type(word);
	else 
		type = 12;
}

void MWord::Set_string_number(size_t i)
{
	string_number = i;
}

int MWord::Get_type()
{
	return type;
}

size_t MWord::Get_string_number()
{
	return string_number;
}

char* MWord::Get_word()
{
	return word;
}

size_t MWord::Get_len()
{
	return strlen(word);
}

char * MWord::Get_type_name()
{
	switch (type)
	{
	case 1:
		return "const_number";
	case 21:
		return "data_register_8";
	case 22:
		return "data_register_16";
	case 23:
		return "data_register_offset";
	case 3:
		return "segment_register";
	case 4:
		return "directive";
	case 5:
		return "minstruction";
	case 6:
		return "type";
	case 7:
		return "variable";
	case 8:
		return "const_string";
	case 9:
		return "binary_const";
	case 10:
		return "16_const";
	case 11:
		return "symbol";
	case 12:
		return "equ_value";
	default:
		return "unknown_item";
	}
}
