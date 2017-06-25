#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include "global_routing.h"

using namespace std;

int main(int argc, char** argv)
{
	int nets;
	global_routing mans;

    if(argc != 3)
    {
        cout << "Usage: ./[exe] [input file] [output file]" << endl;
        exit(1);
    }

    // open the input file
    fstream fin;
    fin.open(argv[1], fstream::in);
    if(!fin.is_open())
    {
        cout << "Error: the input file is not opened!!" << endl;
        exit(1);
    }

    char buffer[100];

    fin >> buffer;
    string s = buffer;

	if (s == "grid") {
		fin >> buffer;
		int row = atoi(buffer);

		fin >> buffer;
		int column = atoi(buffer);

		mans.setAllGrid(row, column);
    }

	fin >> buffer;
	s = buffer;

	if (s == "capacity") {
		fin >> buffer;
		int capacity = atoi(buffer);

		mans.setCapacity(capacity);
	}

	fin >> buffer;
	s = buffer;
	fin >> buffer;
	s = buffer;

	if (s == "net") {
		fin >> buffer;
		nets = atoi(buffer);

		mans.setNetNum(nets);
	}

	for (int i = 0; i < nets; ++i) {
		fin >> buffer;
		int netID = atoi(buffer);
		fin >> buffer;
		int sx = atoi(buffer);
		fin >> buffer;
		int sy = atoi(buffer);
		fin >> buffer;
		int tx = atoi(buffer);
		fin >> buffer;
		int ty = atoi(buffer);
		mans.addline(netID, sx, sy, tx, ty);
	}

	mans.setOutFile(argv[2]);
	mans.start();
	return 0;
}

