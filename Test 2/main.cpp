//============================================================================
// Name        : main.cpp
// Author      : Vedran Alajbegovic
// Version     : 1
// Copyright   : open/free
// Description : #2 IP address lookup - BICOM test
//============================================================================


#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <ostream>
#include <sstream>
#include <sqlite3.h>

using namespace std;

string convertNetmask(const string mask)
{
	int tmp = atoi(mask.c_str());
	uint32_t x = 0xFFFFFFFF << (32 - tmp);
	ostringstream s;
	s << (x >> 24) << '.' << (x >> 16 & 0xFF) << '.' << (x >> 8 & 0xFF) << '.' << (x & 0xFF);
	return s.str();
}


unsigned long convertIP(const std::string ip)
{
	int a, b, c, d;
	unsigned long addr = 0;

	if (sscanf(ip.c_str(), "%d.%d.%d.%d", &a, &b, &c, &d) != 4)
		return 0;

	addr = a << 24;
	addr |= b << 16;
	addr |= c << 8;
	addr |= d;
	return addr;
}

bool isInRange(const string ip, const string network, const string mask)
{
	string msk = convertNetmask(mask);
	unsigned long ip_addr = convertIP(ip);
	unsigned long network_addr = convertIP(network);
	unsigned long mask_addr = convertIP(msk);
	unsigned long net_lower = (network_addr & mask_addr);
	unsigned long net_upper = (net_lower | (~mask_addr));

	if (ip_addr >= net_lower &&
		ip_addr <= net_upper)
		return true;

	return false;
}

int main(int argc, char* argv[]) {
	string str = argv[1];
	string line;

	// very basic check
	if (str != "--database") { cout << "Error: missing --database argument!" << endl; return 0;}
	if (argv[2] == NULL) { cout << "Error: database file!" << endl; return 0;}
	if (argv[3] == NULL) { cout << "Error: no ip address!" << endl; return 0;}
	string ip = argv[3];

	ifstream file(argv[2]);
	if (!file) { cout << "Error: File doesn't exist!" << endl; return 0; }

	while (getline(file,line))
	{
		string network = line.substr(0,line.find("/"));
		string mask = line.substr(line.find("/")+1,line.size());
		if (isInRange(ip, network , mask))
			cout << line << endl; // belongs to a network
	}


	return 0;
}

