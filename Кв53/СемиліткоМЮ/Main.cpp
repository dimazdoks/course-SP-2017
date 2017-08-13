// CourseWork.cpp: определяет точку входа для консольного приложения.

#include "stdafx.h"

using namespace std;



int main()
{
	Assume_vector assume_vector;
	MTable table;

	SS_table ss_tab;

	Label_vector lvector;
	Label_vector::iterator ltr;

	Var_vector vvector;
	Var_vector::iterator vtr;

	Segment_vector svector;
	Segment_vector::iterator str;

	Proc_vector pvect;
	Proc_vector::iterator prt;

	maketable(table);
	print_table(table);

	//struct_check(table,ss_tab, assume_vector, lvector, vvector, svector, pvect);
	
	new_struct_check(table, ss_tab, assume_vector, lvector, vvector, svector, pvect);
	/*for (int i = 0; i < 1000; i++)
		cout << "i = " << i << "      16H = " << get16(i) << endl;*/
	return 0;
}



