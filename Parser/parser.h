#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iterator>
#include "SymbolTable.h"

using namespace std;

void advance(ifstream&, string&);
vector<string> splitLine(string);
void printTabs(ofstream&, int);

bool isOp(char);

void terminalParser(ifstream&, ofstream&, string&, int);

void classParser(ifstream&, ofstream&, string&, int);
void classVarDecParser(ifstream&, ofstream&, string&, int);
void subroutineDecParser(ifstream&, ofstream&, string&, int);
void parameterListParser(ifstream&, ofstream&, string&, int, SymbolTable&);
void subroutineBodyParser(ifstream&, ofstream&, string&, int, SymbolTable&, bool, bool);
void varDecParser(ifstream&, ofstream&, string&, int, SymbolTable&);

void statementsParser(ifstream&, ofstream&, string&, int, SymbolTable&);
void letStatementParser(ifstream&, ofstream&, string&, int, SymbolTable&);
void ifStatementParser(ifstream&, ofstream&, string&, int, SymbolTable&);
void whileStatementParser(ifstream&, ofstream&, string&, int, SymbolTable&);
void doStatementParser(ifstream&, ofstream&, string&, int, SymbolTable&);
void returnStatementParser(ifstream&, ofstream&, string&, int, SymbolTable&);

void expressionParser(ifstream&, ofstream&, string&, int, SymbolTable&);
void termParser(ifstream&, ofstream&, string&, int, SymbolTable&);
void subroutineCallParser(ifstream&, ofstream&, string&, int, SymbolTable&);
int expressionListParser(ifstream&, ofstream&, string&, int, SymbolTable&);

//void termWrite(string);

#endif