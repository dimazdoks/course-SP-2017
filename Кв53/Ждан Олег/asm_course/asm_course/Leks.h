#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>//����������� �����
#include <typeinfo>
#include <set>  // ������������ ���� �������� � ��������������
#include <iterator>//��������
#include <vector>
#include <iomanip> //setw() ��������������

using namespace std;

bool LoadFromFile(const char *fileout, const char *filein);

vector<string> LexicalScan(ofstream &fin, string FileCopyLine);
void SyntaxScan(ofstream &fin, vector<string>lex);
string GetType(string s);

