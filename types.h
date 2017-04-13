


typedef struct lit{
	int val;
	int id;
	vector<p_clause> c;	
	
	lit(int id):val(-1),id(id){}

	
} lit;


typedef struct p_clause{

	int status;
	bool learnt;
	clause * cl;
//	vector<lit> list;

	p_clause(int status, bool learnt):status(status), learnt(learnt){}
} p_clause;

// Secondary copy for quicker BCP.
typedef struct clause{

	vector<int> list;
}

