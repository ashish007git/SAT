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
	Solver_t mySolver = NHCL;

	/*
	 *  OFF => Basic print messages and Errors.
	 *
	 *  LOW => Literal decisions, SAT assignment (if any).
	 *
	 *  MED => Literal decisions, SAT assignment (if any), Forced decision Clauses and Conflict Clauses.
	 *
	 *  HIGH => In addition to the prints in MED, print function name every time its entered.
	 */
	enum Debug_t {OFF, LOW, MED, HIGH};
	Debug_t myDebug = OFF;


	// Secondary copy for quicker BCP.
	typedef struct clause{

		vector<int> list;
		
		clause(){}

		~clause(){
			list.clear();
		}
	} clause;

	typedef struct p_clause{

		int activity;
		int Scount;
		int UAcount;
		clause * cl;

		p_clause():activity(0), Scount(0),UAcount(0){}

		~p_clause(){
			delete cl;
		 }

	} p_clause;


	//Data structure for quicker decision making.
	typedef struct lit{
		int vsid;
		int id;
		bool assign;
		bool forced;
		//bool visited;
		vector<p_clause*> pc;	
		vector<p_clause*> lc;
		
		lit(int id):vsid(0),id(id),assign(false),forced(false){}//,visited(false){}
		
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
	int ** imparr;

	void initiateimparr()
	{
		if(myDebug == HIGH) cout << __PRETTY_FUNCTION__ << endl;

		imparr = new int*[literals.size()];
		for (int i = 0 ; i < literals.size(); i++)
		{
			imparr[i] = new int[literals.size()];
			for(int j = 0 ; j < literals.size(); j++)
			{
				imparr[i][j] = 0;
			}
		}
		return;
	}

	void initiate_literals(){
		if(myDebug == HIGH) cout << __PRETTY_FUNCTION__ << endl;

		lit * lt;
		lt = new lit(0);
		literals.push_back(lt);
		return;

		}
};
