#include<iostream>
#include"parse_c.h"
#include"Solver.h"
#include<fstream>
#include<string>
#include<vector>


using namespace SAT;
using namespace std;
//Datastructure
//vector of Lits


int main (int argc,char ** argv)
{

string filename;

if(argc == 0){
	cout << " Missing arguments" << endl;
 }
else
{
	filename = argv[1];
}

initiate_literals();
parsefile(filename);

cout << "Done parsing and created data structures" << endl;

if(Solve()) cout << "SATISFIABLE" << endl;
else cout << "UNSATISFIABLE" << endl;

	
	return 0;
}





