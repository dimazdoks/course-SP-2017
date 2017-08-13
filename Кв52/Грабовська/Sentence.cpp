#include "Sentence.h"
#define cm command
#define fr for
#define yak if
#define orr else
#define Rech Sentence
#define allrech wholeSentence
#define cc commandCode
#define ff file
#define array1 vector
#define write cout 
#define stroka string
#define ss sentenceSize
#define gLS getLabelSegment
Rech::Rech(array1<Token> _wholeSentence, array1<Segment> &_segTable, array1<Label> &_labelTable, map<string,stroka> &_assumeTable):\
		allrech(_wholeSentence),
		segTable(_segTable),
		labelTable(_labelTable),
		assumeTable(_assumeTable)
{
	cm.lex.text = "";
	labelOrName.lex.text = "";
	cc = "";
	modRM = "";
	byteSIB = "";
	segChangePref = "";
	addrModeChangePref_Operand = "";
	addrModeChangePref_Address = "";
	imm = "";
	displacepment = "";
	jmpOffset = "";
	displacepmentSize = 0;
}

string Rech::generateCommandWithOpType(string command)
{  
	
	fr (auto iter : operands)
	{
		yak (iter.type == OpType::OPREG8)
			command += "_reg8";
		orr yak (iter.type == OpType::OPREG16 || iter.type == OpType::OPREG32)
			command += "_reg16-32";
		orr yak (iter.type == OpType::MEM)
			command += "_mem";
		orr yak (iter.type == OpType::IMM)
			command += "_imm";
	}
	return command;
}

void Rech::divideSentence()
{
	int i = 0;
	yak (allrech[0].lexType == LexType::USER_IDENT)
	{
		labelOrName = allrech[0];
		++i;
		yak (allrech.size() > 1 && allrech[i].lexType == LexType::SINGLE_SYMB)
			++i;
	}

	yak (((allrech.size() > i) && (allrech[i].lexType == LexType::DIRECTIVE || allrech[i].lexType == LexType::COMMAND))\
		|| (allrech.size() == 1 && allrech[0].lexType == LexType::COMMAND))
	{
		command = allrech[i];
		++i;
	}

	array1<Token> curOperand;
	while (i < allrech.size())
	{
		curOperand.clear();
		while (i < allrech.size() && allrech[i].lex.text != ",")
		{
			curOperand.push_back(allrech[i]);
			++i;
		}
		operands.push_back(Operand(curOperand));
		++i;
	}
}

void Rech::updateLabelAndSegmentTables(int &curGlobalOffset)
{
	Label tempLabel;
	tempLabel.value = 0;

	yak (command.lex.text != "SEGMENT" && command.lex.text != "ENDS" && labelOrName.lex.text != "" \
	&& (command.lex.text == "" || isDirective(command.lex.text)))
	{
		//check repeating labels in table
		fr (auto l_iter : labelTable)
			yak (l_iter.name == labelOrName.lex.text)
			{
				tempLabel.value = -1;
				break;
			}

		yak (tempLabel.value != -1)
		{
			tempLabel.name = labelOrName.lex.text;
			tempLabel.value = curGlobalOffset;
			tempLabel.segName = curSeg.name;

			labelTable.push_back(tempLabel);
		}
	}

	yak (command.lex.text == "SEGMENT")
	{
		curSeg.name = labelOrName.lex.text;
		sentenceSegment = curSeg.name;
	}
	orr yak (command.lex.text == "ENDS")
	{
		curSeg.length = curGlobalOffset;
		segTable.push_back(curSeg);
		curSeg.name = "";
		curGlobalOffset = 0;
	}
}

void Rech::generateSentenceAttributes(int &curGlobalOffset)
{
	yak (command.lex.text.empty() && !operands.empty())
	{
		ERROR << "Unknown command";
		return;
	}

	ss = 0;
	yak (command.lex.text == "SEGMENT" || command.lex.text == "ENDS")
	{}
	orr yak (command.lex.text == "ASSUME")
	{
		fr (auto iter : operands)
			assumeTable.insert(std::pair<string,stroka>(iter.operand[2].lex.text, iter.operand[0].lex.text));
	}
	orr yak (command.lex.text == "DD")
		ss = 4;
	orr yak (command.lex.text == "DW")
		ss = 2;
	orr yak (command.lex.text == "DB")
	{
		yak (operands.size() != 0 && operands[0].type != OpType::TXT)
			ss = 1;
	}
	orr yak (command.lex.text == "JC")
	{
		yak (checkIsInLabelTable(operands[0].operand[0].lex.text, labelTable))
		{
			cc = getCommandCode("JC_islabel");
			++ss;
		}
		orr
		{
			cc = getCommandCode("JC_nolabel");
			ss += 2;
		}

		ss += cc.size() / 2;
	}
	orr yak (command.lex.text == "JMP")
	{
		cc = getCommandCode("JMP");
		yak (!checkIsInLabelTable(operands[0].operand[0].lex.text, labelTable))
		{
			displacepment = "90";
			++ss;
		}
		ss += 1; //jmp code size
		++ss; //label offset
	}
	orr yak (command.lex.text != "")
	{
		cc   = getCommandCode(generateCommandWithOpType(command.lex.text));
		ss += getCommandSize(generateCommandWithOpType(command.lex.text));
	}

	fr (auto iter : operands)
	{
		yak (iter.type == OpType::TXT)
		{
			ss += iter.operand[0].lex.text.size() - 2; //text constant size - 2 quotes
			imm = stringToHex(iter.operand[0].lex.text);
		}
		/*orr yak (iter.type == OPREG16 || iter.type == OPREG32)
			++ss; // addr mode change pref*/
		orr yak (iter.type == OpType::IMM)
		{
			yak(command.lex.text != "SHL")
				switch (iter.operand[0].lexType)
				{
				case LexType::BIN_CONST:
					imm = intToHex(iter.operand[0].lex.text, 2);
					break;
				case LexType::DEC_CONST:
					imm = intToHex(iter.operand[0].lex.text, 10);
					break;
				case LexType::HEX_CONST:
					imm = setReversedByteSequence(iter.operand[0].lex.text, ss); //x32
					break;
				default:
					break;
				}

		}
		orr yak (iter.type == OpType::MEM)
		{
			
			fr (int i = 0; i < iter.operand.size(); ++i)
			{
				err.column = iter.operand[i].lex.column + 1;
				yak (!curSeg.name.empty() && iter.operand[i].lexType == LexType::SEG_REG && iter.operand[i + 1].lex.text == ":")
				{
					fr(auto label_iter : labelTable)
						yak (label_iter.name == iter.operand[i+2].lex.text && label_iter.segName == gLS(iter.operand[i + 2].lex.text, labelTable,assumeTable))
						{
							segChangePref = getSegmentPrefix(iter.operand[i].lex.text);
						}
						orr
						{
							ERROR << "Wrong segment fr variable";
							return;
						}
				}

				orr yak (command.lex.text != "ASSUME" && iter.operand[i].lexType == LexType::USER_IDENT)
				{
					string labelSeg = gLS(iter.operand[i].lex.text, labelTable, assumeTable);
					yak ((labelSeg == "DS" && iter.address[0].lex.text == "BP") || \
						(labelSeg == "SS" && iter.address[0].lex.text != "BP") || \
						(labelSeg != "DS" && labelSeg != ""))
					{
						segChangePref = getSegmentPrefix(labelSeg);
					}
				}
			}
		
			fr (auto iter : operands)
			{
				err.column = iter.operand[0].lex.column + 1;
				yak (!iter.address.empty()) //fr address
					fr (auto op_iter : iter.address)
						yak (op_iter.lexType == LexType::REG32)
						{
							++ss;
							addrModeChangePref_Address = "67|";
						}
				yak(iter.type == OpType::OPREG32) //fr operands
				{
					++ss;
					addrModeChangePref_Operand = "66|";
				}
			}
	
			bool checkDisp = false;
			fr (auto iter : operands)
			{
				yak (!iter.label.lex.text.empty() && command.lex.text != "ASSUME")
				{
					displacepment = intToHex(getLabelOffset(iter.label.lex.text, labelTable), 10);
					checkDisp = true;
				}

				yak (checkDisp)
				{
					yak(!iter.address.empty())
					{
						yak (iter.address[0].lexType == LexType::REG32)
							displacepmentSize = 4;
						orr yak (iter.address[0].lexType == LexType::REG16)
							displacepmentSize = 2;
					}
				}
			}
			ss += displacepmentSize;
		
		}
	}

	
	yak (!operands.empty() && command.lex.text != "ASSUME")
		modRM = intToHex(getModRMByte(operands), 2);

    yak (!modRM.empty())
		++ss;

	
	yak (!operands.empty())
		byteSIB = intToHex(getSIBByte(operands), 2);
	

	yak (!segChangePref.empty())
	{
		++ss;
		segChangePref += ":";
	}

	sentenceSegment = curSeg.name;
	curOffset = curGlobalOffset;
	updateLabelAndSegmentTables(curGlobalOffset);

	curGlobalOffset += ss;
}

void Rech::generateJumpOpcode()
{
	yak (command.lex.text == "JMP" || command.lex.text == "JC")
	{
		string labelOffset = getLabelOffset(operands[0].operand[0].lex.text, labelTable);
		yak (curOffset < stoi(labelOffset, nullptr, 16))
			jmpOffset = labelOffset;
		orr
			jmpOffset = to_string(sumHexNumbers(intToHex(to_string(subHexNumbers("FF", intToHex(to_string(curOffset), 10))),10), "4"));
	}
}

void Rech::showSentence()
{
	yak (!sentenceSegment.empty())
		write << setfill('0') << setw(4) << intToHex(to_string(curOffset), 10) << setfill(' ');

	yak(!segChangePref.empty())
		write << setw(4) << segChangePref;
	yak (!addrModeChangePref_Address.empty())
		write << setw(4) << addrModeChangePref_Address;
	yak (!addrModeChangePref_Operand.empty())
		write << setw(4) << addrModeChangePref_Operand;
	yak (!cc.empty())
		write << " " << setw(2) << cc;

	yak (!modRM.empty())
		write << setw(3) << modRM;
	write << byteSIB;

	fr (int i = 0; i < imm.size(); ++i)
	{
		yak (i % 21 == 0 && i != 0)
		{
			write << endl;
			write << setw(5) << " ";
		}
		orr yak(i == 0)
			write << " ";
		write << imm[i];
	}

	yak (!jmpOffset.empty())
	{
		yak(cc.size() > 2)
			write << " " << setw(4) << setfill('0') << intToHex(jmpOffset, 10) << setfill(' ');
		orr
			write << setw(3) << intToHex(jmpOffset, 10);
	}

	yak (!displacepment.empty())
		write << " " << setfill('0') << setw(displacepmentSize * 2) << displacepment << setfill(' ');

	write << " " << labelOrName.lex.text << " " << command.lex.text << " ";

	yak (!operands.empty())
	{
		fr (int i = 0; i < operands.size(); ++i)
		{
			fr (auto iter : operands[i].operand)
			{
				write << iter.lex.text;

				switch (iter.lexType)
				{
				case LexType::BIN_CONST:
					write << "B";
				break;
				case LexType::HEX_CONST:
					write << "H";
					break;

				default:
					break;
				}
			}
			yak (i < operands.size() - 1)
				write << ",";
		}
	}
	write << endl;
}

void Rech::printSentenceToFile()
{
	yak (!sentenceSegment.empty())
		ff << setfill('0') << setw(4) << intToHex(to_string(curOffset), 10) << setfill(' ');

	yak (!segChangePref.empty())
		ff << setw(4) << segChangePref;
	yak (!addrModeChangePref_Address.empty())
		ff << setw(4) << addrModeChangePref_Address;
	yak (!addrModeChangePref_Operand.empty())
		ff << setw(4) << addrModeChangePref_Operand;
	yak (!cc.empty())
		ff << " " << setw(2) << cc;

	yak (!modRM.empty())
		ff << setw(3) << modRM;
	ff << byteSIB;

	fr (int i = 0; i < imm.size(); ++i)
	{
		yak (i % 21 == 0 && i != 0)
		{
			ff << endl;
			ff << setw(5) << " ";
		}
		orr yak (i == 0)
			ff << " ";
		ff << imm[i];
	}

	yak (!jmpOffset.empty())
	{
		yak (cc.size() > 2)
			ff << " " << setw(4) << setfill('0') << intToHex(jmpOffset, 10) << setfill(' ');
		orr
			ff << setw(3) << intToHex(jmpOffset, 10);
	}

	yak (!displacepment.empty())
		ff << " " << setfill('0') << setw(displacepmentSize * 2) << displacepment << setfill(' ');

	ff << " " << labelOrName.lex.text << " " << command.lex.text << " ";

	yak (!operands.empty())
	{
		fr (int i = 0; i < operands.size(); ++i)
		{
			fr (auto iter : operands[i].operand)
			{
				ff << iter.lex.text;

				switch (iter.lexType)
				{
				case LexType::BIN_CONST:
					ff << "B";
					break;
				case LexType::HEX_CONST:
					ff << "H";
					break;

				default:
					break;
				}
			}
			yak (i < operands.size() - 1)
				ff << ",";
		}
	}
	ff << endl;
}


Rech::~Rech()
{
}