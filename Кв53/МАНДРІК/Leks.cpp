#include "Leks.h"

extern vector<Unary_Leksem*> divided_string;
extern vector<EQU*> this_equ;
extern vector<error*> errors_in_lst;
extern int line;

string GetAnalyzedString(ifstream src)
{
	char buf[255];
	src.getline(buf, 255);
	string * result = new string(buf);
	return *result;
};

void div_to_unary_leksems(string& src, int& num) 
{
	int pos = 0;
	EQU* current_equ = NULL;
	Unary_Leksem* Leksem = NULL;
	string buf, tmp_equ;
	bool flag_const_string = false;
	bool flag_end = false;
	bool magic_flag = true;
	bool no_need_to_check = false;
	int pos_in_str = 0;
	//start of buffer analyzis
	src += ' ';
	while (pos_in_str < src.size())
	{
		if ((src[pos_in_str] == 34) || (src[pos_in_str] == 39))
		{
			if (!flag_const_string)
			{
				if (!buf.empty())
					errors_in_lst.push_back(new error("Undefined leksem", buf, line));
				buf.clear();
				flag_const_string = true;
				flag_end = true;
				pos_in_str++;
				continue;
			}
			else if (flag_end)
			{
				//buf += src[pos_in_str];
				//pos_in_str++;
				pos++;
				//make_leksem(buf, pos);
				divided_string.push_back(new Unary_Leksem(buf, CONST, buf.size(), pos));
				pos_in_str++;
				buf.clear();
				flag_const_string = false;
				//continue;
				return;
			}
		}
		if (flag_const_string)
		{
			buf += src[pos_in_str];
			pos_in_str++;
			continue;
		}
		if (src[pos_in_str] == '\t') {
			pos_in_str++;
			continue;
		}
		if (check_one_symbol(src[pos_in_str]))
		{
			if (!buf.empty())
			{
				pos++;
				make_leksem(buf, pos);
			}
			buf.clear();
			buf += src[pos_in_str];
			pos++;
			make_leksem(buf,pos);
			buf.clear();
			pos_in_str++;
			continue;
		}
		if (!check_alphabet(src[pos_in_str]))
			errors_in_lst.push_back(new error("Undefined symbol", buf, line));
		if (src[pos_in_str] == ' ')
		{
			if (buf == "equ")
			{
				pos++;
				make_leksem(buf, pos);
				buf.clear();
				string name;
				for (int j = 0; j < pos_in_str - 4; j++)
					name += src[j];
				string value;
				for (int j = pos_in_str + 1; j < src.size(); j++)
					value += src[j];
				current_equ = new EQU;
				current_equ->name = name;
				current_equ->value = value;
				this_equ.push_back(current_equ);
				//return;
			}
			if (!buf.empty()) 
			{
				pos++;
				make_leksem(buf, pos);
			}
			buf.clear();
			pos_in_str++;
			continue;
		}
		buf += src[pos_in_str];
		pos_in_str++;
	};
};

bool check_alphabet(char is_checked)
{
	if ((is_checked >= 'a') && (is_checked <= 'z'))
		return true;
	else if ((is_checked >= '0') && (is_checked <= '9'))
		return true;
	else if (is_checked == ' ')
		return true;
	return false;
};

bool check_one_symbol(char is_checked)
{
	char symbols[] = { '[',']','(',')',':','+','-','*' ,',' };
	for (int i = 0; i < 9; i++)
		if (is_checked == symbols[i])
			return true;
	return false;
}

bool check_equ(vector<EQU*>	equ_list, string& src)
{
	for (int i = 0; i < equ_list.size(); i++)
		if (equ_list[i]->name == src) {
			src = equ_list[i]->value;
			return true;
		}
	return false;
}

bool check_binary_number(const string& src) 
{
	if((src[src.size() - 1] == 'b') && ((src[0] == '0')||(src[0] == '1')))
	{ 
		for (int i = 1; i < src.size(); i++) 
			if ((src[i] != '0') && (src[i] != '1'))
			{
				errors_in_lst.push_back(new error("Undefined symbol in bin const: ", src, line));
			}
		return true;
	}
	return false;
}

bool check_hex_number(const string& src)
{
	bool hex_error = false;
	bool begin_check = ((src[0] >= '0') && (src[0] <= '9') || (src[0] >= 'a') && (src[0] <= 'f'));
	if (src[src.size() - 1] == 'h' && begin_check)
	{
		for (int i = 1; i < src.size() - 1; i++)
		{
			if ((src[i] >= '0') && (src[i] <= '9') || (src[i] >= 'a') && (src[i] <= 'f'))
				continue;
			else
			{
				errors_in_lst.push_back(new error("Undefined symbol in hex const: ", src, line));
				hex_error = true;
			}
		}
		if (!hex_error)
			return true;
	}
	return false;
}

bool check_dec_number(const string& src)	
{
	bool dec_error = false;
	bool begin_check = ((src[0] >= '0') && (src[0] <= '9'));
	bool end_check = ((src[src.size() - 1] >= '0') && (src[src.size() - 1] <= '9')) || (src[src.size() - 1] == 'd');
	if (begin_check && end_check)
	{
		for (int i = 1; i < src.size() - 1; i++)
			if ((src[i] >= '0') && (src[i] <= '9'))
				continue;
			else
			{
				dec_error = true;
				break;
			}
		if (!dec_error)
			return true;
	}
	return false;
}

bool check_for_uid(string& src)
{
	for (int i = 0; i < src.size(); i++)
	{
		if (!check_alphabet(src[i]))
			return false;
	}
	return true;
}

int get_leks_type(const string& src)
{
	char* Reg32[] = { "eax", "ebx", "ecx", "edx", "ebp", "esp", "esi", "edi", "eflags", "epi" };
	char* Reg8[] = { "al","ah","bl","bh","cl","ch","dl","dh" };
	char* Direc[] = { "end","segment","ends","db","dd","dw","equ","if","else","endif" };
	char* SegReg[] = { "cs","ds","gs","es","ss","fs" };
	char* Comm[] = { "movsb","stos","inc","cmp","mov","and","shr","add","jnz" };
	for (int i = 0; i < 10; i++)
		if (src == Reg32[i])
			return REG32;
	for (int i = 0; i < 8; i++)
		if (src == Reg8[i])
			return REG8;
	for (int i = 0; i < 10; i++)
		if (src == Direc[i])
			return DIR;
	for (int i = 0; i < 6; i++)
		if (src == SegReg[i])
			return SREG;
	for (int i = 0; i < 9; i++)
		if (src == Comm[i])
			return COM;
	return ERR;
}

void make_leksem(string& src, int& num)
{
	if (check_equ(this_equ, src))
	{
		div_to_unary_leksems(src, line);
		return;
	}
	if (check_binary_number(src))
	{
		divided_string.push_back(new Unary_Leksem(src, CONSTB, src.size(), num));
		return;
	}
	else if (check_dec_number(src))
	{
		divided_string.push_back(new Unary_Leksem(src, CONSTD, src.size(), num));
		return;
	}
	else if (check_hex_number(src))
	{
		divided_string.push_back(new Unary_Leksem(src, CONSTH, src.size(), num));
		return;
	}
	else if (check_one_symbol(src[0]))
	{
		if (src == ":")
			if (divided_string[divided_string.size()-1]->get_type() != SREG)
				divided_string[divided_string.size() - 1]->set_type(LABEL);
		divided_string.push_back(new Unary_Leksem(src, OS, src.size(), num));
		return;
	}
	int type = get_leks_type(src);
	if (type == REG32)
		divided_string.push_back(new Unary_Leksem(src, REG32, src.size(), num));
	else if (type == REG8)
		divided_string.push_back(new Unary_Leksem(src, REG8, src.size(), num));
	else if (type == COM)
		divided_string.push_back(new Unary_Leksem(src, COM, src.size(), num));
	else if (type == SREG)
		divided_string.push_back(new Unary_Leksem(src, SREG, src.size(), num));
	else if (type == DIR)
	{
		if (num == 2)
			if (src == "segment")
				divided_string[divided_string.size() - 1]->set_type(SEG_NAME);
			else
				divided_string[divided_string.size() - 1]->set_type(VAR_NAME);
		divided_string.push_back(new Unary_Leksem(src, DIR, src.size(), num));
	}
	else if (check_for_uid)
	{
		if ((divided_string.size() != 0)&&(divided_string[divided_string.size() - 1]->get_name() != "jnz"))
			divided_string.push_back(new Unary_Leksem(src, MEM, src.size(), num));
		else
			divided_string.push_back(new Unary_Leksem(src, LABEL, src.size(), num));
	}
}