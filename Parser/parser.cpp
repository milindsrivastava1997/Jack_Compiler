#include "parser.h"

string className;
string functionName;
SymbolTable classTable;

int ifCount = 0;
int whileCount = 0;

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
	int temp = 0;
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
	//outputFile << token << "\r\n";
	advance(inputFile, token);
}

void classParser(ifstream& inputFile, ofstream& outputFile, string& token, int tabs)
{
	vector<string> v;
	printTabs(outputFile, tabs);
	//outputFile << "<class>\r\n";

	terminalParser(inputFile, outputFile, token, tabs + 1);
	
	v = splitLine(token);
	className = v[1];
	
	terminalParser(inputFile, outputFile, token, tabs + 1);
	terminalParser(inputFile, outputFile, token, tabs + 1);

	v = splitLine(token);
	if(!v[0].compare("<symbol>"))
	{
		terminalParser(inputFile, outputFile, token, tabs + 1);
		outputFile << "</class>\r\n";
		classTable.print();
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
	//outputFile << "</class>\r\n";

	classTable.print();
}

void classVarDecParser(ifstream& inputFile, ofstream& outputFile, string& token, int tabs)
{
	vector<string> v;
	printTabs(outputFile, tabs);
	//outputFile << "<classVarDec>\r\n";

	string kind, type;

	v = splitLine(token);
	kind = v[1];

	terminalParser(inputFile, outputFile, token, tabs + 1);
	
	v = splitLine(token);
	type = v[1];
	
	terminalParser(inputFile, outputFile, token, tabs + 1);
	
	v = splitLine(token);
	classTable.define(v[1], type, kind);
	
	terminalParser(inputFile, outputFile, token, tabs + 1);

	v = splitLine(token);
	while(!v[1].compare(","))
	{
		terminalParser(inputFile, outputFile, token, tabs + 1);

		v = splitLine(token);
		classTable.define(v[1], type, kind);
		
		terminalParser(inputFile, outputFile, token, tabs + 1);
		v = splitLine(token);
	}

	terminalParser(inputFile, outputFile, token, tabs + 1);

	printTabs(outputFile, tabs);
	//outputFile << "</classVarDec>\r\n";
}

void subroutineDecParser(ifstream& inputFile, ofstream& outputFile, string& token, int tabs)
{
	SymbolTable subTable;
	bool isMethod = false, isConstructor = false;
	vector<string> v;
	printTabs(outputFile, tabs);
	//outputFile << "<subroutineDec>\r\n";

	v = splitLine(token);
	if(!v[1].compare("method"))
	{
		subTable.define(string("this"), className, string("argument"));
		isMethod = true;
	}
	else if(!v[1].compare("constructor"))
	{
		isConstructor = true;
	}

	terminalParser(inputFile, outputFile, token, tabs + 1);
	terminalParser(inputFile, outputFile, token, tabs + 1);

	v = splitLine(token);
	functionName = v[1];

	terminalParser(inputFile, outputFile, token, tabs + 1);
	terminalParser(inputFile, outputFile, token, tabs + 1);
	parameterListParser(inputFile, outputFile, (token), tabs + 1, subTable);
	terminalParser(inputFile, outputFile, token, tabs + 1);
	subroutineBodyParser(inputFile, outputFile, (token), tabs + 1, subTable, isMethod, isConstructor);

	subTable.print();

	printTabs(outputFile, tabs);
	//outputFile << "</subroutineDec>\r\n";
}

void parameterListParser(ifstream& inputFile, ofstream& outputFile, string& token, int tabs, SymbolTable& subTable)
{
	vector<string> v;
	printTabs(outputFile, tabs);
	//outputFile << "<parameterList>\r\n";

	string type, kind;

	v = splitLine(token);
	if(!v[1].compare(")"))
	{
		printTabs(outputFile, tabs);
		//outputFile << "</parameterList>\r\n";
		return;
	}

	type = v[1];

	terminalParser(inputFile, outputFile, token, tabs + 1);

	v = splitLine(token);
	subTable.define(v[1], type, string("argument"));

	terminalParser(inputFile, outputFile, token, tabs + 1);

	v = splitLine(token);

	while(!v[1].compare(","))
	{
		terminalParser(inputFile, outputFile, token, tabs + 1);

		v = splitLine(token);
		kind = v[1];

		terminalParser(inputFile, outputFile, token, tabs + 1);

		v = splitLine(token);
		subTable.define(v[1], type, string("argument"));
		
		terminalParser(inputFile, outputFile, token, tabs + 1);

		v = splitLine(token);
	}

	printTabs(outputFile, tabs);
	//outputFile << "</parameterList>\r\n";
}

void subroutineBodyParser(ifstream& inputFile, ofstream& outputFile, string& token, int tabs, SymbolTable& subTable, bool isMethod, bool isConstructor)
{
	vector<string> v;
	printTabs(outputFile, tabs);
	//outputFile << "<subroutineBody>\r\n";

	terminalParser(inputFile, outputFile, token, tabs + 1);
	v = splitLine(token);
	if(isConstructor || isMethod)
	{
		//subTable.define(string("this"), className, string("local"));
	}
	while(!v[1].compare("var"))
	{
		varDecParser(inputFile, outputFile, (token), tabs + 1, subTable);
		v = splitLine(token);
	}

	/* *************************** */
	outputFile << "function " << className << "." << functionName << " " << subTable.varCount(string("local")) << "\r\n";

	if(isMethod)
	{
		outputFile << "push argument 0\r\n";
		outputFile << "pop pointer 0\r\n";
	}
	else if(isConstructor)
	{
		outputFile << "push constant " << classTable.varCount(string("this")) << "\r\n";
		outputFile << "call Memory.alloc 1\r\n";
		outputFile << "pop pointer 0\r\n";
	}

	statementsParser(inputFile, outputFile, (token), tabs + 1, subTable);
	terminalParser(inputFile, outputFile, token, tabs + 1);

	printTabs(outputFile, tabs);
	//outputFile << "</subroutineBody>\r\n";
}

void varDecParser(ifstream& inputFile, ofstream& outputFile, string& token, int tabs, SymbolTable& subTable)
{
	vector<string> v;
	string type;
	printTabs(outputFile, tabs);
	//outputFile << "<varDec>\r\n";

	terminalParser(inputFile, outputFile, token, tabs + 1);

	v = splitLine(token);
	type = v[1];

	terminalParser(inputFile, outputFile, token, tabs + 1);

	v = splitLine(token);
	subTable.define(v[1], type, string("local"));

	terminalParser(inputFile, outputFile, token, tabs + 1);

	v = splitLine(token);
	while(!v[1].compare(","))
	{
		terminalParser(inputFile, outputFile, token, tabs + 1);

		v = splitLine(token);
		subTable.define(v[1], type, string("local"));

		terminalParser(inputFile, outputFile, token, tabs + 1);
		v = splitLine(token);
	}

	terminalParser(inputFile, outputFile, token, tabs + 1);

	printTabs(outputFile,tabs);
	//outputFile << "</varDec>\r\n";
}

void statementsParser(ifstream& inputFile, ofstream& outputFile, string& token, int tabs, SymbolTable& subTable)
{
	vector<string> v;
	printTabs(outputFile, tabs);
	//outputFile << "<statements>\r\n";

	v = splitLine(token);
	while(!v[1].compare("let") || !v[1].compare("if") || !v[1].compare("while") || !v[1].compare("do") || !v[1].compare("return"))
	{
		if(!v[1].compare("let"))
		{
			letStatementParser(inputFile, outputFile, (token), tabs + 1, subTable);
		}
		else if(!v[1].compare("if"))
		{
			ifStatementParser(inputFile, outputFile, (token), tabs + 1, subTable);
		}
		else if(!v[1].compare("while"))
		{
			whileStatementParser(inputFile, outputFile, (token), tabs + 1, subTable);
		}
		else if(!v[1].compare("do"))
		{
			doStatementParser(inputFile, outputFile, (token), tabs + 1, subTable);
		}
		else
		{
			returnStatementParser(inputFile, outputFile, (token), tabs + 1, subTable);
		}
		v = splitLine(token);
	}

	printTabs(outputFile, tabs);
	//outputFile << "</statements>\r\n";
}

void letStatementParser(ifstream& inputFile, ofstream& outputFile, string& token, int tabs, SymbolTable& subTable)
{
	vector<string> v;
	string arrayExpr;
	bool isArray = false, isString = false;
	printTabs(outputFile, tabs);
	//outputFile << "<letStatement>\r\n";

	terminalParser(inputFile, outputFile, token, tabs + 1);

	v = splitLine(token);
	int temp, temp2;
	string type = subTable.typeOf(v[1]);
	string kind = subTable.kindOf(v[1]);
	int index = subTable.indexOf(v[1]);
	if(!type.compare("null"))
	{
		type = classTable.typeOf(v[1]);
		kind = classTable.kindOf(v[1]);
		index = classTable.indexOf(v[1]);
	}

	/*if(!type.compare("Array"))
	{
		isArray = true;
	}*/
	if(!type.compare("String"))
	{
		isString = true;
		//outputFile << "push " << kind << " " << index << "\r\n";
	}

	terminalParser(inputFile, outputFile, token, tabs + 1);

	v = splitLine(token);

	if(!v[1].compare("["))
	{
		isArray = true;
		//outputFile << "push " << kind << " " << index << "\r\n";

		terminalParser(inputFile, outputFile, token, tabs + 1);
		arrayExpr = token;
		temp2 = inputFile.tellg();
		expressionParser(inputFile, outputFile, (token), tabs + 1, subTable);
		outputFile << "pop temp 0\r\n";
		//outputFile << "add\r\n";
		//outputFile << "pop pointer 1\r\n";
		terminalParser(inputFile, outputFile, token, tabs + 1);
	}

	terminalParser(inputFile, outputFile, token, tabs + 1);
	expressionParser(inputFile, outputFile, (token), tabs + 1, subTable);
	if(isArray)
	{
		temp = inputFile.tellg();

		outputFile << "push " << kind << " " << index << "\r\n";
		/*if(!kind.compare("this"))
		{
			outputFile << "pop pointer 1\r\n";
			outputFile << "push that 0\r\n";
		}*/
		inputFile.seekg(temp2);
		expressionParser(inputFile, outputFile, arrayExpr, tabs + 1, subTable);
		//token = arrayExpr;
		inputFile.seekg(temp);
		outputFile << "add\r\n";
		outputFile << "pop pointer 1\r\n";
		outputFile << "pop that 0\r\n";
	}
	else
	{
		/*if(!kind.compare("this"))
		{
			outputFile << "push " << kind << " " << index  << "\r\n";  
			outputFile << "pop pointer 1\r\n";
			outputFile << "pop that 0\r\n";
		}*/
		//else
		{
			outputFile << "pop " << kind << " " << index << "\r\n";
		}
	}
	terminalParser(inputFile, outputFile, token, tabs + 1);

	printTabs(outputFile, tabs);
	//outputFile << "</letStatement>\r\n";
}

void ifStatementParser(ifstream& inputFile, ofstream& outputFile, string& token, int tabs, SymbolTable& subTable)
{
	vector<string> v;
	printTabs(outputFile, tabs);
	//outputFile << "<ifStatement>\r\n";

	ifCount++;

	int temp = ifCount;

	terminalParser(inputFile, outputFile, token, tabs + 1);
	terminalParser(inputFile, outputFile, token, tabs + 1);
	expressionParser(inputFile, outputFile, (token), tabs + 1, subTable);
	outputFile << "not\r\n";
	outputFile << "if-goto ELSE_LABEL_" << temp << "\r\n";
	terminalParser(inputFile, outputFile, token, tabs + 1);	
	terminalParser(inputFile, outputFile, token, tabs + 1);
	statementsParser(inputFile, outputFile, (token), tabs + 1, subTable);
	terminalParser(inputFile, outputFile, token, tabs + 1);
	outputFile << "goto END_IF_LABEL_" << temp << "\r\n";

	outputFile << "label ELSE_LABEL_" << temp << "\r\n";

	v = splitLine(token);
	if(!v[1].compare("else"))
	{
		terminalParser(inputFile, outputFile, token, tabs + 1);
		terminalParser(inputFile, outputFile, token, tabs + 1);
		statementsParser(inputFile, outputFile, (token), tabs + 1, subTable);
		terminalParser(inputFile, outputFile, token, tabs + 1);
	}

	outputFile << "label END_IF_LABEL_" << temp << "\r\n";

	printTabs(outputFile, tabs);
	//outputFile << "</ifStatement>\r\n";
}

void whileStatementParser(ifstream& inputFile, ofstream& outputFile, string& token, int tabs, SymbolTable& subTable)
{
	printTabs(outputFile, tabs);
	//outputFile << "<whileStatement>\r\n";

	whileCount++;

	int temp = whileCount;

	terminalParser(inputFile, outputFile, token, tabs + 1);
	terminalParser(inputFile, outputFile, token, tabs + 1);
	outputFile << "label WHILE_LABEL_" << temp << "\r\n";
	expressionParser(inputFile, outputFile, (token), tabs + 1, subTable);
	outputFile << "not\r\n";
	outputFile << "if-goto END_WHILE_LABEL_" << temp << "\r\n";
	terminalParser(inputFile, outputFile, token, tabs + 1);
	terminalParser(inputFile, outputFile, token, tabs + 1);
	statementsParser(inputFile, outputFile, (token), tabs + 1, subTable);
	outputFile << "goto WHILE_LABEL_" << temp << "\r\n";
	terminalParser(inputFile, outputFile, token, tabs + 1);

	outputFile << "label END_WHILE_LABEL_" << temp << "\r\n";

	printTabs(outputFile, tabs);
	//outputFile << "</whileStatement>\r\n";
}

void doStatementParser(ifstream& inputFile, ofstream& outputFile, string& token, int tabs, SymbolTable& subTable)
{
	printTabs(outputFile, tabs);
	//outputFile << "<doStatement>\r\n";

	terminalParser(inputFile, outputFile, token, tabs + 1);
	subroutineCallParser(inputFile, outputFile, (token), tabs + 1, subTable);
	terminalParser(inputFile, outputFile, token, tabs + 1);

	outputFile << "pop temp 0\r\n";

	printTabs(outputFile, tabs);
	//outputFile << "</doStatement>\r\n";
}

void returnStatementParser(ifstream& inputFile, ofstream& outputFile, string& token, int tabs, SymbolTable& subTable)
{
	vector<string> v;
	printTabs(outputFile, tabs);
	//outputFile << "<returnStatement>\r\n";

	terminalParser(inputFile, outputFile, token, tabs + 1);

	v = splitLine(token);

	if(v[1].compare(";"))
	{
		expressionParser(inputFile, outputFile, (token), tabs + 1, subTable);
	}
	else
	{
		outputFile << "push constant 0\r\n";
	}
	outputFile << "return\r\n";

	terminalParser(inputFile, outputFile, token, tabs + 1);

	printTabs(outputFile, tabs);
	//outputFile << "</returnStatement>\r\n";
}

void expressionParser(ifstream& inputFile, ofstream& outputFile, string& token, int tabs, SymbolTable& subTable)
{
	vector<string> v;
	printTabs(outputFile, tabs);
	//outputFile << "<expression>\r\n";

	termParser(inputFile, outputFile, (token), tabs + 1, subTable);

	v = splitLine(token);
	while(isOp(v[1][0]))
	{
		terminalParser(inputFile, outputFile, token, tabs + 1);
		termParser(inputFile, outputFile, (token), tabs + 1, subTable);
		switch(v[1][0])
		{
			case '+':outputFile << "add\r\n"; break;
			case '-':outputFile << "sub\r\n"; break;
			case '*':outputFile << "call Math.multiply 2\r\n"; break;
			case '/':outputFile << "call Math.divide 2\r\n"; break;
			//case '&':outputFile << "and\r\n"; break;
			case '|':outputFile << "or\r\n"; break;
			//case '<':outputFile << "lt\r\n"; break;
			//case '>':outputFile << "gt\r\n"; break;
			case '=':outputFile << "eq\r\n"; break;
			default:
			{
				if(!v[1].compare("&amp;"))
				{
					outputFile << "and\r\n";
				}
				else if(!v[1].compare("&lt;"))
				{
					outputFile << "lt\r\n";
				}
				else if(!v[1].compare("&gt;"))
				{
					outputFile << "gt\r\n";
				}
			}
		}
		v = splitLine(token);
	}

	printTabs(outputFile, tabs);
	//outputFile << "</expression>\r\n";
}

void termParser(ifstream& inputFile, ofstream& outputFile, string& token, int tabs, SymbolTable& subTable)
{
	vector<string> v;
	printTabs(outputFile, tabs);
	//outputFile << "<term>\r\n";

	v = splitLine(token);

	if(!v[0].compare("<integerConstant>"))
	{
		terminalParser(inputFile, outputFile, token, tabs + 1);
		outputFile << "push constant " << v[1] << "\r\n";
	}
	else if(!v[0].compare("<stringConstant>"))
	{
		string stringToOutput(token.begin() + 17, token.end() - 18);
		outputFile << "push constant " << stringToOutput.size() << "\r\n";
		outputFile << "call String.new 1\r\n";
		for(int i = 0; i < stringToOutput.size(); i++)
		{
			outputFile << "push constant " << (int)stringToOutput[i] << "\r\n";
			outputFile << "call String.appendChar 2\r\n";
		}

		terminalParser(inputFile, outputFile, token, tabs + 1);
	}
	else if(!v[0].compare("<keyword>"))
	{
		terminalParser(inputFile, outputFile, token, tabs + 1);
		if(!v[1].compare("true"))
		{
			outputFile << "push constant 1\r\n";
			outputFile << "neg\r\n";
		}
		else if(!v[1].compare("this"))
		{
			outputFile << "push pointer 0\r\n";
		}
		else
		{
			outputFile << "push constant 0\r\n";
		}
	}
	else if(!v[1].compare("("))
	{
		terminalParser(inputFile, outputFile, token, tabs + 1);
		expressionParser(inputFile, outputFile, (token), tabs + 1, subTable);
		terminalParser(inputFile, outputFile, token, tabs + 1);
	}
	else if(v[1][0] == '-' || v[1][0] == '~')
	{
		terminalParser(inputFile, outputFile, token, tabs + 1);
		termParser(inputFile, outputFile, (token), tabs + 1, subTable);
		if(v[1][0] == '-')
		{
			outputFile << "neg\r\n";
		}
		else
		{
			outputFile << "not\r\n";
		}
	}
	else
	{
		int temp = inputFile.tellg();
		string prevToken = token;
		advance(inputFile, token);

		string type = subTable.typeOf(v[1]);
		string kind = subTable.kindOf(v[1]);
		int index = subTable.indexOf(v[1]);
		if(!type.compare("null"))
		{
			type = classTable.typeOf(v[1]);
			kind = classTable.kindOf(v[1]);
			index = classTable.indexOf(v[1]);
		}
		
		v = splitLine(token);

		if(!v[1].compare("(") || !v[1].compare("."))
		{
			inputFile.seekg(temp);
			subroutineCallParser(inputFile, outputFile, prevToken, tabs + 1, subTable);
			token = prevToken;
		}
		else if(!v[1].compare("["))
		{
			inputFile.seekg(temp);
			terminalParser(inputFile, outputFile, prevToken, tabs + 1);
			terminalParser(inputFile, outputFile, token, tabs + 1);
			expressionParser(inputFile, outputFile, token, tabs + 1, subTable);
			outputFile << "push " << kind << " " << index << "\r\n";
			/*if(!kind.compare("this"))
			{
				outputFile << "pop pointer 1\r\n";
				outputFile << "push that 0\r\n";
			}*/
			outputFile << "add\r\n";
			outputFile << "pop pointer 1\r\n";
			outputFile << "push that 0\r\n";
			terminalParser(inputFile, outputFile, token, tabs + 1);
		}
		else
		{
			token = prevToken;
			inputFile.seekg(temp);
			v = splitLine(token);

			outputFile << "push " << kind << " " << index << "\r\n";
			/*if(!kind.compare("this"))
			{
				outputFile << "pop pointer 1\r\n";
				outputFile << "push that 0\r\n";
			}*/
			
			terminalParser(inputFile, outputFile, token, tabs + 1);
		}
	}

	printTabs(outputFile, tabs);
	//outputFile << "</term>\r\n";
}

void subroutineCallParser(ifstream& inputFile, ofstream& outputFile, string& token, int tabs, SymbolTable& subTable)
{
	vector<string> v;
	bool isMethod = false;
	string varname, type, kind;
	string cName;
	int index;

	v = splitLine(token);
	string subroutineName = v[1];

	if(!(subTable.indexOf(v[1]) == -1 && classTable.indexOf(v[1]) == -1)) // if token is varname
	{
		isMethod = true;
		varname = v[1];
		type = subTable.typeOf(varname);
		kind = subTable.kindOf(varname);
		index = subTable.indexOf(varname);
		if(!type.compare("null"))
		{
			type = classTable.typeOf(varname);
			kind = classTable.kindOf(varname);
			index = classTable.indexOf(varname);
		}
		cName = type;
	}
	else
	{
		cName = v[1];
	}
	terminalParser(inputFile, outputFile, token, tabs);
	
	v = splitLine(token);

	if(!v[1].compare("."))
	{
		if(isMethod)
		{
			outputFile << "push " << kind << " " << index << "\r\n";
			/*if(!kind.compare("this"))
			{
				outputFile << "pop pointer 1\r\n";
				outputFile << "push that 0\r\n";
			}*/
		}
		terminalParser(inputFile, outputFile, token, tabs);
		subroutineName = splitLine(token)[1];
		terminalParser(inputFile, outputFile, token, tabs);
	}
	else
	{
		outputFile << "push pointer 0\r\n";
		isMethod = true;
		cName = className;
	}

	terminalParser(inputFile, outputFile, token, tabs);
	int argNum = expressionListParser(inputFile, outputFile, token, tabs, subTable);
	terminalParser(inputFile, outputFile, token, tabs);

	if(isMethod)
	{
		outputFile << "call " << cName << "." << subroutineName << " " << argNum + 1 << "\r\n";
	}
	else
	{
		outputFile << "call " << cName << "." << subroutineName << " " << argNum << "\r\n";
	}
}

int expressionListParser(ifstream& inputFile, ofstream& outputFile, string& token, int tabs, SymbolTable& subTable)
{
	vector<string> v;
	printTabs(outputFile, tabs);
	//outputFile << "<expressionList>\r\n";

	int count = 0;

	v = splitLine(token);
	if(!v[1].compare(")"))
	{
		printTabs(outputFile, tabs);
		//outputFile << "</expressionList>\r\n";
		return count;
	}

	expressionParser(inputFile, outputFile, (token), tabs + 1, subTable);

	count++;

	v = splitLine(token);
	while(!v[1].compare(","))
	{
		terminalParser(inputFile, outputFile, token, tabs + 1);
		expressionParser(inputFile, outputFile, (token), tabs + 1, subTable);
		count++;
		v = splitLine(token);
	}

	printTabs(outputFile, tabs);
	//outputFile << "</expressionList>\r\n";
	return count;
}
