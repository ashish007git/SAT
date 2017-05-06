#include<iostream>
#include"parse_c.h"
#include"Solver.h"
#include<fstream>
#include<string>
#include<vector>
#include<ctime>


using namespace SAT;
using namespace std;


int main (int argc,char ** argv)
{
	string filename;
	clock_t t;

	if(argc == 0)
	{
		cout << " Missing arguments" << endl;
	}
	else
	{
		filename = argv[1];

		mySolver = HCL;
		myDebug = OFF;
	}

	initiate_literals();
	if( parsefile(filename) == -1)
	{
		cout << "ERROR in parsing file. EXIT!!" << endl;
		return 0;
	}
	initiateimparr();


	t = clock();

	//if(!preSolve()) cout << "RESULT: UNSAT" << endl;
	if(!Solve()) cout << "RESULT: UNSAT" << endl;

	t = clock() - t;
	cout << "Solver Computation time is " << ((float)t/CLOCKS_PER_SEC) << " s" << endl;

	return 0;
}
