#pragma once
//#include "Global.h"
//#include "Utilities.h"
//#include "ExceptionProcesser.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#define la LexicalAnalizer
#define vv vector
class la
{
	vv<Lexem> lexems;
	vv<Token> tokens;
	vv<string> sourceCode;

	void GenerateLexemVector();
	void AnalizeLexems();
	void OutputTokens();
public:
	la() = delete;
	la(string fileName);
	vv<Token>  getTokens();
	vv<string> getSourceCode();
	~la();
};
