#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include "Leks.h"
#include <algorithm>

struct Var {
	string name;
	string type;
	int value;
	string attr;
};

struct Segm {
	string name;
	bool type;
	int length;
};

struct Err {
	int str;
	string error;
	string oper;
};

struct SegR {
	string segm;
	string type;
	string segmr;
};

bool FirstPass(const char *filein);
int MnemVar(int length, int num, int a, string name, vector<SegR> SegRegister, vector<string>::iterator it1, vector<vector<string>>::iterator it);
void CheckingVar(vector<string>::iterator it1, vector<vector<string>>::iterator it, int num, size_t bin, size_t hex, int dec);
void PrintVar(ofstream &fin);
void Match(vector<Var>::iterator it5, int num, string str);

