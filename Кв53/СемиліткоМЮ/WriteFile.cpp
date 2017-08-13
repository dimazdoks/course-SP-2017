
#include "stdafx.h"

void print_table(MTable& table)
{
	ofstream f;
	f.open("table.txt");
	MTable::iterator i;
	size_t max = (--(table.end()))->Get_string_number();
	MTable::iterator k;
	size_t last = 1;
	for (i = table.begin(); i != table.end(); i++)
	{
		if (i->Get_string_number() <= max && i->Get_string_number() == last)
		{
			k = i;
			if (last != 1)
			{
				cout << endl;
				f << endl;
			}
			last++;
			while (k != table.end() && k->Get_string_number() == i->Get_string_number())
			{
				f << k->Get_word() << " ";
				cout << k->Get_word() << " ";
				k++;
			}
			f << endl;
			cout << endl;
		}
		f << i->Get_word() << " " << i->Get_type_name() << " " << i->Get_len() << " " <<endl;
		cout << i->Get_word() << " " << i->Get_type_name() << " " << i->Get_len() << " " << endl;
	}
	f.close();
}

void print_SS_Array(MTable table, SS_table& ss_tab, Assume_vector& V_assume, Label_vector& lvector, Var_vector& vvector, Segment_vector& svector, Proc_vector& pvector)
{
	ofstream f;
	f.open("ss_array.txt");
	size_t next;
	size_t assume_line = 0;
	size_t cur_line = 1;
	MTable::iterator itr = table.begin();
	SS_table::iterator sst_itr;
	Assume_vector::iterator asst;
	Label_vector::iterator ltr;
	Var_vector::iterator vtr;
	Segment_vector::iterator str;
	Proc_vector::iterator prt;
	for (sst_itr = ss_tab.begin(); sst_itr != ss_tab.end(); )
	{
		if (itr != table.end())
		{
			next = itr->Get_string_number() + 1;
			if (!strcmp(itr->Get_word(), "ASSUME"))
				assume_line = itr->Get_string_number();
			while (itr != table.end() && itr->Get_string_number() < next)
			{
				if (itr->Get_string_number() != assume_line)
					f << itr->Get_word() << " ";
				itr++;
			}
			f << endl;
		}
		if (cur_line != assume_line)
		{
			f << "|№ лексеми поля|№ 1го мнемок  |кількість     |№ 1го операнду|кількість     |№ 2го операнду|кількість     |" << endl;
			f << "|" << sst_itr->rec.label_number << "             |" << sst_itr->rec.mem_code_num << "             |" << sst_itr->rec.mem_code_len << "             |";
			f << sst_itr->rec.f_operand_num << "             |" << sst_itr->rec.f_operand_len << "             |";
			f << sst_itr->rec.s_operand_num << "             |" << sst_itr->rec.s_operand_len << "             |" << endl << endl;
			sst_itr++;
		}
		cur_line++;
	}
	f.close();
	cout << endl;
	f.open("List1.lst", ios_base::app);
	f << endl;
	cout << "PROC" << endl;
	f << "PROC" << endl;
	for (prt = pvector.begin(); prt != pvector.end(); prt++)
	{
		cout << "NAME: " << prt->name << "		OFFSET: " << get16(prt->offset) << "		SEGMENT: " << prt->segment << endl;
		f  << "NAME: " << prt->name << "		OFFSET: " << get16(prt->offset) << "		SEGMENT: " << prt->segment << endl;
	}
	cout << endl;
	f << endl;
	cout << "VARIABLES" << endl;
	f << "VARIABLES" << endl;
	for (vtr = vvector.begin(); vtr != vvector.end(); vtr++)
	{
		cout << "NAME: " << vtr->name << "		SIZE: " << get16(vtr->size) << "		SEGMENT: " << vtr->segment << "		VALUE: " << vtr->value << endl;
		f << "NAME: " << vtr->name << "			SIZE: " << get16(vtr->size) << "		SEGMENT: " << vtr->segment << "		VALUE: " << vtr->value << endl;
	}
	cout << endl;
	f << endl;
	cout << "LABELS" << endl;
	f << "LABELS" << endl;
	for (ltr = lvector.begin(); ltr != lvector.end(); ltr++)
	{
		cout << "NAME: " << ltr->name << "		OFFSET " << get16(ltr->offset) << "		SEGMENT: " << ltr->segment << endl; 
		f << "NAME: " << ltr->name << "			OFFSET " << get16(ltr->offset) << "		SEGMENT: " << ltr->segment << endl;
	}
	cout << endl;
	f << endl;
	cout << "SEGMENTS" << endl;
	f << "SEGMENTS" << endl;
	for (str = svector.begin(); str != svector.end(); str++)
	{
		cout << "NAME: " << str->name << "		LENTH: " << get16(str->len) << endl;
		f << "NAME: " << str->name << "			LENTH: " << get16(str->len) << endl;
	}
	cout << endl;
	f << endl;
	for (int i = 0; i < V_assume.size(); i++)
	{
		cout << "ASSUME " << i + 1 << endl;
		f << "ASSUME " << i + 1 << endl;
		for (auto itr = V_assume[i].begin(); itr != V_assume[i].end(); itr++)
		{
			cout << itr->first << " : " << itr->second << endl;
			f << itr->first << " : " << itr->second << endl;
		}
		f << endl;
		cout << endl;
	}
	f.close();
}