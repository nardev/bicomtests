//============================================================================
// Name        : main.cpp
// Author      : Vedran Alajbegovic
// Version     : 1
// Copyright   : open/free
// Description : #2 IP address lookup (Bonus) - BICOM test
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
#include <vector>
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

	if (ip_addr >= net_lower && ip_addr <= net_upper)
	return true;

	return false;
}

static int callbackDB(void *table, int argc, char **argv, char **szColName)
{
	vector<vector<string> > &result = *(vector<vector<string> >*)table;
	vector<string> list;

	for(int i = 0; i < argc; i++){
		list.push_back(argv[i]);
	}

	result.push_back(list);
	return 0;
}

int main(int argc, char* argv[]) {
	string str = argv[1];

	// very basic check
	if (str != "--database") { cout << "Error: missing --database argument!" << endl; return 0;}
	if (argv[2] == NULL) { cout << "Error: database file!" << endl; return 0;}
	string ip = argv[3];
	if (argv[3] == NULL) { cout << "Error: no ip address!" << endl; return 0;}

	sqlite3 *db;
	char *dbErrMsg = 0;
	const char *querry;
	std::vector<vector<string> > table;

	int rc = sqlite3_open(argv[2], &db);

	querry = "SELECT * FROM addresses";

	if(rc)
	{
		cout << "Error: Can't open database" << endl;
		return 0;
	}

	rc = sqlite3_exec(db, querry, callbackDB, &table, &dbErrMsg);

	if(rc != SQLITE_OK)
	{
		cout << "SQL Error: " << dbErrMsg << std::endl;
		sqlite3_free(dbErrMsg);
	}
	if(db)
	{
		sqlite3_close(db);
	}

	if (table.size() != 0)
	{
		for (unsigned int i = 0 ; i < table.size(); i++){
			vector<string> row(table[i]);
			if (isInRange(ip, row[0], row[1]))
				cout << row[0] << "/" << row[1]<< endl; // belongs to a network
		}
	} else {
		cout << "IP doesn't belong to any network";
	}

	return 0;
}

