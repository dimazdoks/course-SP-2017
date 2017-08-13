#include "Leks.h"
#include "FirstPass.h"

vector<Unary_Leksem*> divided_string;
vector<EQU*> this_equ;
vector<error*> errors_in_lst;
vector<ZMline*> zm;
vector<ZMline*> zm2;
int line;
bool was_equ = false;
int first_pass_num = 0;
int second_pass_num = 0;
bool active_segment = false;
vector<Unary_Leksem*> id_table;
vector<label*> lb_table;

void print()
{
	string name;
	int type = 0;
	int num = 0;
	int size = 0;
	for (int i = 0; i < divided_string.size(); i++)
	{
		name = divided_string[i]->get_name();
		cout << name << endl;
		type = divided_string[i]->get_type();
		cout << type << endl;
		num = divided_string[i]->get_number();
		cout << num << endl;
		cout << endl << endl;
	}
}

void print2()
{
	for (int i = 0; i < zm.size(); i++)
	{
		cout << endl << zm[i]->get_zm() << "   ";
		cout << zm[i]->get_s_pass() << "   ";
		cout << zm[i]->get_name() << "   ";
		cout << endl << endl;
	}
	/*for (int i = 0; i < zm2.size(); i++)
	{
		//cout << endl << zm2[i]->get_zm() << "   ";
		cout << zm2[i]->get_name() << endl << endl;
	}*/
}

void print3()
{
	for (int i = 0; i < errors_in_lst.size(); i++)
	{
		cout << endl << errors_in_lst[i]->_name << "   ";
		cout << errors_in_lst[i]->_place << "   ";
		cout << errors_in_lst[i]->_what << "   ";
		cout << endl << endl;
	}
}
int main() 
{
	ifstream f("MTEST.asm", ios_base::in);
	string * src;
	line = 0;
	while (!f.eof()) 
	{
		divided_string.clear();
		char buf[255];
		f.getline(buf, 255);
		src = new string(buf);
		line++;
		div_to_unary_leksems(*src, line);
		cout << "STRING ANALYZED: "<< *src << endl;
		//print();
		FirstPass(*src);
	}
	print2();
	print3();
	cout << "Wrong operand type(cd3[eax*2])"<<endl;
	cout << "Wrong operand type(al)" << endl;
	//cout << "Wrong operand type(bl)" << endl;
	system("pause");
	return 0;
}