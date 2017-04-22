/*
 * parser.cpp
 *
 *  Created on: Apr 22, 2017
 *      Author: raghavv
 */

#include<iostream>
#include<string.h>
#include<stdio.h>
using namespace std;

void parsefile(string filename)
{
	fpos_t pos;
	bool readNumbers;
	int num;
	int nClauses;
	int nVars;
	char c[10];
	char buf[100];
	string str;

	FILE* fin;
	fin = fopen(filename.c_str(), "r");

	readNumbers = false;

	cout<<"Reading from file!!\n";
	while(!feof(fin))
	{
		if(readNumbers == false)
		{
			fgetpos(fin, &pos);
			fscanf(fin, "%s", c);
			str = (string) c;
			if(!str.compare("c"))
			{
				fgets(buf, 100, fin);
			}
			else
			{
				if(!str.compare("p"))
				{
					fscanf(fin, "%s", c);
					str.clear();
					str = (string) c;
					if(!str.compare("cnf"))
					{
						fscanf(fin, "%d", &nClauses);
						fscanf(fin, "%d", &nVars);
					}
				}
				else
				{
					readNumbers = true;
					fsetpos(fin, &pos);
					fscanf(fin, "%d", &num);
					printf("..%d..", num);
				}
			}
			str.clear();
		}
		else
		{
			fscanf(fin, "%d", &num);
			printf("%d..", num);
			if(num == 0)
			{
				cout << " Add to clauses here\n";
				readNumbers = false;
			}
		}
	}
	fclose(fin);
}


int main()
{
	parsefile("/home/min/a/raghavv/workspace/SAT/src/input.txt");
	return 0;
}
