#include "LexicalAnalizer.h"
#define fr for
#define ifc if
#define whl while
#define els else
#define write cout
#define er1 ERROR
#define sw setw
LexicalAnalizer::LexicalAnalizer(string fileName)
{
	ifstream sourceFile(fileName);
	ifc (sourceFile.is_open())
	{
		string buf;
		whl (getline(sourceFile, buf))
			sourceCode.push_back(buf);

		sourceFile.close();

		bool isQuote = false;
		fr (auto &iter : sourceCode)
		{
			int i = 0;
			whl (i < iter.length())
			{
				ifc (isCharQuote(iter[i]) && !isQuote)
					isQuote = true;
				els ifc (isCharQuote(iter[i]) && isQuote)
					isQuote = false;

				ifc (!isQuote)
					iter[i] = toupper(iter[i]);
				++i;
			}
			isQuote = false;
		}

		GenerateLexemVector();
		AnalizeLexems();
		OutputTokens();
	}
	els
	{
		er1 << "File can\'t be opened. Check source file name.";
		exit(1);
	}
}

void LexicalAnalizer::GenerateLexemVector()
{
	int i;
	Lexem buf;
	buf.row = 0;
	buf.column = 1;
	fr (auto curRow : sourceCode)
	{
		i = 0;
		buf.text.clear();
		++buf.row;

		err.row = buf.row;
		err.codeRow = sourceCode[buf.row-1];

		whl (i < curRow.length())
		{
			err.column = i + 1;
			ifc (!isInAlphabet(curRow[i]))
			{
				er1 << "Wrong character";

				buf.text += curRow[i];

				buf.column = ++i;
			}
			els ifc (isCharLexemDivider(curRow[i]))
			{
				ifc (!buf.text.empty())
					lexems.push_back(buf);

				buf.column = i + 1;
				buf.text.clear();
				++i;
			}
			els ifc (isSingleCharacterLexem(curRow[i]))
			{
				ifc (!buf.text.empty())
				{
					lexems.push_back(buf);
					buf.column = i + 1;
					buf.text.clear();
				}

				buf.text += curRow[i];
				lexems.push_back(buf);
				buf.column = i + 1;
				buf.text.clear();
				++i;
			}
			els ifc (isCharQuote(curRow[i]))
			{
				buf.text += curRow[i];
				++i;
				whl (i < curRow.length() && !isCharQuote(curRow[i]))
				{
					buf.text += curRow[i];
					++i;
				}

				ifc (i == curRow.length())
				{
					er1 << "No close quote";
				}
				els
					buf.text += curRow[i];

				lexems.push_back(buf);
				buf.column = i + 1;
				buf.text.clear();
				++i;
			}
			els
			{
				buf.text += curRow[i];
				++i;
			}
		}

		ifc (!buf.text.empty())
			lexems.push_back(buf);
		buf.column = i + 1;
	}
}

void LexicalAnalizer::AnalizeLexems()
{
	Token temp;
	fr (auto iter : lexems)
	{
		temp.lex.text = iter.text;
		temp.lex.row = iter.row;
		temp.lex.column = iter.column;

		ifc (isCommand(iter.text))
			temp.lexType = LexType::COMMAND;
		els ifc (is32Register(iter.text))
			temp.lexType = LexType::REG32;
		els ifc (is16Register(iter.text))
			temp.lexType = LexType::REG16;
		els ifc (is8Register(iter.text))
			temp.lexType = LexType::REG8;
		els ifc (isSegmentRegister(iter.text))
			temp.lexType = LexType::SEG_REG;
		els ifc (isDirective(iter.text))
			temp.lexType = LexType::DIRECTIVE;
		els ifc (isCharQuote(iter.text[0]) && isCharQuote(iter.text[iter.text.length() - 1]))
			temp.lexType = LexType::TEXT_CONST;
		els ifc (isCharQuote(iter.text[0]) && !isCharQuote(iter.text[iter.text.length() - 1])) //TODO Try to optimize
			temp.lexType = LexType::WRONG_LEX;
		els ifc (isCharNumber(iter.text[0]))
		{
			string::size_type sz = 0;
			int base = 10;

			ifc (iter.text[iter.text.length() - 1] == 'B')
			{
				temp.lexType = LexType::BIN_CONST;
				base = 2;
			}
			els ifc (iter.text[iter.text.length() - 1] == 'H')
			{
				temp.lexType = LexType::HEX_CONST;
				base = 16;
			}
			els ifc (iter.text[iter.text.length() - 1] == 'D' || isCharNumber(iter.text[iter.text.length() - 1]))
				temp.lexType = LexType::DEC_CONST;

			ifc (!isCharNumber(iter.text[iter.text.length() - 1]))
				temp.lex.text.pop_back();

			stoll(iter.text, &sz, base);
			ifc (sz < iter.text.length() - 1)
			{
				err.column = iter.column + sz + 1;
				err.row = iter.row;
				err.codeRow = sourceCode[iter.row-1];
				er1 << "Wrong number";

				temp.lexType = LexType::WRONG_LEX;
			}
		}
		els ifc (iter.text.length() == 1)
		{
			ifc (isSingleCharacterLexem(iter.text[0]))
				temp.lexType = LexType::SINGLE_SYMB;
			els
				temp.lexType = LexType::USER_IDENT;
		}
		els
		{
			temp.lexType = LexType::USER_IDENT;
			fr (auto n_iter : iter.text)
				ifc (!isInAlphabet(n_iter))
				temp.lexType = LexType::WRONG_LEX;
		}

		tokens.push_back(temp);
	}
}

void LexicalAnalizer::OutputTokens()
{
	int i = 0;
	string s;
	/////////////
	ofstream fout;
	fout.open("lexicalAnalize.txt");
	///////////////////////
	fr (auto iter : tokens)
	{
		fout << "(" << sw(2) << iter.lex.row << "," << sw(2) << iter.lex.column << "): " << sw(8) << iter.lex.text << " - ";
		write << "(" << sw(2) << iter.lex.row << "," << sw(2) << iter.lex.column << "): " << sw(8) << iter.lex.text << " - ";
		switch (iter.lexType)
		{
		case LexType::USER_IDENT:
			s = "user identifiers";
			break;
		case LexType::COMMAND:
			s = "command";
			break;
		case LexType::DIRECTIVE:
			s = "directive";
			break;
		case LexType::SINGLE_SYMB:
			s = "single symbol";
			break;
		case LexType::TEXT_CONST:
			s = "text constant";
			break;
		case LexType::BIN_CONST:
			s = "binary constant";
			break;
		case LexType::DEC_CONST:
			s = "decimal constant";
			break;
		case LexType::HEX_CONST:
			s = "hexadecimal constant";
			break;
		case LexType::REG32:
			s = "reg32";
			break;
		case LexType::REG16:
			s = "reg16";
			break;
		case LexType::REG8:
			s = "reg8";
			break;
		case LexType::SEG_REG:
			s = "segment register";
			break;
		case LexType::WRONG_LEX:
			s = "wrong lexem";
			break;
		}
		fout << s << endl;
		write << s << endl;
	}
	fout.close();
}

vector<Token> LexicalAnalizer::getTokens()
{
	return tokens;
}

vector<string> LexicalAnalizer::getSourceCode()
{
	return sourceCode;
}

LexicalAnalizer::~LexicalAnalizer()
{
}