#include<iostream>
#include<vector>

using namespace std;



namespace SAT{

	/*
	 * CBCP => Chronological backtracking, Conflict detection, No conflict Learning, No heuristic free decision.
	 *
	 * NCBCP => Non-chronological backtracking, Conflict detection, No conflict learning, No heuristic free decision.
	 *
	 * NHCL => Non-chronological backtracking, Conflict detection, Conflict learning, No heuristic free decision.
	 *
	 * HCL => Non-chronological backtracking, Conflict detection, Conflict learning, Heuristic free decision.
	 */
	enum Solver_t {CBCP, NCBCP, NHCL, HCL};
	Solver_t mySolver = NCBCP;


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

		~p_clause(){
			delete cl;
		 }

	} p_clause;


	//Datastructure for quicker decision making.
	typedef struct lit{
		int vsid;
		int id;
		bool assign;
		bool forced;
		bool visited;
		vector<p_clause*> pc;	
		vector<p_clause*> lc;
		
		lit(int id):vsid(0),id(id),assign(false),forced(false),visited(false){}
		
		~lit(){	
			pc.clear();
			lc.clear();
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
	int resetvsid;

	void initiate_literals(){
		lit * lt;
		lt = new lit(0);
		literals.push_back(lt);
		return;

		}


};
