#include "SyntaxAnalizer.h"
#define fr for
#define write cout
#define doky while 
#define enn endl
#define sa SyntaxAnalizer
#define sents sentences
#define stf setfill
sa::sa(vector<Token> _tokens, vector<string> _codeSource) :
	tokens(_tokens),
	codeSource(_codeSource)
{
	initCommandBytes();
	initSegmentReplacePrefixes();
	initRegNumbers();
	initReg16Combinations();

	GenerateSentencesStruct();
	GenerateJumpOpcodes();
	fr (auto iter : sents)
	{
		iter.showSentence();
		iter.printSentenceToFile();
	}
	ShowLabelTable();
	ShowSegTable();
	printToFileLabelTable();
	printToFileSegTable();
}

void sa::GenerateSentencesStruct()
{
	vector<Token> curRow;
	int rowIter = 0;
	int curOffset = 0;
	fr (int i = 0; i < tokens.size() - 1; ++i)
	{
		err.codeRow = codeSource[rowIter + 3];
		err.row = rowIter + 1;
		err.column = 1;

		doky (i < tokens.size() - 1 && tokens[i].lex.row == tokens[i + 1].lex.row)
		{
			curRow.push_back(tokens[i]);
			++i;
		}
		curRow.push_back(tokens[i]);
		sents.push_back(Sentence(curRow,segTable,labelTable,assumeTable));
		sents.back().divideSentence();
		sents.back().generateSentenceAttributes(curOffset);
		curRow.clear();
		++rowIter;
	}
}

void sa::GenerateJumpOpcodes()
{
	fr (auto &iter : sents)
		iter.generateJumpOpcode();
}


void sa::ShowLabelTable()
{
	write << enn << setw(8) << "NAME" << setw(8) << "VALUE" << setw(16) << "SEGMENT NAME" << enn;
	fr (auto iter : labelTable)
		write << stf(' ') << setw(8) << iter.name << setw(4) << " " << stf('0') << setw(4) \
			 << intToHex(to_string(iter.value), 10) << stf(' ') << setw(16) << iter.segName << enn;
}

void sa::ShowSegTable()
{
	write << enn << setw(8) << "NAME" << setw(10) << "LENGTH" << enn;
	fr (auto iter : segTable)
		write << setw(8) << iter.name << setw(6) << " " << stf('0') << setw(4) << intToHex(to_string(iter.length), 10) << stf(' ') << enn;
}

void sa::printToFileLabelTable()
{

	file << enn << setw(8) << "NAME" << setw(8) << "VALUE" << setw(16) << "SEGMENT NAME" << enn;
	fr (auto iter : labelTable)
		file << stf(' ') << setw(8) << iter.name << setw(4) << " " << stf('0') << setw(4) \
		<< intToHex(to_string(iter.value), 10) << stf(' ') << setw(16) << iter.segName << enn;
}

void sa::printToFileSegTable()
{
	file << enn << setw(8) << "NAME" << setw(10) << "LENGTH" << enn;
	fr (auto iter : segTable)
		file << setw(8) << iter.name << setw(6) << " " << stf('0') << setw(4) << intToHex(to_string(iter.length), 10) << stf(' ') << enn;
}

sa::~sa()
{
}