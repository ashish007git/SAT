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
		int Scount;
		int UAcount;
		clause * cl;

		p_clause():status(0), Scount(0),UAcount(0){}

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
		vector<p_clause*> lc;
		
		lit(int id):val(false),id(id),assign(false),forced(false),visited(false){}
		
		~lit(){	
			pc.clear();
		}
		
	} lit;


	typedef struct f_clause{
	lit * fd;
	clause * fcl;
	
	f_clause(lit * fd, clause *fcl):fd(fd), fcl(fcl){}

	~f_clause(){}

	} f_clause;

	vector<lit*> literals;
	vector<p_clause*> clauses;
	vector<p_clause*> lclauses;

	void initiate_literals(){
		lit * lt;
		lt = new lit(0);
		literals.push_back(lt);
		return;

		}


};
