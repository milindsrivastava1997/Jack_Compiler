#include "parser.h"

void advance(ifstream& inputFile, string& token)
{
	string extract;
	getline(inputFile, token, '\r');
	
	getline(inputFile, extract, '\n');
}

vector<string> splitLine(string input)
{
	vector<string> tokens;
	stringstream s(input);
	string temp;

	while(getline(s, temp, ' '))
	{
		tokens.push_back(temp);
	}

	return tokens;
}

void printTabs(ofstream& outputFile, int tabs)
{
	int temp = tabs;
	while(temp != 0)
	{
		outputFile << "  ";
		temp--;
	}
}

bool isOp(char input)
{
	if(input == '+' || input == '-' || input == '*' || input == '/' || input == '&' || input == '|' || input == '<' || input == '>' || input == '=')
	{
		return true;
	}
	return false;
}

void terminalParser(ifstream& inputFile, ofstream& outputFile, string& token, int tabs)
{
	printTabs(outputFile, tabs);
	outputFile << token << "\r\n";
	advance(inputFile, token);
}

void classParser(ifstream& inputFile, ofstream& outputFile, string& token, int tabs)
{
	vector<string> v;
	printTabs(outputFile, tabs);
	outputFile << "<class>\r\n";

	terminalParser(inputFile, outputFile, token, tabs + 1);
	terminalParser(inputFile, outputFile, token, tabs + 1);
	terminalParser(inputFile, outputFile, token, tabs + 1);

	v = splitLine(token);
	if(!v[0].compare("<symbol>"))
	{
		terminalParser(inputFile, outputFile, token, tabs + 1);
		outputFile << "</class>\r\n";
		return;
	}
	while(!v[1].compare("static") || !v[1].compare("field"))
	{
		classVarDecParser(inputFile, outputFile, (token), tabs + 1);
		v = splitLine(token);
	}
	while(!v[1].compare("constructor") || !v[1].compare("function") || !v[1].compare("method"))
	{
		subroutineDecParser(inputFile, outputFile, (token), tabs + 1);
		v = splitLine(token);
	}

	terminalParser(inputFile, outputFile, token, tabs + 1);

	printTabs(outputFile, tabs);
	outputFile << "</class>\r\n";
}

void classVarDecParser(ifstream& inputFile, ofstream& outputFile, string& token, int tabs)
{
	vector<string> v;
	printTabs(outputFile, tabs);
	outputFile << "<classVarDec>\r\n";

	terminalParser(inputFile, outputFile, token, tabs + 1);
	terminalParser(inputFile, outputFile, token, tabs + 1);
	terminalParser(inputFile, outputFile, token, tabs + 1);

	v = splitLine(token);
	while(!v[1].compare(","))
	{
		terminalParser(inputFile, outputFile, token, tabs + 1);
		terminalParser(inputFile, outputFile, token, tabs + 1);
		v = splitLine(token);
	}

	terminalParser(inputFile, outputFile, token, tabs + 1);

	printTabs(outputFile, tabs);
	outputFile << "</classVarDec>\r\n";
}

void subroutineDecParser(ifstream& inputFile, ofstream& outputFile, string& token, int tabs)
{
	vector<string> v;
	printTabs(outputFile, tabs);
	outputFile << "<subroutineDec>\r\n";

	terminalParser(inputFile, outputFile, token, tabs + 1);
	terminalParser(inputFile, outputFile, token, tabs + 1);
	terminalParser(inputFile, outputFile, token, tabs + 1);
	terminalParser(inputFile, outputFile, token, tabs + 1);
	parameterListParser(inputFile, outputFile, (token), tabs + 1);
	terminalParser(inputFile, outputFile, token, tabs + 1);
	subroutineBodyParser(inputFile, outputFile, (token), tabs + 1);

	printTabs(outputFile, tabs);
	outputFile << "</subroutineDec>\r\n";
}

void parameterListParser(ifstream& inputFile, ofstream& outputFile, string& token, int tabs)
{
	vector<string> v;
	printTabs(outputFile, tabs);
	outputFile << "<parameterList>\r\n";

	v = splitLine(token);
	if(!v[1].compare(")"))
	{
		printTabs(outputFile, tabs);
		outputFile << "</parameterList>\r\n";
		return;
	}

	terminalParser(inputFile, outputFile, token, tabs + 1);
	terminalParser(inputFile, outputFile, token, tabs + 1);

	v = splitLine(token);

	while(!v[1].compare(","))
	{
		terminalParser(inputFile, outputFile, token, tabs + 1);
		terminalParser(inputFile, outputFile, token, tabs + 1);
		terminalParser(inputFile, outputFile, token, tabs + 1);

		v = splitLine(token);
	}

	printTabs(outputFile, tabs);
	outputFile << "</parameterList>\r\n";
}

void subroutineBodyParser(ifstream& inputFile, ofstream& outputFile, string& token, int tabs)
{
	vector<string> v;
	printTabs(outputFile, tabs);
	outputFile << "<subroutineBody>\r\n";

	terminalParser(inputFile, outputFile, token, tabs + 1);
	v = splitLine(token);
	while(!v[1].compare("var"))
	{
		varDecParser(inputFile, outputFile, (token), tabs + 1);
		v = splitLine(token);
	}

	statementsParser(inputFile, outputFile, (token), tabs + 1);
	terminalParser(inputFile, outputFile, token, tabs + 1);

	printTabs(outputFile, tabs);
	outputFile << "</subroutineBody>\r\n";
}

void varDecParser(ifstream& inputFile, ofstream& outputFile, string& token, int tabs)
{
	vector<string> v;
	printTabs(outputFile, tabs);
	outputFile << "<varDec>\r\n";

	terminalParser(inputFile, outputFile, token, tabs + 1);
	terminalParser(inputFile, outputFile, token, tabs + 1);
	terminalParser(inputFile, outputFile, token, tabs + 1);

	v = splitLine(token);
	while(!v[1].compare(","))
	{
		terminalParser(inputFile, outputFile, token, tabs + 1);
		terminalParser(inputFile, outputFile, token, tabs + 1);
		v = splitLine(token);
	}

	terminalParser(inputFile, outputFile, token, tabs + 1);

	printTabs(outputFile,tabs);
	outputFile << "</varDec>\r\n";
}

void statementsParser(ifstream& inputFile, ofstream& outputFile, string& token, int tabs)
{
	vector<string> v;
	printTabs(outputFile, tabs);
	outputFile << "<statements>\r\n";

	v = splitLine(token);
	while(!v[1].compare("let") || !v[1].compare("if") || !v[1].compare("while") || !v[1].compare("do") || !v[1].compare("return"))
	{
		if(!v[1].compare("let"))
		{
			letStatementParser(inputFile, outputFile, (token), tabs + 1);
		}
		else if(!v[1].compare("if"))
		{
			ifStatementParser(inputFile, outputFile, (token), tabs + 1);
		}
		else if(!v[1].compare("while"))
		{
			whileStatementParser(inputFile, outputFile, (token), tabs + 1);
		}
		else if(!v[1].compare("do"))
		{
			doStatementParser(inputFile, outputFile, (token), tabs + 1);
		}
		else
		{
			returnStatementParser(inputFile, outputFile, (token), tabs + 1);
		}
		v = splitLine(token);
	}

	printTabs(outputFile, tabs);
	outputFile << "</statements>\r\n";
}

void letStatementParser(ifstream& inputFile, ofstream& outputFile, string& token, int tabs)
{
	vector<string> v;
	printTabs(outputFile, tabs);
	outputFile << "<letStatement>\r\n";

	terminalParser(inputFile, outputFile, token, tabs + 1);
	terminalParser(inputFile, outputFile, token, tabs + 1);

	v = splitLine(token);

	if(!v[1].compare("["))
	{
		terminalParser(inputFile, outputFile, token, tabs + 1);
		expressionParser(inputFile, outputFile, (token), tabs + 1);
		terminalParser(inputFile, outputFile, token, tabs + 1);
	}

	terminalParser(inputFile, outputFile, token, tabs + 1);
	expressionParser(inputFile, outputFile, (token), tabs + 1);
	terminalParser(inputFile, outputFile, token, tabs + 1);

	printTabs(outputFile, tabs);
	outputFile << "</letStatement>\r\n";
}

void ifStatementParser(ifstream& inputFile, ofstream& outputFile, string& token, int tabs)
{
	vector<string> v;
	printTabs(outputFile, tabs);
	outputFile << "<ifStatement>\r\n";

	terminalParser(inputFile, outputFile, token, tabs + 1);
	terminalParser(inputFile, outputFile, token, tabs + 1);
	expressionParser(inputFile, outputFile, (token), tabs + 1);
	terminalParser(inputFile, outputFile, token, tabs + 1);	
	terminalParser(inputFile, outputFile, token, tabs + 1);
	statementsParser(inputFile, outputFile, (token), tabs + 1);
	terminalParser(inputFile, outputFile, token, tabs + 1);

	v = splitLine(token);
	if(!v[1].compare("else"))
	{
		terminalParser(inputFile, outputFile, token, tabs + 1);
		terminalParser(inputFile, outputFile, token, tabs + 1);
		statementsParser(inputFile, outputFile, (token), tabs + 1);
		terminalParser(inputFile, outputFile, token, tabs + 1);
	}

	printTabs(outputFile, tabs);
	outputFile << "</ifStatement>\r\n";
}

void whileStatementParser(ifstream& inputFile, ofstream& outputFile, string& token, int tabs)
{
	printTabs(outputFile, tabs);
	outputFile << "<whileStatement>\r\n";

	terminalParser(inputFile, outputFile, token, tabs + 1);
	terminalParser(inputFile, outputFile, token, tabs + 1);
	expressionParser(inputFile, outputFile, (token), tabs + 1);
	terminalParser(inputFile, outputFile, token, tabs + 1);
	terminalParser(inputFile, outputFile, token, tabs + 1);
	statementsParser(inputFile, outputFile, (token), tabs + 1);
	terminalParser(inputFile, outputFile, token, tabs + 1);

	printTabs(outputFile, tabs);
	outputFile << "</whileStatement>\r\n";
}

void doStatementParser(ifstream& inputFile, ofstream& outputFile, string& token, int tabs)
{
	printTabs(outputFile, tabs);
	outputFile << "<doStatement>\r\n";

	terminalParser(inputFile, outputFile, token, tabs + 1);
	subroutineCallParser(inputFile, outputFile, (token), tabs + 1);
	terminalParser(inputFile, outputFile, token, tabs + 1);

	printTabs(outputFile, tabs);
	outputFile << "</doStatement>\r\n";
}

void returnStatementParser(ifstream& inputFile, ofstream& outputFile, string& token, int tabs)
{
	vector<string> v;
	printTabs(outputFile, tabs);
	outputFile << "<returnStatement>\r\n";

	terminalParser(inputFile, outputFile, token, tabs + 1);

	v = splitLine(token);

	if(v[1].compare(";"))
	{
		expressionParser(inputFile, outputFile, (token), tabs + 1);
	}

	terminalParser(inputFile, outputFile, token, tabs + 1);

	printTabs(outputFile, tabs);
	outputFile << "</returnStatement>\r\n";
}

void expressionParser(ifstream& inputFile, ofstream& outputFile, string& token, int tabs)
{
	vector<string> v;
	printTabs(outputFile, tabs);
	outputFile << "<expression>\r\n";

	termParser(inputFile, outputFile, (token), tabs + 1);

	v = splitLine(token);
	while(isOp(v[1][0]))
	{
		terminalParser(inputFile, outputFile, token, tabs + 1);
		termParser(inputFile, outputFile, (token), tabs + 1);
		v = splitLine(token);
	}

	printTabs(outputFile, tabs);
	outputFile << "</expression>\r\n";
}

void termParser(ifstream& inputFile, ofstream& outputFile, string& token, int tabs)
{
	vector<string> v;
	printTabs(outputFile, tabs);
	outputFile << "<term>\r\n";

	v = splitLine(token);

	if(!v[0].compare("<integerConstant>") || !v[0].compare("<stringConstant>") || !v[0].compare("<keyword>"))
	{
		terminalParser(inputFile, outputFile, token, tabs + 1);
	}
	else if(!v[1].compare("("))
	{
		terminalParser(inputFile, outputFile, token, tabs + 1);
		expressionParser(inputFile, outputFile, (token), tabs + 1);
		terminalParser(inputFile, outputFile, token, tabs + 1);
	}
	else if(v[1][0] == '-' || v[1][0] == '~')
	{
		terminalParser(inputFile, outputFile, token, tabs + 1);
		termParser(inputFile, outputFile, (token), tabs + 1);
	}
	else
	{
		int temp = inputFile.tellg();
		string prevToken = token;
		advance(inputFile, token);

		v = splitLine(token);

		if(!v[1].compare("(") || !v[1].compare("."))
		{
			inputFile.seekg(temp);
			subroutineCallParser(inputFile, outputFile, prevToken, tabs + 1);
			token = prevToken;
		}
		else if(!v[1].compare("["))
		{
			inputFile.seekg(temp);
			terminalParser(inputFile, outputFile, prevToken, tabs + 1);
			terminalParser(inputFile, outputFile, token, tabs + 1);
			expressionParser(inputFile, outputFile, token, tabs + 1);
			terminalParser(inputFile, outputFile, token, tabs + 1);
		}
		else
		{
			token = prevToken;
			inputFile.seekg(temp);
			terminalParser(inputFile, outputFile, token, tabs + 1);
		}
	}

	printTabs(outputFile, tabs);
	outputFile << "</term>\r\n";
}

void subroutineCallParser(ifstream& inputFile, ofstream& outputFile, string& token, int tabs)
{
	vector<string> v;

	terminalParser(inputFile, outputFile, token, tabs);
	
	v = splitLine(token);

	if(!v[1].compare("."))
	{
		terminalParser(inputFile, outputFile, token, tabs);
		terminalParser(inputFile, outputFile, token, tabs);
	}

	terminalParser(inputFile, outputFile, token, tabs);
	expressionListParser(inputFile, outputFile, token, tabs);
	terminalParser(inputFile, outputFile, token, tabs);
}

void expressionListParser(ifstream& inputFile, ofstream& outputFile, string& token, int tabs)
{
	vector<string> v;
	printTabs(outputFile, tabs);
	outputFile << "<expressionList>\r\n";

	v = splitLine(token);
	if(!v[1].compare(")"))
	{
		printTabs(outputFile, tabs);
		outputFile << "</expressionList>\r\n";
		return;
	}

	expressionParser(inputFile, outputFile, (token), tabs + 1);

	v = splitLine(token);
	while(!v[1].compare(","))
	{
		terminalParser(inputFile, outputFile, token, tabs + 1);
		expressionParser(inputFile, outputFile, (token), tabs + 1);
		v = splitLine(token);
	}

	printTabs(outputFile, tabs);
	outputFile << "</expressionList>\r\n";
}