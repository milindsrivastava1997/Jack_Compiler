#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <limits.h>
#include <regex>

using namespace std;

void tokenizeAndWrite(ofstream&, string);
bool isKeyword(string);
bool isSymbol(char);
bool isNum(char);

int main(int argc, char** argv)
{
	if(argc != 3)
	{
		cerr << "Wrong number of command line arguments";
		return 0;
	}
	
	ifstream inputFile;
	ofstream outputFile;

	inputFile.open(argv[1]);
	outputFile.open(argv[2]);

	string token, extract;

	int lineNumber = 0;
	bool multiLineComment = false;
	int startCommentPosition;
	int endCommentPosition;

	outputFile << "<tokens>\r\n";

	while(true)
	{
		getline(inputFile, token, '\r');
		getline(inputFile, extract, '\n');

		if(inputFile.eof())
		{
			break;
		}
		if(token.size() == 0)
		{
			continue;
		}

		lineNumber++;

		int commentPosition = token.find("//");
		if(commentPosition != -1)
		{
			token.erase(token.begin() + commentPosition, token.end());
		}

		if(!multiLineComment)
		{
			startCommentPosition = token.find("/*");
			if(startCommentPosition != -1)
			{
				multiLineComment = true;
				tokenizeAndWrite(outputFile, string(token.begin(), token.begin() + startCommentPosition));
			}
		}

		if(multiLineComment)
		{
			endCommentPosition = token.find("*/", commentPosition + 1);
			if(endCommentPosition != -1)
			{
				tokenizeAndWrite(outputFile, string(token.begin() + endCommentPosition + 2, token.end()));
				multiLineComment = false;
				continue;
			}
		}

		if(multiLineComment)
		{
			continue;
		}

		if(token.size() == 0)
		{
			continue;
		}

		int leadingWhitespace = token.find_first_not_of(" \t");
		if(leadingWhitespace == -1)
		{
			continue;
		}
		token.erase(token.begin(), token.begin() + leadingWhitespace);

		if(token.size() == 0)
		{
			continue;
		}

		tokenizeAndWrite(outputFile, token);
	}

	outputFile << "</tokens>\r\n";

	inputFile.close();
	outputFile.close();

	return 0;
}

void tokenizeAndWrite(ofstream& outputFile, string token)
{

	if(token.size() == 0)
	{
		return;
	}
	int firstQuote = token.find("\"");
	int secondQuote;
	if(firstQuote != -1)
	{
		secondQuote = token.find("\"", firstQuote + 1);
		tokenizeAndWrite(outputFile, string(token.begin(), token.begin() + firstQuote));
		outputFile << "<stringConstant> " << string(token.begin() + firstQuote + 1, token.begin() + secondQuote) << " </stringConstant>\r\n";
		tokenizeAndWrite(outputFile, string(token.begin() + secondQuote + 1, token.end()));
		return;
	}

	vector<string> tokens;
	stringstream s(token);
	string temp;

	while(getline(s, temp, ' ')) // tokenize
	{
		tokens.push_back(temp);
	}

	vector<string>::iterator it = tokens.begin();

	bool stringFlag = false;

	while(it != tokens.end())
	{
		string temp = *it;
		temp = regex_replace(temp, regex("\\s+"), string(""));
		if(temp.size() == 0)
		{
			it++;
			continue;
		}
		if(isKeyword(temp) && !stringFlag)
		{
			outputFile << "<keyword> " << *it << " </keyword>\r\n";
		}
		else
		{
			for(int i = 0; i < temp.size();)
			{
				if(isSymbol(temp[i]) && !stringFlag)
				{
					outputFile << "<symbol> ";
					if(temp[i] == '<')
					{
						outputFile << "&lt;";	
					}
					else if(temp[i] == '>')
					{
						outputFile << "&gt;";
					}
					else if(temp[i] == '"')
					{
						outputFile << "&quot;";
					}
					else if(temp[i] == '&')
					{
						outputFile << "&amp;";
					}
					else
					{
						outputFile << temp[i];
					}
					outputFile << " </symbol>\r\n";

					i++;
				}
				else if(isNum(temp[i]) && !stringFlag)
				{
					outputFile << "<integerConstant> ";
					while(isNum(temp[i]) && i < temp.size())
					{
						outputFile << temp[i];
						i++;
					}
					outputFile << " </integerConstant>\r\n";
				}
				else
				{	
					if(temp.size() >= 7)
					{
						if(!string("return;").compare(string(temp.begin() + i, temp.begin() + i + 7)))
						{
							outputFile << "<keyword> return </keyword>\r\n<symbol> ; </symbol>\r\n";
							i += 7;
							continue;
						}
					}
					if(temp.size() >= 6)
					{
						if(!string("false;").compare(string(temp.begin() + i, temp.begin() + i + 6)))
						{
							outputFile << "<keyword> false </keyword>\r\n<symbol> ; </symbol>\r\n";
							i += 6;
							continue;
						}
						if(!string("false)").compare(string(temp.begin() + i, temp.begin() + i + 6)))
						{
							outputFile << "<keyword> false </keyword>\r\n<symbol> ; </symbol>\r\n";
							i += 6;
							continue;
						}
					}
					if(temp.size() >= 5)
					{
						if(!string("true;").compare(string(temp.begin() + i, temp.begin() + i + 5)))
						{
							outputFile << "<keyword> true </keyword>\r\n<symbol> ; </symbol>\r\n";
							i += 5;
							continue;
						}
						if(!string("null;").compare(string(temp.begin() + i, temp.begin() + i + 5)))
						{
							outputFile << "<keyword> null </keyword>\r\n<symbol> ; </symbol>\r\n";
							i += 5;
							continue;
						}
						if(!string("this;").compare(string(temp.begin() + i, temp.begin() + i + 5)))
						{
							outputFile << "<keyword> this </keyword>\r\n<symbol> ; </symbol>\r\n";
							i += 5;
							continue;
						}
						if(!string("true)").compare(string(temp.begin() + i, temp.begin() + i + 5)))
						{
							outputFile << "<keyword> true </keyword>\r\n<symbol> ; </symbol>\r\n";
							i += 5;
							continue;
						}
						if(!string("null)").compare(string(temp.begin() + i, temp.begin() + i + 5)))
						{
							outputFile << "<keyword> null </keyword>\r\n<symbol> ; </symbol>\r\n";
							i += 5;
							continue;
						}
						if(!string("this)").compare(string(temp.begin() + i, temp.begin() + i + 5)))
						{
							outputFile << "<keyword> this </keyword>\r\n<symbol> ; </symbol>\r\n";
							i += 5;
							continue;
						}
					}
					int lastParentheses = temp.find_last_of('{');
					lastParentheses = temp.find_last_of('(');
					if(lastParentheses == -1 || lastParentheses < i)
					{
						lastParentheses = INT_MAX;
					}
					int lastCloseParentheses = temp.find_last_of(')');
					if(lastCloseParentheses == -1)
					{
						lastCloseParentheses = temp.size();
					}
					outputFile << "<identifier> ";
					while(!isSymbol(temp[i]) && !isNum(temp[i]) && temp[i] != '"' && i < temp.size() && i < lastParentheses)
					{
						outputFile << temp[i];
						i++;
					}
					if(i == lastParentheses)
					{
						outputFile << " </identifier>\r\n";
						if(isKeyword(string(temp.begin() + lastParentheses + 1, temp.begin() + lastCloseParentheses)))
						{
							outputFile << "<symbol> " << temp[lastParentheses] << " </symbol>\r\n<keyword> " << string(temp.begin() + lastParentheses + 1, temp.begin() + lastCloseParentheses) << " </keyword>\r\n";
							i = lastCloseParentheses;
						}
					}
					else
					{
						outputFile << " </identifier>\r\n";
					}
				}
			}

		}
		it++;
	}
}

bool isKeyword(string input)
{
	if(input.size() == 0)
	{
		return false;
	}
	if(!input.compare("class") || !input.compare("constructor") || !input.compare("function") || !input.compare("method") || !input.compare("field") || !input.compare("static")
		|| !input.compare("var") || !input.compare("int") || !input.compare("char") || !input.compare("boolean") || !input.compare("void") || !input.compare("true") || !input.compare("false")
		|| !input.compare("null") || !input.compare("this") || !input.compare("let") || !input.compare("do") || !input.compare("if") || !input.compare("else") || !input.compare("while")
		|| !input.compare("return"))
	{
		return true;
	}
	return false;
}

bool isSymbol(char input)
{
	if(input == '{' || input == '}' || input == '(' || input == ')' || input == '[' || input == ']' || input == '.' || input == ',' || input == ';' || input == '+' || input == '-'
		|| input == '*' || input == '/' || input == '&' || input == '|' || input == '<' || input == '>' || input == '=' || input == '~')
	{
		return true;
	}
	return false;
}

bool isNum(char input)
{
	if(input <= '9' && input >= '0')
	{
		return true;
	}
	return false;
}