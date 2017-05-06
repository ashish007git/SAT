#ifndef PARSE_C_H_
#define PARSE_C_H_

#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<stdlib.h>
#include"types.h"
#include<algorithm>

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

int addclause(int *larray)
{
	p_clause * pc;
	clause	 * cl;
	lit      * Lt;
	int i,j,l;

	i = 0;

	pc = new p_clause();
	
	//make clause
	cl = new clause;
 	pc->cl = cl; 		
	while(larray[i] != 0)
	{	
		l = abs(larray[i]);
		//create only required lit, 
		if(l > literals.size()/2)
			{
			for(j = literals.size()/2 + 1; j <= l; j++)
			{
				Lt = new lit(2*j-1);
				literals.push_back(Lt);
				Lt = new lit(2*j);
				literals.push_back(Lt);	
			}	

		}
		//make clause	
		if(larray[i] < 0)
		{
			cl->list.push_back(2*l-1);
			literals.at(2*l-1)->pc.push_back(pc);
		}
		if(larray[i] > 0)

		{
			cl->list.push_back(2*l);				
			literals.at(2*l)->pc.push_back(pc);

		}

		i++;

	}

	sort(cl->list.begin(), cl->list.end());
	pc->UAcount = cl->list.size();
	clauses.push_back(pc);
	return 0;

}


int parsefile(string filename)
{
	if(myDebug == HIGH) cout << __FUNCTION__ << endl;

	char c;
	int i=0;
	int rlits[1000];
	int n_clauses;
	int n_lits;
	
	ifstream infile(filename.c_str());
	if(infile.is_open())
	{
	do	
	{	
		infile.get(c);
		//cout << ".." << c  << ".." << endl;
		if(isspace(c))continue;
		if( c == 'c')
		{	
			//skip whole line
			while(c != '\n')infile.get(c);
			
		}
		else if( c == 'p')
		{
			//skip whole line
			while(c != '\n')infile.get(c);
			
			//skip  "p cnf"
			/*while(c != 'f')infile.get(c);

			infile.get(c);
			cout << ".." << c  << ".." << endl;
			while(isspace(c))infile.get(c);	
			cout << ".." << c  << ".." << endl;
			//n_clauses = getint(&infile,&c);
								
			while(isspace(c))infile.get(c);
			cout << ".." << c  << ".." << endl;
			//n_lits = getint(&infile,&c);

			//Make sure to finish parsing the line		
			if( c == '\n')continue;
		
			while(isspace(c))infile.get(c);		
			if( c == '\n')continue;
			else
			{
				cout << "Unexpected characters found. file format is wrong" << endl;
				return -1;
			}*/
		}				
		else if(isdigit(c)|| c=='-')
		{
			if(c == '0')
			{	
				rlits[i] = 0;						
				addclause(rlits);			
				i = 0;
											
			}
			else
			{
				rlits[i++] = getint(&infile,&c);
			}			 	
		} 								
		
	}
	while( !infile.eof());
	} 
	else
	{
		cout << "File cannot be opened" << endl;
		return -1;	
	}	

	if(myDebug != OFF) cout << "Clauses : " << clauses.size() << " Literals : " << literals.size() << endl;
	return 0;

}

#endif
