#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <sstream>

using namespace std;
int main()
{
 	ifstream record("records.dat");
	while(!record.eof())
	{
		string st;
		getline(record,st);
		char * tokens=strtok((char*)st.c_str()," \"");
		while(tokens!=0)
		{
			cout << tokens<< endl;
			tokens=strtok(NULL," \"");
		}
	}	
}
