#pragma once
//#include "Global.h"
//#include "Sentence.h"
//#include "ExceptionProcesser.h"
#include <iomanip>
#define vv void
#define sa SyntaxAnalizer
class sa
{
	vector<Sentence> sentences;
	vector<Token>	 tokens;
	vector<string>	 codeSource;

	vector<Segment>		segTable;
	vector<Label>		labelTable;
	map<string, string> assumeTable;

	vv GenerateSentencesStruct();
	vv GenerateJumpOpcodes();
	vv ShowLabelTable();
	vv ShowSegTable();
	vv printToFileLabelTable();
	vv printToFileSegTable();
public:
	sa(vector<Token> _tokens, vector<string> _codeSource);
	~sa();
};
