#include<iostream>
#include<vector>

using namespace std;



namespace SAT{

	// Secondary copy for quicker BCP.
	typedef struct clause{

		vector<int> list;

		clause(){}

		~clause(){
			list.clear();
		}
	} clause;

	typedef struct p_clause{

		int status;
		bool learnt;
		int Scount;
		int UAcount;
		clause * cl;

		p_clause( bool learnt):status(0), learnt(learnt),Scount(0),UAcount(0){}

		//Need a function to clear cl

		~p_clause(){ }

	} p_clause;


	//Datastructure for quicker decision making.
	typedef struct lit{
		bool val;
		int id;
		bool assign;
		bool forced;
		bool visited;
		vector<p_clause*> pc;

		lit(int id):val(false),id(id),assign(false),forced(false),visited(false){}

		~lit(){
			pc.clear();
		}

	} lit;


	vector<lit*> literals;
	vector<p_clause*> clauses;

	void initiate_literals(){
		lit * lt;
		lt = new lit(0);
		literals.push_back(lt);
		return;

		}


};
