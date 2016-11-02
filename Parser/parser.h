#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iterator>

using namespace std;

void advance(ifstream&, string&);
vector<string> splitLine(string);
void printTabs(ofstream&, int);

bool isOp(char);

void terminalParser(ifstream&, ofstream&, string&, int);

void classParser(ifstream&, ofstream&, string&, int);
void classVarDecParser(ifstream&, ofstream&, string&, int);
void subroutineDecParser(ifstream&, ofstream&, string&, int);
void parameterListParser(ifstream&, ofstream&, string&, int);
void subroutineBodyParser(ifstream&, ofstream&, string&, int);
void varDecParser(ifstream&, ofstream&, string&, int);

void statementsParser(ifstream&, ofstream&, string&, int);
void letStatementParser(ifstream&, ofstream&, string&, int);
void ifStatementParser(ifstream&, ofstream&, string&, int);
void whileStatementParser(ifstream&, ofstream&, string&, int);
void doStatementParser(ifstream&, ofstream&, string&, int);
void returnStatementParser(ifstream&, ofstream&, string&, int);

void expressionParser(ifstream&, ofstream&, string&, int);
void termParser(ifstream&, ofstream&, string&, int);
void subroutineCallParser(ifstream&, ofstream&, string&, int);
void expressionListParser(ifstream&, ofstream&, string&, int);