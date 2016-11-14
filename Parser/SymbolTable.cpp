#include "SymbolTable.h"

int SymbolTable::staticCount;

SymbolTable::SymbolTable()
{
	SymbolTable::staticCount = 0;
	fieldCount = 0;
	localCount = 0;
	argCount = 0;
}

void SymbolTable::define(string name, string type, string kind)
{
	variable* temp = new variable;
	temp->name = name;
	temp->type = type;
	temp->kind = kind;
	if(!kind.compare("static"))
	{
		temp->index = SymbolTable::staticCount;
		SymbolTable::staticCount++;
	}
	else if(!kind.compare("field"))
	{
		temp->index = fieldCount;
		temp->kind = "this";
		fieldCount++;
	}
	else if(!kind.compare("local"))
	{
		temp->index = localCount;
		localCount++;
	}
	else if(!kind.compare("argument"))
	{
		temp->index = argCount;
		argCount++;
	}
	vars.push_back(temp);
}

void SymbolTable::print()
{
	for(int i = 0; i < vars.size(); i++)
	{
		cout << vars[i]->name << " " << vars[i]->type << " " << vars[i]->kind << " " << vars[i]->index << endl;
	}
}

int SymbolTable::varCount(string kind)
{
	if(!kind.compare("static"))
	{
		return SymbolTable::staticCount;
	}
	else if(!kind.compare("this"))
	{
		return fieldCount;
	}
	else if(!kind.compare("local"))
	{
		return localCount;
	}
	else if(!kind.compare("argument"))
	{
		return argCount;
	}
}

string SymbolTable::kindOf(string name)
{
	for(int i = 0; i < vars.size(); i++)
	{
		if(!vars[i]->name.compare(name))
		{
			return vars[i]->kind;
		}
	}
	return "null";
}

string SymbolTable::typeOf(string name)
{
	for(int i = 0; i < vars.size(); i++)
	{
		if(!vars[i]->name.compare(name))
		{
			return vars[i]->type;
		}
	}
	return "null";
}

int SymbolTable::indexOf(string name)
{
	for(int i = 0; i < vars.size(); i++)
	{
		if(!vars[i]->name.compare(name))
		{
			return vars[i]->index;
		}
	}
	return -1;
}

SymbolTable::~SymbolTable()
{
	for(int i = 0; i < vars.size(); i++)
	{
		delete vars[i];
	}
	vars.clear();
}