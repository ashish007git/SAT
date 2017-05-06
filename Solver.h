#include <stack>
#include <queue>
#include <algorithm>
#include "utils.h"

using namespace SAT;
using namespace std;


//To make first decison
lit * firstdecision(){
	if(myDebug == HIGH) cout << __FUNCTION__ << endl;

	lit* temp;
	temp = literals.at(1);
	temp->assign = true;
	getcomp(temp)->assign = true;
	updateimp(getcomp(temp)->id);
	if(myDebug != OFF) cout << "A " << temp->id << endl;
	return temp;

}



//Assign next unassigned literal.
lit * branchdecision(lit * temp){
	if(myDebug == HIGH) cout << __FUNCTION__ << endl;

	int mvsid = -1;
	int mid = 0;

	for(int i = 1; i < literals.size(); i++)
	{
		temp = literals.at(i);
		if(temp->assign == false)
		{
			if(mvsid < temp->vsid)
			{
				mvsid = temp->vsid;
				mid   = temp->id;
			}
		}
	}
	

	if(!mid) return NULL;
	else 
	{
		literals.at(mid)->assign = true;
		getcomp(literals.at(mid))->assign = true;
		updateimp(getcomp(literals.at(mid))->id);
		if(myDebug != OFF) cout << "A " << mid << endl;
		return literals.at(mid);
	}
}


//Find Forced decisions
f_clause * findforceddecision(clause* cl)
{
	if(myDebug == HIGH) cout << __FUNCTION__ << endl;

	lit* temp;
	f_clause * fc;

	for(int i = 0; i < cl->list.size(); i++)
	{
		temp = literals.at(cl->list.at(i));
		if( !temp->assign)
		{

			fc = new f_clause(temp,cl);

			if(myDebug == MED || myDebug == HIGH)
			{
				cout << "FC : ";
				for(i = 0; i < cl->list.size(); i++)
				{
					cout << cl->list.at(i)<< " ";
				}
				cout << endl;
			}

			return fc;

		}
	}

	return NULL;
}


//Undo changes
void Undo(lit * a)
{
	if(myDebug == HIGH) cout << __FUNCTION__ << endl;

	int i;
	lit * temp;
	temp = getcomp(a);
	if(myDebug != OFF) cout << "U " << a->id << endl;

	for(i = 0; i < a->pc.size(); i++)
	{
		a->pc.at(i)->Scount--;
	}

	for(i = 0; i < temp->pc.size(); i++)
	{
		if(temp->pc.at(i)->Scount == 0)
			temp->pc.at(i)->UAcount++;
	}

	//For learnt clauses
	for(i = 0; i < a->lc.size(); i++)
	{
		a->lc.at(i)->Scount--;
	}

	for(i = 0; i < temp->lc.size(); i++)
	{
		if(temp->lc.at(i)->Scount == 0)
			temp->lc.at(i)->UAcount++;
	}

	//if(mySolver != CBCP)
	{
		if(a->forced)
		{
			undoimparr(a->id);
			undoimparr(temp->id);
		}
	}

	return;
}

//evaluate SAT clauses
bool evalSATclauses(lit * a)
{
	if(myDebug == HIGH) cout << __FUNCTION__ << endl;

	int i;
	if(a->pc.size() == 0) return false;

	for(i = 0; i < a->pc.size(); i++)
	{
		a->pc.at(i)->Scount++;
	}
	//Learnt
	for(i = 0; i < a->lc.size(); i++)
	{
		a->lc.at(i)->Scount++;
	}
	return true;
}


//evaluate UNSAT clauses.
bool evalUNSATclauses(lit * a, queue<f_clause*> *b, clause * &ccl)
{
	if(myDebug == HIGH) cout << __FUNCTION__ << endl;

	int i ;
	lit * temp;
	f_clause* forced;
	temp = getcomp(a);


	//Bug fix - UAcount for all clauses are to be updated for proper undo.
	for(i = 0; i < temp->lc.size(); i++)
	{
		if(temp->lc.at(i)->Scount == 0)
		{
			temp->lc.at(i)->UAcount--;
		}
	
	} 

	//Bug fix - UAcount for all clauses are to be updated for proper undo.
	for(i = 0; i < temp->pc.size(); i++)
	{
		if(temp->pc.at(i)->Scount == 0)
		{
			temp->pc.at(i)->UAcount--;
		}
	
	} 

	//Learnt clauses
	for(i = 0; i < temp->lc.size(); i++)
	{
		if(temp->lc.at(i)->Scount == 0)
		{
			if(temp->lc.at(i)->UAcount == 1)
			{
				//Find forced decision and insert into queue.
				forced = findforceddecision(temp->lc.at(i)->cl);
				if(!forced->fd->forced)
				{
					forced->fd->forced = true;
					b->push(forced);
				}
			}
			if(temp->lc.at(i)->UAcount == 0) //Unresolved clause, all literals assigned => UNSAT clause.
			{
				//if(mySolver != CBCP)
				{
					ccl = temp->lc.at(i)->cl;
					updateimparr(temp->id+ 2*(temp->id%2) -1,ccl);
				}
				return false;
			}
		}
	}


	//Original Set.
	for(i = 0; i < temp->pc.size(); i++)
	{
		if(temp->pc.at(i)->Scount == 0)
		{
			if(temp->pc.at(i)->UAcount == 1)
			{
				//Find forced decision and insert into queue.
				forced = findforceddecision(temp->pc.at(i)->cl);
				if(!forced->fd->forced)
				{
					forced->fd->forced = true;
					b->push(forced);
				}
			}
			if(temp->pc.at(i)->UAcount == 0) //Unresolved clause, all literals assigned => UNSAT clause.
			{
				//if(mySolver != CBCP)
				{
					ccl = temp->pc.at(i)->cl;

					if(myDebug == MED || myDebug == HIGH)
					{
						cout << "CCL : ";
						for(i = 0; i < ccl->list.size(); i++)
						{
							cout << ccl->list.at(i)<< " ";
						}
						cout << endl;
					}

					updateimparr(temp->id +2*((temp->id)%2)-1,ccl);
				}
				return false;
			}
		}
	}
	return true;
}



//Check if the decision is related to conflict
bool impdecision(int d, clause * cl)
{
	if(myDebug == HIGH) cout << __FUNCTION__ << endl;

	if(mySolver == CBCP) return true;
	else //Non-chronological backtracking.
	{
		int i;
		if(cl == NULL) return true;
		for( i = 0; i < cl->list.size(); i++)
		{
			if( (d -1 + 2*(d%2)) == cl->list.at(i)) return true;
		}
		return false;
	}
}

bool checkSAT()
{
	int i;
	p_clause* temp;
	for( i = 0; i < clauses.size(); i++)
	{
		temp = clauses.at(i);
		if(clauses.at(i)->Scount == 0)
		{
			return false;
		}
	}

	return true;
}



//evaluate UNSAT clauses.
bool preevalUNSATclauses(lit * a, queue<lit*> *b, clause * &ccl)
{
	if(myDebug == HIGH) cout << __FUNCTION__ << endl;

	int i ;
	lit * temp;
	f_clause* forced;
	temp = getcomp(a);


	//Bug fix - UAcount for all clauses are to be updated for proper undo.
	for(i = 0; i < temp->pc.size(); i++)
	{
		if(temp->pc.at(i)->Scount == 0)
		{
			temp->pc.at(i)->UAcount--;
		}
	}


	//Original Set.
	for(i = 0; i < temp->pc.size(); i++)
	{
		if(temp->pc.at(i)->Scount == 0)
		{
			if(temp->pc.at(i)->UAcount == 1)
			{
				//Find forced decision and insert into queue.
				forced = findforceddecision(temp->pc.at(i)->cl);
				if(!forced->fd->forced)
				{
					forced->fd->forced = true;
					b->push(forced->fd);
				}
				delete forced;
			}
			if(temp->pc.at(i)->UAcount == 0) //Unresolved clause, all literals assigned => UNSAT clause.
			{
				ccl = temp->pc.at(i)->cl;

				if(myDebug == MED || myDebug == HIGH)
				{
					cout << "CCL : ";
					for(i = 0; i < ccl->list.size(); i++)
					{
						cout << ccl->list.at(i)<< " ";
					}
					cout << endl;
				}

				return false;
			}
		}
	}
	return true;
}



bool preSolve(){
	if(myDebug == HIGH) cout << __FUNCTION__ << endl;

		p_clause* temp;
		queue<lit*> Q;
		lit* L;
		vector<int> slits;
		clause * ccl = NULL;
		vector<p_clause*>::iterator it;
		int n;
		int id;

		for(int i = 0; i < clauses.size(); i++)
		{
			temp = clauses.at(i);
			if(temp->UAcount == 1) //Clauses with Only 1 literal.
			{
				L = literals.at(temp->cl->list.at(0));
				cout << "L " << L->id << endl;
				L->assign = true;
				L->forced = true;
				getcomp(L)->assign = true;
				getcomp(L)->forced = true;
				Q.push(L);

				it = find(L->pc.begin(), L->pc.end(), temp);
				if(it != L->pc.end()) L->pc.erase(it);

				it = find(clauses.begin(), clauses.end(), temp);
				if(it != clauses.end()) clauses.erase(it);
				i--;
			}
		}

		for(int i = 0; i < clauses.size(); i++)
		{
			temp = clauses.at(i);
			for(int j = 0; j < temp->cl->list.size(); j++)
				cout << " " << temp->cl->list.at(j);
			cout << endl;
		}

		if(Q.empty()) return true; //No Single literal clauses.

		while(!Q.empty())
		{
			L = Q.front();
			cout << L->id << ".." << endl;
			Q.pop();
			slits.push_back(L->id);

			evalSATclauses(L);

			if(!preevalUNSATclauses(L, &Q, ccl))
			{
				cout << "Conflict detected!" << endl;
				return false; //Conflict in preSolve stage.
			}
		}

		if(checkSAT())
		{
			cout << "RESULT: SAT" << endl;
			cout << "ASSIGNMENT: " << endl;

			for(int j = 0; j < slits.size(); j++)
			{
				id = slits.at(j);
				n = (int)(id/2) - (id)*(id%2);
				cout << n << " ";
			}
			cout << endl;

			return true;
		}

		if(myDebug == OFF) cout << "Clauses : " << clauses.size() << " Literals : " << literals.size() << endl;

		return false;
}



bool Solve(){
	if(myDebug == HIGH) cout << __FUNCTION__ << endl;

		lit* temp;
		stack<lit*> S;
		queue<f_clause*> Q;
		bool backtrack = false;
		clause * ccl = NULL;
		clause * fcl;
		vector<int> slits;
		int n;

		//cout << "Started solving" << endl;
		//To decide first variable
		temp = firstdecision(); //FF


		S.push(temp);

		while(! S.empty())
		{
			temp = S.top();

			//Code for forward traversal
			if(backtrack == false)
			{

				//Classify Satisfied and Unsatisfied clauses with current assignment.
				evalSATclauses(temp); //FF

				//Find forced decisions using BCP and populate the queue. - TBD
				//Check for conflicts along with forced decisions.
				//If no conflicts Pop the head of the queue, (if not empty) and push it into the stack. - TBD
				if(evalUNSATclauses(temp, &Q, ccl)) //FF
				{

					//No conflict:
					//Check for SATISFIABILITY, if YES exit.
					if(checkSAT())
					{
						cout << "RESULT: SAT" << endl;
						cout << "ASSIGNMENT: " << endl;

						while(!S.empty())
						{
							temp = S.top();
							S.pop();
							n = (int)(temp->id/2) - (temp->id)*(temp->id%2);
							slits.push_back(n);
						}

						for(int j = slits.size()-1; j >= 0; j--)
						{
							cout << slits.at(j) << " ";
						}
						cout << endl;

						return true; //FF
					}

					//If NO then queue empty, pick a variable based on heuristics and select its value.
					//And, push it into the stack as the next free decision.
					if(!Q.empty())
					{
						temp = Q.front()->fd;
						fcl  = Q.front()->fcl;
						/*if(mySolver != CBCP)*/ updateimparr(temp->id + 2*(temp->id%2) -1, fcl);
						delete Q.front();
						Q.pop();
						{
							temp->forced = true;
							temp->assign = true;
							getcomp(temp)->assign = true;
							if(myDebug != OFF) cout << "F " << temp->id << endl;
							S.push(temp);
						}
					}
					else
					{
						temp = branchdecision(temp); //FF
						if(temp != NULL) S.push(temp);
						else backtrack = true;
					}
				}
				else
				{
					//If conflict:
					//Learnt conflict clause is to be added to clauses - TBD
					backtrack =  true;

					//Learn conflict clause only when mySolver is NOT in CBCP.
					if(mySolver != CBCP) ccl = learnconflict(ccl, fcl, temp->id);

					while(!Q.empty())
					{
						Q.front()->fd->forced = false;
						delete Q.front();
						Q.pop(); //Empty queue upon conflicts.
					}
				} 
			}
			else    //Backtrack or Undo.
			{
				//Add a flag for "not in conflict clause case" - TBD
				if(temp->forced == false && impdecision(temp->id,ccl))
				{
					backtrack = false;
					Undo(temp); //FF

					//Reset flags in Lit
					//temp->visited = false;
					temp->forced  = false;
					S.pop();

					temp = getcomp(temp); //FF
					//temp->visited = true;
					temp->forced = true;
					impclearrow(temp->id);
					/*if(mySolver != CBCP)*/ updateimparr(temp->id + 2*(temp->id%2) -1, ccl);
					if(myDebug != OFF) cout << "B " << temp->id << endl;
					ccl = NULL;
					S.push(temp);

				}
				else
				{
					//Undo changes to clauses
					backtrack = true;
					Undo(temp);

					//Free decision on backtrack, but not present in conflict clause.
					if(!temp->forced) impclearrow(temp->id);

					//Reset flags in Lit
					//temp->visited = false;
					temp->forced  = false;
					temp->assign = false;
					getcomp(temp)->assign = false;
					S.pop();
				}
			}
		}

		//if(myDebug == OFF) cout << "Clauses : " << clauses.size() << " Literals : " << literals.size() << endl;

	return false;
}
