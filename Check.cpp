/*
 * Solver.cpp
 *
 *  Created on: Apr 27, 2017
 *      Author: raghavv
 */

#include<iostream>
#include"parse_c.h"
#include"Solver.h"
#include<fstream>
#include<string>
#include<vector>
#include<ctime>


using namespace SAT;
using namespace std;
//Datastructure
//vector of Lits


int main (int argc,char ** argv)
{
	string filename;
	clock_t t;

	//cout <<"My Solver is " << mySolver << endl;


	if(argc == 0)
	{
		cout << " Missing arguments" << endl;
	}
	else
	{
		filename = argv[1];
	}

	initiate_literals();
	if( parsefile(filename) == -1)
	{
		cout << "ERROR in parsing file. EXIT!!" << endl;
		return 0;
	}
	initiateimparr();

	//cout << "Done parsing and created data structures" << endl;

	t = clock();

	//if(!preSolve()) cout << "RESULT: UNSAT" << endl;
	if(!Solve()) cout << "RESULT: UNSAT" << endl;

	t = clock() - t;
	cout << "Solver Computation time is " << ((float)t/CLOCKS_PER_SEC) << " s" << endl;

	return 0;
}
