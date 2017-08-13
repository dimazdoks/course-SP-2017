#pragma once

#include "Leks.h"

class label
{
public:
	label(string name, bool was): _name(name), _was(was) {};
	void set_bool(bool was) { _was = was; };
	string get_name() { return _name; };
	bool get_was() { return _was; };
	~label() {};
private:
	string _name;
	bool _was;
};

string make_hex(int& Number);
string bin_hex(string Number);

class ZMline
{
public:
	ZMline() = default;
	ZMline(string name, int z, int line) :_name(name), _z(z), ln(line) { hex_val = bin_hex(make_hex(_z)); };
	~ZMline() {};
	string get_name() { return _name; };
	string get_zm() { return hex_val; };
	void s_pass(const string &src) { _sec_pass = src; };
	string get_s_pass() { return _sec_pass; };
	void do_hex(string src) { string tmp = hex_val;  hex_val = src; hex_val+= ' '; hex_val += tmp; };
private:
	string hex_val;
	string _name;
	string _sec_pass;
	int _z;
	int ln;
};

void FirstPass(string& src);
