// WordWrapTestCPP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "WordWrapTestCPP.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include <set>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{

	if (argc < 2)
	{
		cout << "Please specify file name";
		return -1;
	}

	clock_t begin = clock();

	TCHAR* fileName = argv[1];

	ifstream ifs;
	ifs.open(fileName, ios::binary);
	if (!ifs.is_open())
	{
		cout << "Cannot open file";
		return -2;
	}
	ifs.seekg(0, ios::end);
	int length = ifs.tellg();
	ifs.seekg(0, ios::beg);
	char * buffer;
	buffer = new char[length];
	ifs.read(buffer, length);
	ifs.close();


	clock_t end = clock();
	double elapsed = (double)(end - begin) / CLOCKS_PER_SEC;

	cout << "Opening file: " << elapsed << " sec" << endl;

	// word wrap
	vector<int> lineStartIndices;
	const int charWidth = 1;
	const int textWidth = 30;
	//set<char> separators = { ' ', '\t', '\r' };
	const int sepCount = 3;
	char separators[] = " \t\r";
	char newLine = '\n';

	char* text = buffer;

	begin = clock();


	int lastPossibleLineStart = 0;
	int currWidth = 0;
	bool wasBlank = false;
	int lastWordStart = 0;
	int lastWordWidth = 0;
	bool wordStarted = false;

	int j = 0;
	while (j < length)
	{
		char c = text[j];
		//bool isBlank = separators.find(c) != separators.end();
		bool isBlank = false;
		for (int x = 0; x < sepCount; x++)
			if (separators[x] == c)
			{
				isBlank = true;
				break;
			}

		currWidth += charWidth;

		if (wasBlank)
		{
			if (!isBlank) { lastWordStart = j; lastWordWidth = 1; wordStarted = true; }
		}
		else
			if (isBlank) { wordStarted = false; }

		lastWordWidth += charWidth;

		if ((wordStarted && currWidth > textWidth) || j >= length - 1 || c == newLine)
		{
			lineStartIndices.emplace_back(lastPossibleLineStart);
			lastPossibleLineStart = lastWordStart;
			currWidth = lastWordWidth;
			wordStarted = false;
		}
		wasBlank = isBlank;
		j++;
	}

	end = clock();
	elapsed = (double)(end - begin) / CLOCKS_PER_SEC;

	cout << "Number of lines: " << lineStartIndices.size() << endl;
	cout << "Word wrap: " << elapsed << " sec" << endl;

	for (int i = 0; i < 50 && i < lineStartIndices.size(); i++)
	{
		int lineEnd = i < lineStartIndices.size() - 1 ? lineStartIndices[i + 1] : length;
		for (int x = lineStartIndices[i]; x < lineEnd; x++) cout << buffer[x];
		cout << endl;
	}


	delete[]buffer;

	system("Pause");

	return 0;

}
