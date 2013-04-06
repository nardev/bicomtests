//============================================================================
// Name        : main.cpp
// Author      : Vedran Alajbegovic
// Version     : 1
// Copyright   : open/free
// Description : #1 Terminal tables - BICOM test
//============================================================================

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {

	ifstream file(argv[1]); // vea add - should check if more spaces..
	if (!file) { cout << "Error: File doesn't exist!" << endl; return 0; }

	vector<string> mapa;
	string line, top, sub, chars,t;
	int longest = 0, num_of_lines = 0;

	while(getline(file,line)){
		// load and trim preserving spaces!
		t.clear();
		for (int i = 0; i <= line.size() ; i++)
		{
			if ((line[i] != ' ' && line[i+1] == ' ') || (line[i] == ' ' && line[i+1] == ' ') || (line[i] != ' ' && line[i+1] == NULL))
			{
				t += line[i];
				i++;
			}
		}
		mapa.push_back(t);
		num_of_lines++;
		if (line.size() > longest) longest = line.size();
	}

	string tmp(longest*4+1,' ');
	vector<string> frame(2*num_of_lines+1,tmp);

	for(int i = 0; i < mapa.size(); i++)
	{
		for(int j = 0; j <= mapa[i].size()-1; j++)
		{
			if (mapa[i][j] != ' ' && mapa[i][j] != NULL)
			{
				frame[2*i].replace(4*j,5,"+---+");
				frame[2*i+1].replace(4*j, 5,string("| ") + mapa[i][j] + string(" |"));
				frame[2*i+2].replace(4*j,5,"+---+");
			}
		}
	}

	for(int i; i < frame.size() ; i++){
		cout << frame[i] << endl;
	}

	return 0;
}



