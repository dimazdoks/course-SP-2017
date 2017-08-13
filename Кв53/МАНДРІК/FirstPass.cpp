#include "FirstPass.h"

extern vector<Unary_Leksem*> divided_string;
extern vector<EQU*> this_equ;
extern vector<error*> errors_in_lst;
extern int line;
extern bool was_equ;
extern int first_pass_num;
extern int second_pass_num;
extern vector<ZMline*> zm;
extern bool active_segment;
extern vector<Unary_Leksem*> id_table;
extern vector<label*> lb_table;
extern vector<ZMline*> zm2;

void FirstPass(string& src)
{
	int k = 5;
	active_segment = false;
	if (divided_string[0]->get_type() == LABEL)
	{
		lb_table.push_back(new label(divided_string[0]->get_name(), false));
	}
	if (divided_string.size() > 1) 
	{
		if (divided_string[1]->get_name() == "segment")
		{
			if (!active_segment) {
				first_pass_num = 0;
				zm.push_back(new ZMline(src, first_pass_num, line));
			}
			else
				errors_in_lst.push_back(new error("SEG ERROR", "", line));
		}
		if (divided_string[1]->get_name() == "ends")
		{
			zm.push_back(new ZMline(src, first_pass_num, line));
			active_segment = false;
		}
		if (divided_string[1]->get_type() == DIR)
		{
			if ((divided_string[1]->get_name() == "db") && ((divided_string[2]->get_type() == CONSTB) || (divided_string[2]->get_type() == CONSTD) || (divided_string[2]->get_type() == CONSTH)))
			{
				zm.push_back(new ZMline(src, first_pass_num, line));
				first_pass_num++;
				if (was_equ)
				{
					zm[zm.size() - 1]->do_hex("=");
					was_equ = false;
				}
				zm[zm.size() - 1]->s_pass(divided_string[2]->get_name());
			}
			else if (divided_string[1]->get_name() == "db")
			{
				zm.push_back(new ZMline(src, first_pass_num, line));
				first_pass_num += divided_string[2]->get_name().size();
				if (was_equ)
				{
					zm[zm.size() - 1]->do_hex("=");
					was_equ = false;
				}
				if ((divided_string[2]->get_type() == CONSTB) || (divided_string[2]->get_type() == CONSTD) || (divided_string[2]->get_type() == CONSTH))
					zm[zm.size() - 1]->s_pass(divided_string[2]->get_name());
			}
			else if (divided_string[1]->get_name() == "dw")
			{
				zm.push_back(new ZMline(src, first_pass_num, line));
				first_pass_num += 2;
				if (was_equ)
				{
					zm[zm.size() - 1]->do_hex("=");
					was_equ = false;
				}
				zm[zm.size() - 1]->s_pass(divided_string[2]->get_name());
			}
			else if (divided_string[1]->get_name() == "dd")
			{
				zm.push_back(new ZMline(src, first_pass_num, line));
				first_pass_num += 4;
				if (was_equ)
				{
					zm[zm.size() - 1]->do_hex("=");
					was_equ = false;
				}
				zm[zm.size() - 1]->s_pass(divided_string[2]->get_name());
			}
			else if (divided_string[1]->get_name() == "equ")
			{
				zm.push_back(new ZMline(src, first_pass_num, line));
				was_equ = true;
				if (was_equ)
				{
					zm[zm.size() - 1]->do_hex("=");
					was_equ = false;
				}
				if ((divided_string[2]->get_type() == CONSTB)|| (divided_string[2]->get_type() == CONSTD)|| (divided_string[2]->get_type() == CONSTH))
					zm[zm.size() - 1]->s_pass(divided_string[2]->get_name());
			}
		}
		//code 
		//
		else 
		{
			zm.push_back(new ZMline(src, first_pass_num, line));
			for (int i = 0; i < divided_string.size(); i++)
			{
				if (divided_string[i]->get_type() == COM)
				{
					if (divided_string[divided_string.size() - 1]->get_type() == REG32)

						first_pass_num++;
					//else
					//errors_in_lst.push_back(new error(src, "wrong operand type",line));
				}
				else if (divided_string[i]->get_type() == SREG)
					first_pass_num++;
				else if (divided_string[i]->get_type() == MEM)
					first_pass_num += 4;
				else if (divided_string[i]->get_name() == "[")
					first_pass_num += 2;
				else if ((divided_string[i]->get_type() == LABEL)&&(i != 0))
				{
					first_pass_num += 5;
					for (int j = 0; j < lb_table.size(); j++)
						if (lb_table[j]->get_name() == divided_string[i]->get_name())
						{
							first_pass_num -= 5;
							first_pass_num++;
						}
				}
				else if ((divided_string[i]->get_type() == CONSTH)&&(divided_string[0]->get_name() != "shr"))
				{
					int val = 0,mul = 1;
					string data;
					char current;
					data = divided_string[i]->get_name();
					for (int k = data.size() - 2; k >= 0; k--)
					{
						if (mul == 1)
						{
							current = data[k];
							val += current - '0';
						}
						else
						{
							current = data[k];
							val += (current - '0')*mul;
						}
						mul*=16;
					}
					first_pass_num += val;
				}
				if ((i > 0)&&(divided_string[i - 1]->get_name() == "inc")&&(divided_string[i]->get_name() == "eax"))
					zm[zm.size() - 1]->s_pass("40");
				else if ((i > 0) && (divided_string[i - 1]->get_name() == "inc") && (divided_string[i]->get_name() == "edx"))
					zm[zm.size() - 1]->s_pass("42");
				else if ((i < divided_string.size() - 1)&&(divided_string[i]->get_name() == "jnz")&&(divided_string[i+1]->get_name() == lb_table[0]->get_name()))
					zm[zm.size() - 1]->s_pass("75 DB");
				else if ((i < divided_string.size() - 1) && (divided_string[i]->get_name() == "jnz") && (divided_string[i + 1]->get_name() != lb_table[0]->get_name()))
					zm[zm.size() - 1]->s_pass("0F 85 2F R");
				else if (divided_string[i]->get_name() == "stos")
					zm[zm.size() - 1]->s_pass("AA");
				else if (divided_string[i]->get_name() == "movsb")
					zm[zm.size() - 1]->s_pass("A4");
				else if (divided_string[i]->get_name() == "shr")
					zm[zm.size() - 1]->s_pass("C1 E8 ffh");
				else if (divided_string[i]->get_name() == "mov")
					zm[zm.size() - 1]->s_pass("8B 3C5D 9 R");
				else if (divided_string[i]->get_name() == "2h")
					zm[zm.size() - 1]->s_pass("66| 83 047D 7 R 2h");
				else if (divided_string[i]->get_name() == "cmp")
					zm[zm.size() - 1]->s_pass("3B 1C");
				else if ((i < divided_string.size() - 1)&&(divided_string[i]->get_name() == "and")&&(divided_string[i+1]->get_name() == "es"))
					zm[zm.size() - 1]->s_pass("26: 21 1C5D 9 R");
				else if (divided_string[i]->get_name() == "and")
					zm[zm.size() - 1]->s_pass("21 045D 9 R");

			}
		}
	}
}

string make_hex(int& Number)
{
	int Numb;
	string Value;
	char Temp;
	if (Number >= 0)
		Numb = Number;
	else
		Numb = 255 + Number + 1;
	if (Numb) {
		while (Numb) {
			if (Numb % 2) Value.push_back('1');
			else  Value.push_back('0');
			Numb /= 2;
		}
		for (int i = 0; i < Value.size() / 2; i++) {
			Temp = Value[i];
			Value[i] = Value[Value.size() - 1 - i];
			Value[Value.size() - 1 - i] = Temp;
		}
		return Value;
	}
	return "0";
}

string bin_hex(string num) {
	int r;
	string n, tmp;
	string val;
	if (r = num.size() % 4) {
		for (int i = 0; i < 4 - r; i++) {
			n += '0';
		}
	}
	n += num;
	r = n.size() / 4;
	for (int i = 0; i < r; i++) {
		tmp.clear();
		for (int j = i * 4; j < (i + 1) * 4; j++) {
			tmp.push_back(n[j]);
		}
		if (tmp == "0000") val.push_back('0');
		if (tmp == "0001") val.push_back('1');
		if (tmp == "0010") val.push_back('2');
		if (tmp == "0011") val.push_back('3');
		if (tmp == "0100") val.push_back('4');
		if (tmp == "0101") val.push_back('5');
		if (tmp == "0110") val.push_back('6');
		if (tmp == "0111") val.push_back('7');
		if (tmp == "1000") val.push_back('8');
		if (tmp == "1001") val.push_back('9');
		if (tmp == "1010") val.push_back('A');
		if (tmp == "1011") val.push_back('B');
		if (tmp == "1100") val.push_back('C');
		if (tmp == "1101") val.push_back('D');
		if (tmp == "1110") val.push_back('E');
		if (tmp == "1111") val.push_back('F');
	}
	return val;
}