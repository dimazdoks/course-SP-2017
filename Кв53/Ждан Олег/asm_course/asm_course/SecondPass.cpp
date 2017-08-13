#include "SecondPass.h"
#include <stdio.h>

extern vector<vector<string>> Leks;
extern vector<string> AsmCod;
extern vector<Var>Variables;
extern vector<Segm> Segment;
extern vector<Err> error_log;
extern vector<int> offset;
extern vector<SegR> SegRegister;

bool SecondPass(const char *filein) {
	ofstream fin(filein);
	if (!fin) {
		cout << "InputFile opening error\n";
		return false;
	}
	int num = 0;
	string SegmName;
	vector<string> bytecod;
	vector<string>::iterator it1,iter;
	vector<string>::iterator it2 = AsmCod.begin();
	vector<int>::iterator it3 = offset.begin();

	for (vector<vector<string>>::iterator it = Leks.begin(); it != Leks.end(); it++) {
		num++;
		it1 = find(it->begin(), it->end(), "SEGMENT");
		if (it1 != it->end()) {
			SegmName = *(it1-1);
		}

		it1 = find(it->begin(), it->end(), "DB");
		if (it1 != it->end()) {
			if (*(it3+1)-*(it3)>1){
				string str = *(it1 + 1);
				for (int i = 1; i <= (*(it3 + 1)); i++) {
					char a = str[i];
					char str1[3] = "0";
					_itoa(int(a), str1, 16);
					bytecod.push_back(str1);
				}
			}
			else {
				string type = GetType(*(it1 + 1));
				char str[16];
				string str1 = "00";
				int i;
				strcpy(str, (*(it1 + 1)).c_str());
				if (type == "Decimal constant") {
					sscanf(str, "%d", &i);
					sprintf(str, "%X", i);
				}
				else {
					if (type == "Hexadecimal constant") {
						sscanf(str, "%x", &i);
						sprintf(str, "%X", i);
					}
					else {
						if (type == "Binary constant") {
							int a = 0, b = 1;

							bool flag = false;
							for (int i = (*(it1 + 1)).size() - 2; i != -1; i--) {
								if (str[i] == '1') {
									a += b;
								}
								b = b * 2;
							}
							sprintf(str, "%X", a);
						}
					}
				}
				str1 += str;
				size_t l = str1.length();
				bytecod.push_back(str1.substr(l - 2, l));
			}
		}

		it1 = find(it->begin(), it->end(), "DW");
		if (it1 != it->end()) {
			string type = GetType(*(it1 + 1));
			char str[16];
			string str1 = "0000";
			int i;
			strcpy(str, (*(it1 + 1)).c_str());
			if (type == "Decimal constant") {
				sscanf(str, "%d", &i);
				sprintf(str, "%X", i);
			}
			else {
				if (type == "Hexadecimal constant") {
					sscanf(str, "%X", &i);
					sprintf(str, "%X", i);
				}
				else {
					if (type == "Binary constant") {
						int a = 0, b = 1;

						bool flag = false;
						for (int i = (*(it1 + 1)).size() - 2; i != -1; i--) {
							if (str[i] == '1') {
								a += b;
							}
							b = b * 2;
						}
						sprintf(str, "%X", a);
					}
				}
			}
			str1 += str;
			size_t l = str1.length();
			bytecod.push_back(str1.substr(l - 4, l));
		}
		

		it1 = find(it->begin(), it->end(), "DD");
		if (it1 != it->end()) {
			string type = GetType(*(it1 + 1));
			if (type == "Users Identifier") {
				vector<Var>::iterator iter = Variables.begin();
				while (iter != Variables.end() && iter->name != (*(it1 + 1))) {
					iter++;
				}
				string str1="0000000";
				char str[16];
				sprintf(str, "%X", iter->value);
				str1 += str;
				size_t l = str1.length();
				bytecod.push_back(str1.substr(l - 8, l));
				bytecod.push_back("R");

			}
			else {
				char str[16];
				string str1;
				int i;
				strcpy(str, (*(it1 + 1)).c_str());
				if (type == "Decimal constant") {
					sscanf(str, "%d", &i);
					sprintf(str, "%X", i);
				}
				else {
					if (type == "Hexadecimal constant") {
						sscanf(str, "%X", &i);
						sprintf(str, "%X", i);
					}
					else {
						if (type == "Binary constant") {
							int a = 0, b = 1;

							bool flag = false;
							for (int i = (*(it1 + 1)).size() - 2; i != -1; i--) {
								if (str[i] == '1') {
									a += b;
								}
								b = b * 2;
							}
							sprintf(str, "%X", a);
						}
					}
				}
				str1 += str;
				string str2;

				for (int i = str1.size() - 1; i >= 0; i = i - 2) {
					if (i >= 1) {
						str2 += str1[i - 1];
					}
					else { str2 += "0"; }
					str2 += str1[i];
				}
				str2 += "0000000";

				bytecod.push_back(str2.substr(0, 8));
			}
		}

		it1 = find(it->begin(), it->end(), "INC");
		if (it1 != it->end()) {
		string type = GetType(*(it1 + 1));
		if (type == "16-bit register") {
			int a = 64 + RegN(*(it1 + 1));
			char s[3];
			sprintf(s, "%X", a);
			bytecod.push_back(s);
		}
		if (type == "32-bit register") {
			int a = 64 + RegN(*(it1 + 1));
			char s[3];
			sprintf(s, "%X", a);
			bytecod.push_back("66|");
			bytecod.push_back(s);
		}
		}
		it1 = find(it->begin(), it->end(), "POP");
		if (it1 != it->end()) {
			string type = GetType(*(it1 + 1));
			if (type == "16-bit register") {
				int a = 88 + RegN(*(it1 + 1));
				char s[3];
				sprintf(s, "%X", a);
				bytecod.push_back(s);
			}
			if (type == "32-bit register") {
				int a = 88 + RegN(*(it1 + 1));
				char s[3];
				sprintf(s, "%X", a);
				bytecod.push_back("66|");
				bytecod.push_back(s);
			}
		}

		it1 = find(it->begin(), it->end(), "PUSH");
		if (it1 != it->end()) {
			string type = GetType(*(it1 + 1));
			vector<Var>::iterator iter = Variables.begin();
			string str="000";
			char str1[4];
			if (type == "Users Identifier") {
				while (iter != Variables.end() && iter->name != (*(it1 + 1))) {
					iter++;
				}
				if (iter != Variables.end()) {
					vector<SegR>::iterator iter1 = SegRegister.begin();
					while (iter1 != SegRegister.end() && (iter1->segm != SegmName || iter1->type != iter->attr)) {
						iter1++;
					}
					if (iter1->segmr == "GS") {
						bytecod.push_back("65:");
					}
					if (iter1->segmr == "FS") {
						bytecod.push_back("64:");
					}
					if (iter1->segmr == "ES") {
						bytecod.push_back("26:");
					}
					if (iter->type == "L DWORD" && *(it1) != "CALL") {
						bytecod.push_back("66|");
					}
					bytecod.push_back("FF");
					it1 += 2;
					if (*it1 == "[") {
						int a =128 + VarN(*(it1 + 1))+6*8;
						char s[3];
						sprintf(s, "%X", a);
					bytecod.push_back(s);
					}
					else {
					bytecod.push_back("06");
					}
				}
				sprintf(str1, "%X", iter->value);
				str += str1;
				size_t l = str.length();
				bytecod.push_back(str.substr(l - 4, l));
				bytecod.push_back("R");
			}
		}

		it1 = find(it->begin(), it->end(), "AND");
		if (it1 != it->end()) {
			string type = GetType(*(it1 + 1));
			vector<Var>::iterator iter = Variables.begin();
			string str = "000";
			char str1[4];
			if (type == "Users Identifier") {
				while (iter != Variables.end() && iter->name != (*(it1 + 1))) {
					iter++;
				}
				if (iter != Variables.end()) {
					vector<SegR>::iterator iter1 = SegRegister.begin();
					while (iter1 != SegRegister.end() && (iter1->segm != SegmName || iter1->type != iter->attr)) {
						iter1++;
					}
					if (iter1->segmr == "GS") {
						bytecod.push_back("65:");
					}
					if (iter1->segmr == "FS") {
						bytecod.push_back("64:");
					}
					if (iter1->segmr == "ES") {
						bytecod.push_back("26:");
					}
					if (iter->type == "L DWORD" && *(it1) != "CALL") {
						bytecod.push_back("66|");
					}
					bytecod.push_back("21");
					it1 += 2;
					if (*it1 == "[") {
						int a = 128 + VarN(*(it1 + 1));
						it1 = find(it->begin(), it->end(), ",");
						a += RegN(*(it1+1)) * 8;
						char s[3];
						sprintf(s, "%X", a);
						bytecod.push_back(s);
					}
				}
				sprintf(str1, "%X", iter->value);
				str += str1;
				size_t l = str.length();
				bytecod.push_back(str.substr(l - 4, l));
				bytecod.push_back("R");
			}
	
		}

		it1 = find(it->begin(), it->end(), "MOV");
		if (it1 != it->end()) {
			vector<string>::iterator it2 = it1;
			it1 = find(it->begin(), it->end(), ",");
			string type = GetType(*(it1 + 1));
			vector<Var>::iterator iter = Variables.begin();
			string str = "000";
			char str1[4];
			if (type == "Users Identifier") {
				while (iter != Variables.end() && iter->name != (*(it1 + 1))) {
					iter++;
				}
				if (iter != Variables.end()) {
					vector<SegR>::iterator iter1 = SegRegister.begin();
					while (iter1 != SegRegister.end() && (iter1->segm != SegmName || iter1->type != iter->attr)) {
						iter1++;
					}
					if (iter1->segmr == "GS") {
						bytecod.push_back("65:");
					}
					if (iter1->segmr == "FS") {
						bytecod.push_back("64:");
					}
					if (iter1->segmr == "ES") {
						bytecod.push_back("26:");
					}
					if (iter->type == "L DWORD" && *(it1) != "CALL") {
						bytecod.push_back("66|");
					}
					bytecod.push_back("8B");
					it1 += 2;
					if (*it1 == "[") {
						int a = 128 + VarN(*(it1 + 1));
						a += RegN(*(it2 + 1)) * 8;
						char s[3];
						sprintf(s, "%X", a);
						bytecod.push_back(s);
					}
				}
				sprintf(str1, "%X", iter->value);
				str += str1;
				size_t l = str.length();
				bytecod.push_back(str.substr(l - 4, l));
				bytecod.push_back("R");
			}
		}

		it1 = find(it->begin(), it->end(), "XCHG");
		if (it1 != it->end()) {
			string type = GetType(*(it1 + 1));
			if (*(it1 + 1) == "AX" || *(it1 + 1) == "EAX") {
				it1 = find(it->begin(), it->end(), ",");
				int a = 144 + RegN(*(it1 + 1));
				char s[3];
				sprintf(s, "%X", a);
				bytecod.push_back(s);
			}
			else {
				iter = (it1);
				int a = RegN(*(it1 + 1));
				it1 = find(it->begin(), it->end(), ",");
				if (*(it1 + 1) == "AX" || *(it1 + 1) == "EAX") {
					it1 = find(it->begin(), it->end(), ",");
					a += 144;
					char s[3];
					sprintf(s, "%X", a);
					bytecod.push_back(s);
				}
				else {
					a = 192 + RegN(*(it1 + 1)) + RegN(*(iter + 1))*8;
					bytecod.push_back("87");
					char s[3];
					sprintf(s, "%X", a);
					bytecod.push_back(s);
				}
			}
			
		}

		it1 = find(it->begin(), it->end(), "CALL");
		if (it1 != it->end()) {
			if (*(it1 + 1) == "FAR" && *(it1 + 2) == "PTR") {
				it1 += 2;
				bytecod.push_back("9A");
				vector<Var>::iterator iter = Variables.begin();
				string str = "000";
				char str1[4];
				while (iter != Variables.end() && iter->name != (*(it1 + 1))) {
					iter++;
				}
				sprintf(str1, "%X", iter->value);
				str += str1;
				size_t l = str.length();
				bytecod.push_back(str.substr(l - 4, l));
				bytecod.push_back("----");
				bytecod.push_back("R");
			}
			else {
				vector<Var>::iterator iter = Variables.begin();
				string str = "000";
				char str1[4];
				while (iter != Variables.end() && iter->name != (*(it1 + 1))) {
					iter++;
				}
				if (iter != Variables.end()) {
					vector<SegR>::iterator iter1 = SegRegister.begin();
					while (iter1 != SegRegister.end() && (iter1->segm != SegmName || iter1->type != iter->attr)) {
						iter1++;
					}
					if (iter1->segmr == "GS") {
						bytecod.push_back("65:");
					}
					if (iter1->segmr == "FS") {
						bytecod.push_back("64:");
					}
					if (iter1->segmr == "ES") {
						bytecod.push_back("26:");
					}
					if (iter->type == "L DWORD" && *(it1) != "CALL") {
						bytecod.push_back("66|");
					}
					bytecod.push_back("FF");
					it1 += 2;
					if (*it1 == "[") {
						int a = 128 + VarN(*(it1 + 1)) + 3 * 8;
						char s[3];
						sprintf(s, "%X", a);
						bytecod.push_back(s);
					}
					else {
						bytecod.push_back("1E");
					}
				}
				sprintf(str1, "%X", iter->value);
				str += str1;
				size_t l = str.length();
				bytecod.push_back(str.substr(l - 4, l));
				bytecod.push_back("R");
			}
		}

		it1 = find(it->begin(), it->end(), "JNE");
		if (it1 != it->end()) {
			bytecod.push_back("75");
			bytecod.push_back("E2");
		}

		it1 = find(it->begin(), it->end(), "JL");
		if (it1 != it->end()) {

			vector<Var>::iterator iter = Variables.begin();
			string str = "000";
			char str1[4];
			while (iter != Variables.end() && iter->name != (*(it1 + 1))) {
				iter++;
			}
			if (iter->value > *it3) {
				
				bytecod.push_back("0F");
				bytecod.push_back("8C");
				sprintf(str1, "%X", iter->value);
				str += str1;
				size_t l = str.length();
				bytecod.push_back(str.substr(l - 4, l));

				bytecod.push_back("R");
			}
			else {
				bytecod.push_back("7C");
				string str;
				int a=255-iter->value;
				sprintf(str1, "%X", a);
				bytecod.push_back(str1);
			}
			
		}

		it1 = find(it->begin(), it->end(), "RET");
		if (it1 != it->end()) {
			bytecod.push_back("C3");
		}

		it1 = it->begin();
		if (*it1 != "ASSUME" && *it1 != "END") {
			fin << setfill('0') << setw(4) << hex << uppercase << *it3 <<" ";
			for (vector<string>::iterator iter = bytecod.begin(); iter != bytecod.end(); iter++) {
				fin << hex << uppercase << *iter << " ";
			}
			fin << setfill(' ');
			fin << setw(16) << "" << *it2 << "\n";
		}
		else {
			fin << setw(20) << "" << *it2 << "\n";
		}

		for (vector<Err>::iterator it1 = error_log.begin(); it1 != error_log.end(); it1++) {
			if (it1->str == num) {
				fin << "Str[" << dec << it1->str << "]:" << it1->error << it1->oper << "\n";
			}
		}
		bytecod.clear();
		it2++;
		it3++;
	}


	PrintVar(fin);
	return true;
}

int RegN(string str) {
	if (str == "AL" || str == "AX" || str == "EAX") {
		return 0;
	}
	if (str == "CL" || str == "CX" || str == "ECX") {
		return 1;
	}
	if (str == "DL" || str == "DX" || str == "EDX") {
		return 2;
	}
	if (str == "BL" || str == "BX" || str == "EBX") {
		return 3;
	}
	if (str == "AH" || str == "SP" || str == "ESP") {
		return 4;
	}
	if (str == "CH" || str == "BP" || str == "EBP") {
		return 5;
	}
	if (str == "DH" || str == "SI" || str == "ESI") {
		return 6;
	}
	if (str == "BH" || str == "DI" || str == "EDI") {
		return 7;
	}
	return 0;
}
int VarN(string str) {
		if (str == "SI") {
			return 4;
		}
		if (str == "DI") {
			return 5;
		}
		if (str == "BP") {
			return 6;
		}
		if (str == "BX") {
			return 7;
		}
	return 0;
}