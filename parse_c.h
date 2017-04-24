#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<algorithm>
#include<stdlib.h>
#include"types.h"

using namespace std;
using namespace SAT;

int getint(ifstream *inf, char *c)
{
	int neg,val,factor;
	int i = 0;
	char buf[32];

	val = 0;
	factor  = 1;
	neg = 1;

	if(*c == '-')
	{
		neg = -1;
		inf->get(*c);
	}

	do
	{
		buf[i++] = *c;
		inf->get(*c);

	}while(*c != ' ' && *c!= '\n');

	for(i--; i >=0 ; i--)
	{
		val +=	factor*(int)(buf[i]-'0');
		factor = factor*10;
	}

	return neg*val;
}


int addclause(vector<int> larray)
{
	p_clause * pc;
	clause	 * cl;
	lit      * Lt;
	int size;
	int i,j,l;
	int lim;

	size = larray.size();

 	sort(larray.begin(), larray.end());
 	lim = max(abs(larray.at(0)), abs(larray.at(size-1)));

 	for(i = 0; i < size-1; i++)
	{
		if(larray.at(i) < 0)
		{
			for(j = i+1; j < size; j++)
			{
				if(larray.at(j) == -larray.at(i))
				{
					//Clause is redundant.
					return 0;
				}
			}
		}
	}

	pc = new p_clause(false);

	//make clause
	cl = new clause;
	pc->cl = cl;

	//Create only the required Literals.
	if(lim > literals.size()/2)
	{
		for(j = literals.size()/2 + 1; j <= lim; j++)
		{
			Lt = new lit(2*j-1);
			literals.push_back(Lt);
			Lt = new lit(2*j);
			literals.push_back(Lt);
		}
	}

	/*for(i = 0; i < size; i++)
	{
		cout<< larray.at(i) << "..";
	}
	cout<<"\n";*/


	for(i = 0; i < size; i++)
	{
		l = abs(larray.at(i));

		if(i < size-1)
		{
			if(larray.at(i) == larray.at(i+1)) continue;
		}

		//make clause
		if(larray.at(i) < 0)
		{
			cl->list.push_back(2*l-1);
			literals.at(2*l-1)->pc.push_back(pc);
		}
		if(larray.at(i) > 0)
		{
			cl->list.push_back(2*l);
			literals.at(2*l)->pc.push_back(pc);
		}
	}

	pc->UAcount = cl->list.size();
	clauses.push_back(pc);

	return 0;
}

int parsefile(string filename)
{
	char c;
	int i=0;
	int n_clauses;
	int n_lits;
	vector<int> rlits;

	ifstream infile(filename.c_str());
	if(infile.is_open())
	{
	do
	{
		infile.get(c);
		if(isspace(c))continue;
		if( c == 'c')
		{
			//skip whole line
			while(c != '\n')infile.get(c);

		}
		else if( c == 'p')
		{
			//skip  "p cnf"
			while(c != 'f')infile.get(c);

			infile.get(c);
			while(isspace(c))infile.get(c);
			n_clauses = getint(&infile,&c);

			while(isspace(c))infile.get(c);
			n_lits = getint(&infile,&c);

			//Make sure to finish parsing the line
			if( c == '\n')continue;

			while(isspace(c))infile.get(c);
			if( c == '\n')continue;
			else
			{
				cout << "Unexpected characters found. file format is wrong" << endl;
				return -1;
			}
		}
		else if(isdigit(c)|| c=='-')
		{
			if(c == '0')
			{
				addclause(rlits);
				rlits.clear();
			}
			else
			{
				rlits.push_back(getint(&infile,&c));
			}
		}

	}
	while( !infile.eof());
	}
	else
	{
		cout << "ERROR!! File cannot be opened." << endl;
		return -1;
	}

	return 0;

}
