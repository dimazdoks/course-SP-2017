#include "Leks.h"

vector<string> AsmCod;
vector<vector<string>> Leks;

bool LoadFromFile(const char *fileout, const char *filein) {

	string FileCopyLine;
	fstream fout(fileout);
	ofstream fin(filein);
	int StrNum = 0;
	if (!fout) {
		cout << "OutputFile opening error\n";
		return false;
	}
	if (!fin) {
		cout << "InputFile opening error\n";
		return false;
	}

	while (getline(fout, FileCopyLine)) {
		int i = 0;
		while (FileCopyLine[i] == '\t' || FileCopyLine[i] == ' ') {
			i++;
		}
		if (FileCopyLine[i] != NULL) {
			AsmCod.push_back(FileCopyLine);
		}
		FileCopyLine.erase(0, i);

		if (FileCopyLine != "") {
			FileCopyLine += ' ';
			StrNum++;
			//cout << FileCopyLine << "\n";
			fin << "String[" << StrNum << "]: " << FileCopyLine << "\n";
			fin << setw(10) <<"Num:"<< setw(20)<<"Leksem:"<< setw(15)<<"Length:"<< setw(35)<<"Type:\n";
			SyntaxScan(fin, LexicalScan(fin, FileCopyLine));
		}
	}
	return true;
}

vector<string> LexicalScan(ofstream &fin, string FileCopyLine) {
	set<char> symbol;
	symbol.insert(':');
	symbol.insert(';');
	symbol.insert(',');
	symbol.insert('+');
	symbol.insert('-');
	symbol.insert('*');
	symbol.insert('/');
	symbol.insert('=');
	symbol.insert('(');
	symbol.insert(')');
	symbol.insert('[');
	symbol.insert(']');

	int Num = 0;
	int i = 0;
	vector<string> lex;

	while ((i = FileCopyLine.find(" ")) != std::string::npos) {
		int j = -1;
		while (j <= i - 2) {
			j++;
			if (symbol.find(FileCopyLine[j]) != symbol.end()) {
				if (j != 0) {
					Num++;
					fin << setw(10) << Num << setw(20) << FileCopyLine.substr(0, j) << setw(15) << j << setw(35) << GetType(FileCopyLine.substr(0, j)) <<"\n";
					lex.push_back(FileCopyLine.substr(0, j));
					FileCopyLine.erase(0, j);
				}
				i -= (j + 1);
				Num++;
				fin << setw(10) << Num << setw(20) << FileCopyLine.substr(0, 1) << setw(15) << 1 << setw(35) << "Symbol\n";
				lex.push_back(FileCopyLine.substr(0, 1));
				FileCopyLine.erase(0, 1);
				j = -1;
			}
		}
		if (i == 0) {
			FileCopyLine.erase(0, 1);
		}
		else {
			Num++;
			fin << setw(10) << Num << setw(20) << FileCopyLine.substr(0, i) << setw(15) << i << setw(35) <<GetType(FileCopyLine.substr(0, i)) << "\n";
			lex.push_back(FileCopyLine.substr(0, i));
			FileCopyLine.erase(0, i + 1);
		}
	}
	fin << "\n";
	return lex;
}

string GetType(string s) {
	char* Mnemocod[] = { "RET","POP","PUSH","AND","MOV","XCHG","JNE","CALL","INC","FAR","PTR","JL"};
	char* Direc[] = { "END","SEGMENT","ENDS","PROC","ENDP","ASSUME","USE16"};
	char* DataDirec[] = { "DB","DD","DW"};
	char* Reg32[] = { "EAX", "EBX", "ECX", "EDX", "EBP", "ESP", "ESI", "EDI", "EFLAGS", "EPI" };
	char* Reg16[] = { "AX", "BX", "CX", "DX", "BP", "SP", "SI", "DI", "IP" };
	char* Reg8[] = { "AL","AH","BL","BH","CL","CH","DL","DH" };
	char* SegReg[] = { "CS","DS","GS","ES","SS","FS"};
	for (int i = 0; i <12; i++) {
		if (s == Mnemocod[i]) {
			return "Mnemocod of machine instruction";
		}
	}
	for (int i = 0; i < 7; i++) {
		if (s == Direc[i]) {
			return "Directive";
		}
	}
	for (int i = 0; i < 3; i++) {
		if (s == DataDirec[i]) {
			switch (i) {
			case 0: return "Directive data type 1";
			case 1: return "Directive data type 4";
			case 2:	return "Directive data type 2";
			}
		}
	}
	for (int i = 0; i < 10; i++) {
		if (s == Reg32[i]) {
			return "32-bit register";
		}
	}
	for (int i = 0; i < 9; i++) {
		if (s == Reg16[i]) {
			return "16-bit register";
		}
	}
	for (int i = 0; i < 8; i++) {
		if (s == Reg32[i]) {
			return "8-bit register";
		}
	}
	for (int i = 0; i < 6; i++) {
		if (s == SegReg[i]) {
			return "Segment register";
		}
	}
	if (s[0] == '\'' && s[s.size()-1] == '\'') {
		return "Text constant";
	}
	int flag = 0;
	for (size_t i = 0; i < (s.size() - 1); i++) {
		if ((((int)s[i] >= '0') && ((int)s[i] <= '9')) || (((int)s[i] >= 'A') && ((int)s[i] <= 'F')))
			flag++;
	}
	if (flag == s.size() - 1) {
		if (s[s.size() - 1] == 'B' || s[s.size() - 1] == 'b')
			return "Binary constant";
		if (s[s.size() - 1] == 'H' || s[s.size() - 1] == 'h')
			return "Hexadecimal constant";
		if (((int)s[0] >= '0') && ((int)s[0] <= '9'))
		return "Decimal constant";
	}
	return "Users Identifier";
}

void SyntaxScan(ofstream &fin, vector<string>lex) {
	Leks.push_back(lex);
	set<string> type;
	type.insert("Directive");
	type.insert("Mnemocod of machine instruction");
	type.insert("Directive data type 1");
	type.insert("Directive data type 2");
	type.insert("Directive data type 4");
	
	struct MyStruct {
		int NumLeks;
		int Sum;
	};
	MyStruct Cell;
	vector<string>::iterator iter;
	vector<MyStruct> NumSum;

	bool Lexem = false;
	int Num = 0;
	Cell.Sum = 0;

	iter = lex.begin();
	
	if (GetType(*iter) == "Users Identifier") {
		Num++;

		Lexem = true;
		iter++;
		if (*iter == ":") {
			fin << setw(16) << "Label1 (Name)" << setw(26) << "Mnemocod" << setw(35) << "Operand ¹ " << 1 << "\n";
			fin << setw(13) << "¹ Lexem" << setw(23) << "¹/¹ first Lexem" << setw(15) << "Number Lexem";
			fin << setw(20) << "¹/¹ first Lexem" << setw(15) << "Number Lexem" << "\n";
			fin << setw(11) << Num;
			for (int i = 0; i < 2; i++) {
				fin << setw(20) << "--" << setw(15) << "--";
			}
			fin << "\n\n";
			return;
		}
	}

	Cell.NumLeks = Num+1;

		while (iter != lex.end() && type.find(GetType(*iter)) != type.end()) {
			Num++;
			Cell.Sum++;
			iter++;	
		}

		NumSum.push_back(Cell);

		Cell.NumLeks = Num+1;

		while (iter != lex.end()) {
			Cell.Sum = 0;
			Cell.NumLeks = Num+1;
			while (iter != lex.end() && *iter != ",") {
				Cell.Sum++;
				Num++;
				iter++;
			}
			Num++;
			NumSum.push_back(Cell);
			if (iter != lex.end()) {
				iter++;
			}
		}

		if (NumSum.size() < 3) {
			fin << setw(16) << "Label1 (Name)" << setw(26) << "Mnemocod" << setw(35) << "Operand ¹ "<< 1 << "\n";
			fin << setw(13) << "¹ Lexem" << setw(23) << "¹/¹ first Lexem" << setw(15) << "Number Lexem";
			fin << setw(20) << "¹/¹ first Lexem" << setw(16) << "Number Lexem\n";
			
			if(Lexem == true){
			fin << setw(11) << 1;
			}
			else {
			fin << setw(11) << "--";
			}
			for (vector<MyStruct>::iterator it = NumSum.begin(); it != NumSum.end(); it++) {
				fin<< setw(20) << it->NumLeks << setw(15) << it->Sum;
			}
			if (NumSum.size() != 2) {
					fin << setw(20) << "--" << setw(14) << "--";			
			}
		}
		else
		{
			fin << setw(16) << "Label1 (Name)" << setw(26) << "Mnemocod" << setw(34) << "Operand ¹ " << 1;
			for (size_t i = 2; i < NumSum.size(); i++) {
				fin<< setw(35) << "Operand ¹ "<< i;
			}
			fin <<"\n"<< setw(13) << "¹ Lexem" << setw(23) << "¹/¹ first Lexem" << setw(15) << "Number Lexem" << setw(20) << " ¹/¹ first Lexem" << setw(15) << "Number Lexem";
			for (size_t i = 2; i < NumSum.size(); i++) {
				fin << setw(20) << " ¹/¹ first Lexem" << setw(15) << "Number Lexem";
			}
			fin << "\n";
			if (Lexem == true) {
				fin << setw(11) << 1;
			}
			else {
				fin << setw(11) << "--";
			}
			for (vector<MyStruct>::iterator it = NumSum.begin(); it != NumSum.end(); it++) {
				fin << setw(20) << it->NumLeks << setw(15) << it->Sum;
			}
			fin << "\n";
		}

		fin<<"\n\n";
	return;
}