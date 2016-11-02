#include "parser.h"

int main(int argc, char** argv)
{
	if(argc != 3)
	{
		cerr << "Wrong number of arguments" << endl;
		return 0;
	}

	ifstream inputFile;
	ofstream outputFile;

	inputFile.open(argv[1]);
	outputFile.open(argv[2]);

	string token;

	// extract initial "<tokens>"
	advance(inputFile, token);

	advance(inputFile, token);
	
	classParser(inputFile, outputFile, token, 0);

	inputFile.close();
	outputFile.close();

	return 0;
}