#include "Leks.h"

Table::Table() {
	DataReg.Reg32.insert("EAX");
	DataReg.Reg32.insert("EBX");
	DataReg.Reg32.insert("ECX");
	DataReg.Reg32.insert("EDX");
	DataReg.Reg32.insert("EBP");
	DataReg.Reg32.insert("ESP");
	DataReg.Reg32.insert("ESI");
	DataReg.Reg32.insert("EDI");
	DataReg.Reg32.insert("EFLAGS");
	DataReg.Reg32.insert("EIP");

	DataReg.Reg16.insert("AX");
	DataReg.Reg16.insert("BX");
	DataReg.Reg16.insert("CX");
	DataReg.Reg16.insert("DX");
	DataReg.Reg16.insert("SI");
	DataReg.Reg16.insert("DI");
	DataReg.Reg16.insert("BP");
	DataReg.Reg16.insert("SP");
	DataReg.Reg16.insert("IP");

	DataReg.Reg8.insert("AH");
	DataReg.Reg8.insert("AL");
	DataReg.Reg8.insert("BH");
	DataReg.Reg8.insert("BL");
	DataReg.Reg8.insert("CH");
	DataReg.Reg8.insert("CL");
	DataReg.Reg8.insert("DH");
	DataReg.Reg8.insert("DL");

	SegReg.insert("CS");
	SegReg.insert("DS");
	SegReg.insert("ES");
	SegReg.insert("FS");
	SegReg.insert("GS");
	SegReg.insert("SS");

	Command.insert("NOP");
	Command.insert("SHL");
	Command.insert("NOT");
	Command.insert("ADC");
	Command.insert("SUB");
	Command.insert("JNC");
	Command.insert("JMP");

	Direct.insert("END");
	Direct.insert("SEGMENT");
	Direct.insert("ENDS");
	Direct.insert("ASSUME");
	Direct.insert("DB");
	Direct.insert("DD");
	Direct.insert("DW");

	OneSymb.insert(":");
	OneSymb.insert(";");
	OneSymb.insert(",");
	OneSymb.insert((string)(char*)34);
	OneSymb.insert("+");
	OneSymb.insert("-");
	OneSymb.insert("*");
	OneSymb.insert("/");
	OneSymb.insert("=");
	OneSymb.insert("(");
	OneSymb.insert(")");
	OneSymb.insert("[");
	OneSymb.insert("]");

	User.insert("_");
	User.insert("@");
	User.insert("$");
	User.insert("?");
};

void Table::UsrAdd(const string source) {
	User.insert(source);
}
//LEKS
Leks::Leks() {
	length = 0;
	str = '\0';
	type = '\0';
};

Leks::~Leks() {
	str.clear();
	type.clear();
	length = 0;
};

void Leks::PrintLeks() {
	printf("%s\n", str.c_str());
	printf("%s\n", type.c_str());
	printf("%i\n", length);
};

Leks& Leks::operator=(const string& source) {
	str = source;
	type = GetType(source);
	length = source.length();
	return *this;
};

string Leks::GetType(const string& source) {
	if (DataReg.Reg32.find(source) != DataReg.Reg32.end())
		return "32 data reg";
	if (DataReg.Reg16.find(source) != DataReg.Reg16.end())
		return "16 data register";
	if (DataReg.Reg8.find(source) != DataReg.Reg8.end())
		return "8 data register";
	if (SegReg.find(source) != SegReg.end())
		return "segment register";
	if (Command.find(source) != Command.end())
		return "command";
	if (Direct.find(source) != Direct.end())
		return "directive";
	if (OneSymb.find(source) != OneSymb.end())
		return "One symbol";
	return NULL;
};

//LeksString

LeksString::LeksString() {
	str2 = NULL;
	len = 0;
}

LeksString::~LeksString() {
	if (str2 != NULL)
		delete[] str2;
	len = 0;
}

void LeksString::PrintString() {
	printf("%s\n", name.c_str());
	for (int i = 0; i < len; i++)
		str2[i].PrintLeks();
}

LeksString& LeksString::operator= (const string& source) {
	int i = 0;
	int j = 0;
	int k = 0;

	string buff;

	set<char> help;
	help.insert(':');
	help.insert(';');
	help.insert(',');
	help.insert(34);
	help.insert('+');
	help.insert('-');
	help.insert('*');
	help.insert('/');
	help.insert('=');
	help.insert('(');
	help.insert(')');
	help.insert('[');
	help.insert(']');

	set<char> antitab;
	antitab.insert('\t');

	name = source;

	while (i < source.length()) {
		if ((source[i] == ' ') || (help.find(source[i]) != help.end()))
			len++;
		i++;
	};
	i = 0;
	//Here runs exception "_NFirst was 0x22".
	str2 = new Leks[len + 1]();
	// In static size_t __CLRCALL_OR_CDECL length(const _Elem *_First) in iosfwd.
	while (i < source.length()) {
		if ((source[i] == ' ') || (i == source.length() - 1)) {
			str2[k] = buff;
			k++;
			j = 0;
			buff.clear();
		}
		else if (help.find(source[i]) != help.end()) {
			str2[k] = buff;
			k++;
			buff.clear();
			buff += source[i];
			str2[k] = buff;
			k++;
			j = 0;
			buff.clear();
		}
		else if (antitab.find(source[i]) != antitab.end())
			i++;
		else {
			buff[j] = source[i];
			i++;
		}
	};
	return *this;
}

//LeksList
LeksList::LeksList() {
	str3 = NULL;
	ListSize = 0;
}

LeksList::~LeksList() {
	if (str3 != NULL)
		delete[] str3;
	ListSize = 0;
}

void LeksList::GetFromFile(char* filename) {
	int i = 0;
	ifstream asmfile(filename, ios_base::in);
	string s;
	if (!asmfile.is_open())
		exit(-1);
	while (!asmfile.eof()) {
		getline(asmfile, s);
		ListSize++;
	};
	asmfile.clear();
	asmfile.seekg(0, ios::beg);
	//That's what I've meant about construction. However when i looked with "step into" i can't tell the difference between using new <type>[] and new <type>[]().
	str3 = new LeksString[ListSize]();
	//<id> = new <type>[<size>]();
	while ((!asmfile.eof()) && (i < ListSize)) {
		getline(asmfile, s);
		str3[i] = s;
		i++;
	};
	asmfile.close();
}

void LeksList::PrintList() {
	for (int i = 0; i < ListSize; i++) 
		str3[i].PrintString();
}