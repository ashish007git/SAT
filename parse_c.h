#include<iostream>
#include<fstream>
#include<string>
#include<vector>

#include"types.h"

using namespace std;

int getint(ifstream *inf, char *c)
{	
	int val,factor;
	int i;
	char buf[32];
	
	val = 0;
	factor  = 1;
	neg = 1;
	
	if(*c == '-')
	{
		neg =- 1;
		*c = getc(*inf);
	}
	
	do
	{
		buf[i++] = *c;          
 		*c = getc(*inf);
                                       
	}while(*c != ' ' || *c!= '\n');
	
	for(i--; i >=0 ; i--)
	{
		val +=	factor*(int)(buf[i]-'0');
		factor = factor*10;
	}
	
	return val; 
}

int addclause(int *larray)
{
	p_clause * pc;
	clause	 * cl;
	lit      * Lt;
	int i,j,l;

	i = 0;

	pc = new p_clause(1,false);
	
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
			cl.list.push_back(2*l-1);
			literals.at(2*l-1).pc.push_back(pc);
		}
		if(larray[i] > 0)

		{
			cl.list.push_back(2*l);				
			literals.at(2*l).pc.push_back(pc);

		}

		i++;

	}

	return 0;

}


int parsefile(string filename)
{
	char c;
	int i;
	int rlits[1000];
	int n_clauses;
	int n_lits;
	
	ifstream infile(filename.c_str());
	if(infile.isopen())
	{
	do	
	{
		if(isspace(c))continue;
		if( c == 'c')
		{
			//skip whole line
			while(getc(infile) != '\n');
			
		}
		else if( c == 'p')
		{
			//skip  "p cnf"
			while(getc(infile) != 'f');

			while(isspace(c = getc(infile)));		
			n_clauses = getint(&infile,&c);
								
			while(isspace(c = getc(infile)));
			n_lits	  = getint(&infile,&c);						

			//Make sure to finish parsing the line		
			if( c == '\n')continue;
		
			while(isspace(c = getc(infile)));
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
				rlits[i] = 0;						
				addclause(rlits);			
				i = 0;
											
			}
			else
			{
				rlits[i++] = getint(&infile,&c);
			}			} 	
		} 								
		
	}
	while(( c = getc(infile))!= EOF);
	} 
	else
	{
		cout << "file not openable" << endl;
		return -1;	
	}	

}
