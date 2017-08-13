#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#define ERR -1

#define EMPTY 0

#define REG 10
#define REG32 15
#define REG8 18

#define DIR 20

#define SREG 30

#define COM 40

#define CONST 50
#define CONSTB 51
#define CONSTD 52
#define CONSTH 53

#define OS 60

#define UID 70
#define LABEL 74
#define SEG_NAME 75
#define VAR_NAME 76
#define MEM 80

using namespace std;

class Unary_Leksem
{
public:
	Unary_Leksem() = default;
	Unary_Leksem(string name, int type, int size, int number) : _name(name), _type(type), _size(size), _number(number) {};
	string get_name() { return _name; };
	int get_type() { return _type; };
	int get_number() { return _number; };
	int get_size() { return _size; };
	void set_type(int type) { _type = type; };
	~Unary_Leksem() = default;
private:
	string _name;
	int _type;
	int _number;
	int _size;
};

typedef struct EQU
{
	string name;
	string value;
	//string HexValue;
};

class error
{
public:
	error() = default;
	error(string name, string what, int line) : _name(name), _what(what), _place(line) {};
	~error() = default;
	int _place;
	string _name;
	string _what;
};

string GetAnalyzedString(ifstream src);

void div_to_unary_leksems(string& src, int& num);

bool check_alphabet(const char is_checked);

bool check_equ(vector<EQU*>	equ_list, string& tmp);

bool check_binary_number(const string& src);

bool check_hex_number(const string& src);

bool check_dec_number(const string& src);

bool check_one_symbol(char is_checked);

void make_leksem(string& src, int &num);

bool check_for_uid(string& src);

int get_leks_type(const string& src);