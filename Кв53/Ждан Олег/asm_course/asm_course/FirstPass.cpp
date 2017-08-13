#include "FirstPass.h"

extern vector<vector<string>> Leks;
extern vector<string> AsmCod;
vector<Var>Variables;
vector<Segm> Segment;
vector<SegR> SegRegister;
vector<Err> error_log;
vector<int> offset;

bool FirstPass(const char *filein) {
	ofstream fin(filein);
	if (!fin) {
		cout << "InputFile opening error\n";
		return false;
	}

	vector<string>::iterator it1;
	vector<string>::iterator it3 = AsmCod.begin();

	Err err;
	Var var;
	Segm Seg;
	int length = 0;
	int pop_length = 0;
	int num = 0;
	string proc;
	
	//Segment & Data
	for (vector<vector<string>>::iterator it = Leks.begin(); it != Leks.end(); it++) {
		num++;
		//Segment
		it1 = find(it->begin(), it->end(), "SEGMENT");
			if (it1 != it->end()) {
				it1--;
				Seg.name = *it1;
			}

			it1 = find(it->begin(), it->end(), "ENDS");
			if (it1 != it->end()) {
				if (*(it1 - 1) != Seg.name) {
					err.error ="Block nesting error";
					err.oper ="";
					err.str = num;
					error_log.push_back(err);
					err.error = "Open segments:";
					err.oper =Seg.name;
					err.str = -1;
					error_log.push_back(err);
				}
				length = 0;
			}
			//Variables
			it1 = find(it->begin(), it->end(), "DB");
			if (it1 != it->end()) {

				var.name = *(it1 - 1);
				var.attr = Seg.name;
				var.type = "L BYTE";
				//debug
				vector<Var>::iterator it2 = Variables.begin();
				while (it2 != Variables.end() && it2->name != (*(it1 - 1))) {
					it2++;
				}
				if (it2 != Variables.end()) {
					err.error = " error A2005: Symbol is multidefined:";
					err.oper = it2->name;
					err.str = num;
					error_log.push_back(err);
				}
				else {
					CheckingVar(it1, it, num, 8, 2, 255);
				}
				if (it2 == Variables.end()) {
					Variables.push_back(var);
				}
			}

			it1 = find(it->begin(), it->end(), "DW");
			if (it1 != it->end()) {
				vector<Var>::iterator it5;
				var.name = *(it1 - 1);
				var.attr = Seg.name;
				var.type = "L WORD";
				//debug								
				it5 = Variables.begin();
				while (it5 != Variables.end() && it5->name != (*(it1 - 1))) {
					it5++;
				}
				if (it5 != Variables.end()) {
					err.error = " error A2005: Symbol is multidefined:";
					err.oper = it5->name;
					err.str = num;
					error_log.push_back(err);
				} else {
					CheckingVar(it1, it, num, 16, 4, 65535);
				}
				//
				if (it5 == Variables.end()) {
					Variables.push_back(var);
				}
			}

			it1 = find(it->begin(), it->end(), "DD");
			if (it1 != it->end()) {
				vector<Var>::iterator it5;
				var.name = *(it1 - 1);
				var.attr = Seg.name;
				var.type = "L DWORD";
				//debug
				it5 = Variables.begin();
				while (it5 != Variables.end() && it5->name != (*(it1 - 1))) {
					it5++;
				}
				if (it5 != Variables.end()) {
					err.error = " error A2005: Symbol is multidefined:";
					err.oper = it5->name;
					err.str = num;
					error_log.push_back(err);
				}
				else {	
					CheckingVar(it1, it, num, 32, 8, 4294967295);
				}
				//
				if (it5 == Variables.end()) {
					Variables.push_back(var);
				}
			}

		it1 = find(it->begin(), it->end(), "PROC");
		if (it1 != it->end()) {
			var.name = *(it1 - 1);
			var.attr = Seg.name;
			proc = var.name;
			var.type = "N PROC";
			Variables.push_back(var);
		}

		it1 = find(it->begin(), it->end(), "ENDP");
		if (it1 != it->end()) {
			if ((*(it1 - 1)) != proc) {
				err.error = "Block nesting error";
				err.oper = "";
				err.str = num;
				error_log.push_back(err);
			}
		}

		it1 = find(it->begin(), it->end(), ":");
		if (it1 != it->end() && (it1+1)==it->end()) {
			var.name = *(it1 - 1);
			var.attr = Seg.name;
			var.type = "L NEAR";
			Variables.push_back(var);
		}
	}
	//Mnemocod operation
	length = 0;
	num = 0;

	for (vector<vector<string>>::iterator it = Leks.begin(); it != Leks.end(); it++) {
		num++;

		//Segment
		it1 = find(it->begin(), it->end(), "SEGMENT");
		if (it1 != it->end()) {
			Seg.name = *(it1-1);
			it1 = find(it->begin(), it->end(), "USE16");
			if (it1 != it->end()) {
				Seg.type = true;
			}
			else {
				Seg.type = false;
			}
			string SegReg[] = { "CS","DS","GS","ES","SS","FS"};
			SegR SegIt;
			it1 = (it + 1)->begin();
			if (*it1 == "ASSUME") {
				while (it1 != (it + 1)->end()) {
					if (find(SegReg, SegReg + 6, *(it1)) != SegReg + 6) {
						SegIt.segm = Seg.name;
						SegIt.segmr = *(it1);
						SegIt.type =*(it1+2);
						SegRegister.push_back(SegIt);
					}
					it1++;
				}
			}

		}

		it1 = find(it->begin(), it->end(), "ENDS");
		if (it1 != it->end()) {
			Seg.length = length;
			Segment.push_back(Seg);
			length = 0;
		}

		//Variables
		it1 = find(it->begin(), it->end(), "DB");
		if (it1 != it->end()) {
			vector<Var>::iterator iter=Variables.begin();
			while (iter->name != (*(it1 - 1))) {
				iter++;
			}
			if (GetType(*(it1 + 1)) == "Text constant")
				length += (*(it1 + 1)).size() - 2;
			else
				length += 1;
			iter->value = pop_length;
		}

		it1 = find(it->begin(), it->end(), "DW");
		if (it1 != it->end()) {
			vector<Var>::iterator iter = Variables.begin();
			while (iter->name != (*(it1 - 1))) {
				iter++;
			}
			length += 2;
			iter->value = pop_length;
		}

		it1 = find(it->begin(), it->end(), "DD");
		if (it1 != it->end()) {
			vector<Var>::iterator iter = Variables.begin();
			while (iter->name != (*(it1 - 1))) {
				iter++;
			}
			length += 4;
			iter->value = pop_length;
		}

		it1 = find(it->begin(), it->end(), "PROC");
		if (it1 != it->end()) {
			vector<Var>::iterator iter = Variables.begin();
			while (iter->name != (*(it1 - 1))) {
				iter++;
			}
			iter->value = pop_length;
		}

		it1 = find(it->begin(), it->end(), ":");
		if (it1 != it->end() && (it1 + 1) == it->end()) {
			vector<Var>::iterator iter = Variables.begin();
			while (iter->name != (*(it1 - 1))) {
				iter++;
			}
			iter->value = pop_length;
		}

		it1 = find(it->begin(), it->end(), "INC");
		if (it1 != it->end()) {
			length = MnemVar(length, num, 3,Seg.name, SegRegister, it1, it);
			if (GetType(*(it1 + 1)) == "32-bit register") {
				err.error = " warning A4057: Illegal size for operand";
				err.oper = "";
				err.str = num;
				error_log.push_back(err);
			}
		}

		it1 = find(it->begin(), it->end(), "POP");
		if (it1 != it->end()) {
			length=MnemVar(length, num, 3, Seg.name, SegRegister, it1, it);
			if (GetType(*(it1 + 1)) == "32-bit register") {
				err.error = " warning A4057: Illegal size for operand";
				err.oper = "";
				err.str = num;
				error_log.push_back(err);
			}
		}

		it1 = find(it->begin(), it->end(), "PUSH");
		if (it1 != it->end()) {
			length = MnemVar(length, num, 2, Seg.name, SegRegister, it1, it);
		}

		it1 = find(it->begin(), it->end(), "AND");
		if (it1 != it->end()) {
			string str,str1;
			str1 = GetType(*(it->end() - 1));
			str = GetType(*(it1 + 1));
			if (str == str1) {
				if (str == "32-bit register") {
					length += 3;
				}
				else {
					length += 2;
				}
			}
			if (str == "Users Identifier") {
				vector<Var>::iterator it5;
				it5 = Variables.begin();
				while (it5 != Variables.end() && it5->name != (*(it1 + 1))) {
					it5++;
				}
				Match(it5, num, str1);
				length = MnemVar(length, num, 0, Seg.name, SegRegister, (it1), it);
			}
			if (str1 == "Hexadecimal constant") {
				length += ((it->end() - 1)->size() / 2);
			}
			if (str1 == "Binary constant") {
				length += ((it->end() - 1)->size() / 8);
			}
			if (str1 == "Decimal constant") {
				length += (((it->end() - 1)->size() - 1) / 256) + 1;
			}
		}

		it1 = find(it->begin(), it->end(), "MOV");
		if (it1 != it->end()) {
			string str = GetType(*(it1 + 1));
			string str1 = GetType(*(it->end() - 1));
			if (str == str1) {
				if (str == "32-bit register") {
					length += 3;
				}
				else {
					length += 2;
				}
			}
			if (GetType(*(it1 + 3)) == "Users Identifier") {
				vector<Var>::iterator it5 = Variables.begin();
				while (it5 != Variables.end() && it5->name != (*(it1 + 3))) {
					it5++;
				}
				length = MnemVar(length, num, 0, Seg.name, SegRegister, (it1+2), it);
				if ((it1 + 4) == it->end()) {
					length--;
					}
				Match(it5, num, str);
			}
			if (str1 == "Hexadecimal constant") {
				length += ((it->end() - 1)->size() / 2);
			}
			if (str1 == "Binary constant") {
				length += ((it->end() - 1)->size() / 8);
			}
			if (str1 == "Decimal constant") {
				length += (((it->end() - 1)->size() - 1) / 256) + 1;
			}
		}
		
		it1 = find(it->begin(), it->end(), "XCHG");
		if (it1 != it->end()) {
			string str = GetType(*(it1 + 1));
			string str1 = GetType(*(it->end() - 1));
			if (str == str1) {
				string type = GetType(*(it1 + 1));
				if (*(it1 + 1) == "AX" || *(it1 + 1) == "EAX") {
					length++;
				}
				else {
					it1 = find(it->begin(), it->end(), ",");
					if (*(it1 + 1) == "AX" || *(it1 + 1) == "EAX") {
						length++;
					}
					else {
						length += 2;
					}
				}
			}
			if (str == "32-bit register")
				length++;
		}

		it1 = find(it->begin(), it->end(), "CALL");
		if (it1 != it->end()) {
			
			if (*(it1 + 1) == "FAR" && *(it1 + 2) == "PTR") {
				it1 += 2;
			}
			length = MnemVar(length, num, 0, Seg.name, SegRegister, it1, it);
		}

		it1 = find(it->begin(), it->end(), "JNE");
		if (it1 != it->end()) {
			length += 2;
		}

		it1 = find(it->begin(), it->end(), "RET");
		if (it1 != it->end()) {
			length += 1;
		}

		it1 = find(it->begin(), it->end(), "JL");
		if (it1 != it->end()) {
			vector<Var>::iterator iter = Variables.begin();
			while (iter != Variables.end() && iter->name != (*(it1 + 1))) {
				iter++;
			}
			if(iter->value < 0){
				length += 4;
			}else{ length += 2; }
		}

		//out
		it1 = it->begin(); 
		if (*it1 != "ASSUME" && *it1 != "END") {
			fin << setfill('0') << setw(4) << hex << uppercase << pop_length;
			fin << setfill(' ');
			fin << setw(10) << "" << *it3 << "\n";
		}
		else {
			fin << setw(14) << "" << *it3 << "\n";
		}

		for (vector<Err>::iterator it1 = error_log.begin(); it1 != error_log.end(); it1++) {
			if (it1->str == num) {
				fin << "Str[" << dec << it1->str << "]:" << it1->error << it1->oper << "\n";
			}
		}
		offset.push_back(pop_length);
		pop_length = length;
		it3++;
	}

	fin << "\n";
	//out
	PrintVar(fin);
	vector<SegR>::iterator iter = SegRegister.begin();
	for(vector<SegR>::iterator iter = SegRegister.begin(); iter != SegRegister.end(); iter++) {
	}
	return true;
}

int MnemVar(int length,int num, int a, string name,vector<SegR> SegRegister,vector<string>::iterator it1, vector<vector<string>>::iterator it) {
	Err err;
	vector<Var>::iterator it5 = Variables.begin();
	string str = GetType(*(it1 + 1));
		if (str == "Users Identifier") {
			while (it5 != Variables.end() && it5->name != (*(it1 + 1))) {
				it5++;
			}
			if (it5 != Variables.end()) {
				length += 3;
				vector<SegR>::iterator iter = SegRegister.begin();
				while (iter != SegRegister.end() && (iter->segm != name || iter->type!=it5->attr)) {
					iter++;
				}
				if (iter == SegRegister.end()) {
					err.error = " error A2068 : Cannot address with segment register";
					err.oper = "";
					err.str = num;
					error_log.push_back(err);
				}
				else {
					if (iter->segmr == "GS" || iter->segmr == "FS" || iter->segmr == "ES") {
						length++;
					}
					if (it5->type == "L DWORD" && *(it1) != "CALL") {
						length++;
					}
				}
			}
			else {
				length += 3;
				err.error = "  error A2009: Symbol not defined:";
				err.oper = (*(it1 + 1));
				err.str = num;
				error_log.push_back(err);
			}
		}
		else {
			if (str == "32-bit register") {
				length += 1;
			}
			if (str == "Hexadecimal constant") {
				length += ((it->end() - 1)->size() / 2);
			}
			if (str == "Binary constant") {
				length += ((it->end() - 1)->size() / 8);
			}
			if (str == "Decimal constant") {
				length += (((it->end() - 1)->size() - 1) / 256) + 1;
			}
		}
		length += 1;
	
	return length;
}

void CheckingVar(vector<string>::iterator it1, vector<vector<string>>::iterator it, int num, size_t bin, size_t hex, int dec) {
	Err err;
	string str= GetType(*(it1 + 1));
	vector<vector<string>>::iterator it3 = it + 1;
	while (it3 != Leks.end() && *(it3->begin()) != (*(it1 - 1))) {
		it3++;
	}
	if (it3 != Leks.end()) {
		err.error = " error A2005: Symbol is multidefined:";
		err.oper = (*(it1 - 1));
		err.str = num;
		error_log.push_back(err);
	}
	else {
		if ((it1 + 2) != it->end()) {
			err.error = "warning A4001 : Extra characters on line";
			err.oper = "";
			err.str = num;
			error_log.push_back(err);
		}
		else {
			if ((it1 + 2) == it->end()) {
				if (str == "Binary constant") {
					if ((*(it1 + 1)).size() > bin + 1) {
						err.error = "error A2050: Value out of range";
						err.oper = "";
						err.str = num;
						error_log.push_back(err);
					}
				}
				else {
					if (str == "Hexadecimal constant") {
						if ((*(it1 + 1)).size() > hex + 1) {
							err.error = "error A2050: Value out of range";
							err.oper = "";
							err.str = num;
							error_log.push_back(err);
						}
					}
					else {
						if (str == "Decimal constant") {
							if (stoi(*(it1 + 1)) > dec) {
								err.error = "error A2050: Value out of range";
								err.oper = "";
								err.str = num;
								error_log.push_back(err);
							}
						}
						else {
							if (hex == 8 && str == "Users Identifier") {
								vector<Var>::iterator iter = Variables.begin();
								while (iter != Variables.end() && iter->name != (*(it1 + 1))) {
									iter++;
								}
								if (iter == Variables.end()) {
									vector<vector<string>>::iterator it3 = it + 1;
									while (it3 != Leks.end() && *(it3->begin()) != (*(it1 + 1))) {
										it3++;
									}
									if (it3 == Leks.end()) {
										err.error = "error A2009: Symbol not defined: ";
										err.oper = *(it1 + 1);
										err.str = num;
										error_log.push_back(err);
									}
								}
							}
							else {
								if ( hex!=2 && str == "Text constant") {
									err.error = " error A2050 : Value out of range";
									err.oper = "";
									err.str = num;
									error_log.push_back(err);
								}
							}
						}
					}
				}
			}
			else {
				err.error = "error A2028: Operator expected";
				err.oper = "";
				err.str = num;
				error_log.push_back(err);
			}
		}
	}
	return;
}

void Match(vector<Var>::iterator it5, int num, string str) {
	Err err;
	if (it5 != Variables.end() && it5->type == "L DWORD" && str != "32-bit register") {
			err.error = " warning A4031: Operand types must match";
			err.oper = "";
			err.str = num;
			error_log.push_back(err);
	}
	if (it5 != Variables.end() && it5->type == "L WORD" && str != "16-bit register") {
			err.error = " warning A4031: Operand types must match";
			err.oper = "";
			err.str = num;
			error_log.push_back(err);
	}
	if (it5 != Variables.end() && it5->type == "L BYTE" && str != "8-bit register") {
			err.error = " warning A4031: Operand types must match";
			err.oper = "";
			err.str = num;
			error_log.push_back(err);
	}
}

void PrintVar(ofstream &fin) {
	fin << setw(10) << "Name:" << setw(10) << "Size:" << setw(10) << "Length:\n";
	for (vector<Segm>::iterator it = Segment.begin(); it != Segment.end(); it++) {
		fin << setw(10) << it->name;
		if (it->type == true) {
			fin << setw(10) << "16bit";
		}
		else {
			fin << setw(10) << "32bit";
		}
		int length = it->length;
		fin << setw(5) << "";
		fin << setfill('0') << setw(4) << hex << uppercase << it->length << "\n";
		fin << setfill(' ');
	}

	struct sortVar
	{
		inline bool operator() (const Var& struct1, const Var& struct2)
		{
			return (struct1.name < struct2.name);
		}
	};

	sort(Variables.begin(), Variables.end(), sortVar());

	fin << "\n";
	fin << setw(10) << "Name:" << setw(10) << "Type:" << setw(10) << "Value:" << setw(10) << "Attr:\n";
	for (vector<Var>::iterator it = Variables.begin(); it != Variables.end(); it++) {
		fin << setw(10) << it->name << setw(10) << it->type;
		fin << setw(5) << "";
		fin << setfill('0') << setw(4) << hex << uppercase << it->value << setfill(' ');
		fin << setw(10) << it->attr << "\n";
	}
}