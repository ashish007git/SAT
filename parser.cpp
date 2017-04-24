/*
 * parser.cpp
 *
 *  Created on: Apr 22, 2017
 *      Author: raghavv
 */

#include<iostream>
#include<string.h>
#include<stdio.h>
#include<vector>
using namespace std;

int parsefile(string filename)
{
	bool readNumbers;
	int num;
	int nClauses;
	int nVars;
	char c1;
	char c[4];
	char buf[100];
	string str = "cnf";
	vector<int> rlits;

	FILE* fin;
	fin = fopen(filename.c_str(), "r");
	/*if(fin == NULL)
	{
		cout<<"ERROR!!! Opening the file\n";
		return -1;
	}*/

	readNumbers = false;

	cout<<"Reading from file!!\n";

	while(!feof(fin))
	{
		if(readNumbers == false)
		{
			c1 = (char) fgetc(fin);
			if(c1 == ' ' || c1 == '\n')
			{
				continue;
			}

			if(c1 == 'c')
			{
				fseek(fin, -1, SEEK_CUR);
				fgets(buf, 100, fin);
			}
			else
			{
				if(c1 == 'p')
				{
					fscanf(fin, "%s", c);
					if(!str.compare(c))
					{
						fscanf(fin, "%d", &nClauses);
						fscanf(fin, "%d", &nVars);
					}
				}
				else
				{
					readNumbers = true;
					fseek(fin, -1, SEEK_CUR);
					fscanf(fin, "%d", &num);

					if(num != 0)
					{
						rlits.push_back(num);
					}
					else
					{
						fseek(fin, -1, SEEK_CUR);
						fgets(buf, 100, fin);
					}
				}
			}
		}
		else
		{
			fscanf(fin, "%d", &num);
			if(num == 0)
			{
				addclause(rlits);
				rlits.clear();
				readNumbers = false;
			}
			else
			{
				rlits.push_back(num);
			}
		}
	}
	fclose(fin);

	return 0;
}


int main()
{
	parsefile("/home/min/a/raghavv/workspace/SAT/src/input.txt");
	return 0;
}
