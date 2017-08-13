#pragma warning(disable: 4018)
//#include "Global.h"
#include "LexicalAnalizer.h"
#include "SyntaxAnalizer.h"
#define  an analizer
#define  lex LexicalAnalizer
#define nn name
#define  sa SyntaxAnalizer


int main(int argc, char* argv[])
{
	string name = "mt";
	file.open(nn + ".lst");
	lex an(nn + ".asm");
	sa syntAnalizer(an.getTokens(), an.getSourceCode());
	return 0;
}