#pragma once

#include <string>
#include <set>
#include <cstdlib>
#include <ctype.h>
#include <iostream>
#include <fstream>

using namespace std;

class Table {
public:
	Table();

	void UsrAdd(const string source);

	struct DataReg {
		set<string> Reg32;
		set<string> Reg16;
		set<string> Reg8;
	};
	DataReg DataReg;
	set<string> SegReg;
	set<string> Command;
	set<string> Direct;
	set<string> OneSymb;
	set<string> User;
};

class Leks: public Table {
public:
	Leks();
	~Leks();
	//methods
	string GetType(const string& source);
	void PrintLeks();
	Leks& operator= (const string& source);
private:
	string str;
	int length;
	string type;
};

class LeksString {
public:
	LeksString();
	~LeksString();
	void PrintString();
	LeksString& operator= (const string& source);
private:
	string name;
	Leks* str2;
	int len;
};
class LeksList {
public:
	LeksList();
	~LeksList();

	void GetFromFile(char* filename);
	void PrintList();
private:
	LeksString* str3;
	int ListSize;
};