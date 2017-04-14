#include<stdio.h>
#include<vector>



public class SAT{
	//Datastructure for quicker decision making.
	typedef struct lit{
		int val;
		int id;
		vector<p_clause*> pc;	
		
		lit(int id):val(-1),id(id){}
		
		~lit(){	
			delete c;
		}
		
	} lit;


	typedef struct p_clause{

		int status;
		bool learnt;
		clause * cl;
	//	vector<lit> list;

		p_clause( bool learnt):status(0), learnt(learnt){}

		~p_clause(){ }

	} p_clause;

	// Secondary copy for quicker BCP.
	typedef struct clause{

		vector<int> list;
		
		clause(){}

		~clause(){
			delete list;
		}
	} clause;
};
