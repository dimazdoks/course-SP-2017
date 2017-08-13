#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>//подключение файла
#include <typeinfo>
#include <set>  // заголовочный файл множеств и мультимножеств
#include <iterator>//итератор
#include <vector>
#include <iomanip> //setw() форматирование

using namespace std;

bool LoadFromFile(const char *fileout, const char *filein);

vector<string> LexicalScan(ofstream &fin, string FileCopyLine);
void SyntaxScan(ofstream &fin, vector<string>lex);
string GetType(string s);

