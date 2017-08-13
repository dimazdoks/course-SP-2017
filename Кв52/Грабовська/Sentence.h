#pragma once
//#include "Global.h"
#include <iostream>
#include <iomanip>
#include <fstream>
//#include "Utilities.h"
//#include "ExceptionProcesser.h"
#include "Operand.h"
#define str string
#define vv void
#define sent Sentence
#define array1 vector
class sent
{
	array1<Token>		wholeSentence;
	array1<Segment>		&segTable;
	array1<Label>		&labelTable;
	map<str, str> &assumeTable;

	Token			labelOrName;
	Token		    command;
	array1<Operand> operands;
	int    sentenceSize;
	int	   curOffset;
	int	   displacepmentSize;
	str sentenceSegment;

	///////////////////
	str commandCode;
	str modRM;
	str byteSIB;
	str segChangePref;
	str addrModeChangePref_Operand;
	str addrModeChangePref_Address;
	str imm;
	str displacepment;
	str jmpOffset;
	///////////////////

	str generateCommandWithOpType(str);
	vv   updateLabelAndSegmentTables(int &curGlobalOffset);
public:
	sent(array1<Token> _wholeSentence, array1<Segment> &_segTable, array1<Label> &_labelTable, map<str, str> &_assumeTable);

	vv divideSentence();
	vv generateSentenceAttributes(int &curGlobalOffset);
	vv generateJumpOpcode();
	vv showSentence();
	vv printSentenceToFile();

	~sent();
};
