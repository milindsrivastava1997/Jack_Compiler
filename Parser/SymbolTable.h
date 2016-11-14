#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct variable
{
	string name;
	string type;
	string kind;
	int index;
};

class SymbolTable
{
private:
	vector<variable*> vars;
	static int staticCount;
	int fieldCount;
	int localCount;
	int argCount;

public:
	SymbolTable();
	void define(string, string, string);
	void print();
	int varCount(string);
	string kindOf(string);
	string typeOf(string);
	int indexOf(string);
	~SymbolTable();
};

#endif