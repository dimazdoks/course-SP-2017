//MakeTable.cpp


#include "stdafx.h"
using namespace std;

char* parse_leks(const char* word)
{
	char buf[50] = {0};
	char* leks;
	size_t i = 0;
	if (check_on_res_symbol(word[i]))
	{
		//if first symbol isn't reserved then we get word
		while (word[i] !=' ' && word[i] != '\0' && word[i] != '\t' && check_on_res_symbol(word[i]))
		{
			buf[i] = word[i];
			i++;
		}
	}
	else 
	{
		// if first symbol is reserved symbol then we get symbol
		leks = new char[2];
		leks[0] = word[0];
		leks[1] = '\0';
		return leks;
	}
	if (strlen(buf) > 0)
	{
		leks = new char[i + 1];
		strcpy_s(leks,i+1,buf);
	}
	return leks;
}

void maketable(MTable &table)
{
	ifstream file;
	char* str;
	map<string, string> equs;
	size_t num_of_str = 1;
	file.open("test.asm", ios_base::in);
	while (!file.eof())
	{
		MWord Cur_Word;
		char buf[100] = { 0 };
		size_t len;
		size_t i = 0;
		int is_get_leks = 0;
		file.getline(buf, 100);
		// cout << buf << endl;
		if (strlen(buf) > 0)
		{
			str = new char[len = (strlen(buf) + 1)];
			strcpy_s(str, len, buf);
			while (str[i] != NULL && i <= strlen(str) && str[i] != ';')
			{
				char* leks;
				if (str[i] == ' ' || str[i] == '\t')
					i++;
				else
				{
					is_get_leks = 1;
					leks = parse_leks(str + i);
					i += strlen(leks);
					incaps_string(leks);
					if (!strcmp(leks, "EQU"))
					{
						while (str[i] == ' ' || str[i] == '\t' || str[i] == '\0')
							i++;
						if (str[i] != '\0')
						{

							char equ_buf[10] = { 0 };
							char* equ_val;
							string equ_word;
							string equ_value;
							MTable::iterator equ_name = table.end();
							strcpy_s(equ_buf, sizeof(equ_buf), str + i);
							equ_val = new char[strlen(equ_buf) + 1];
							equ_value = equ_buf;
							strcpy_s(equ_val, strlen(equ_buf) + 1, equ_buf);
							i += strlen(equ_val);
							equ_name--;
							equ_word = equ_name->Get_word();
							equs[equ_word] = equ_val;
							Cur_Word.Set_word("EQU");
							Cur_Word.Set_type('n');
							Cur_Word.Set_string_number(num_of_str);
							table.push_back(Cur_Word);
							Cur_Word.Set_word(equ_val);
							Cur_Word.Set_type('y');
							Cur_Word.Set_string_number(num_of_str);
							table.push_back(Cur_Word);
						}
					}
					else
					{
						if (equs.find(leks) == equs.end())
						{
							Cur_Word.Set_word(leks);
							Cur_Word.Set_type('n');
							Cur_Word.Set_string_number(num_of_str);
							table.push_back(Cur_Word);
						}
						else
						{
							size_t j = 0;
							while (equs[leks].c_str()[j] != '\0')
							{
								char* new_leks;
								while (equs[leks].c_str()[j] == ' ' || equs[leks].c_str()[j] == '\t')
									j++;
								new_leks = parse_leks(equs[leks].c_str() + j);
								j += strlen(new_leks);
								incaps_string(new_leks);
								Cur_Word.Set_word(new_leks);
								Cur_Word.Set_type('n');
								Cur_Word.Set_string_number(num_of_str);
								table.push_back(Cur_Word);
							}
						}
					}

				}
			}
			if (is_get_leks)
				num_of_str++;
		}
	}
	file.close();
}