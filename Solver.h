#include <stack>
#include <queue>

using namespace SAT;
using namespace std;

//Get complement of give literal
lit * getcomp(lit * a)
{

	if(a->id == 0)
	{
		cout<< "Invalid variable to get comp" << endl;
		return NULL;
	}
	if(a->id%2 == 0) return literals.at(a->id-1);
	else return literals.at(a->id+1);

}


//To make first decison
lit * firstdecision(){

	lit* temp;
	temp = literals.at(1);
	temp->assign = true;
	getcomp(temp)->assign = true;
	return temp;

}

//Assign next unassigned literal.
lit * branchdecision(lit * temp){
	{
		for(int i = 1; i < literals.size(); i = i+2)
		{
			temp = literals.at(i);
			if(temp->assign == false)
			{
				temp->assign = true;
				getcomp(temp)->assign = true;
				return temp;
			}
		}
	}
	return NULL;
}


//Undo changes
void Undo(lit * a)
{
	int i;
	lit * temp;
	temp = getcomp(a);

	for(i = 0; i < a->pc.size(); i++)
	{
		a->pc.at(i)->Scount--;
	}

	for(i = 0; i < temp->pc.size(); i++)
	{
		if(temp->pc.at(i)->Scount == 0)
			temp->pc.at(i)->UAcount++;
	}

	return;
}

//Find Forced decisions
f_clause * findforceddecision(clause* cl)
{
	lit* temp;
	f_clause * fc;

	for(int i = 0; i < cl->list.size(); i++)
	{
		temp = literals.at(cl->list.at(i));
		if( !temp->assign)
		{
			fc = new f_clause(temp,cl);
			return fc;
		}
	}

	return NULL;
}

//evaluate SAT clauses
bool evalSATclauses(lit * a)
{
	int i;
	if(a->pc.size() == 0) return false;

	for(i = 0; i < a->pc.size(); i++)
	{
		a->pc.at(i)->Scount++;
	}
	return true;
}


//evaluate UNSAT clauses.
bool evalUNSATclauses(lit * a, queue<f_clause*> *b, clause * ccl)
{

	int i ;
	lit * temp;
	f_clause* forced;
	temp = getcomp(a);
	for(i = 0; i < temp->pc.size(); i++)
	{
		if(temp->pc.at(i)->Scount == 0)
		{
			temp->pc.at(i)->UAcount--;
			if(temp->pc.at(i)->UAcount == 1)
			{
				//Find forced decision and insert into queue.
				forced = findforceddecision(temp->pc.at(i)->cl);
				b->push(forced);
			}
			if(temp->pc.at(i)->UAcount == 0) //Unresolved clause, all literals assigned => UNSAT clause.
				{
					ccl = temp->pc.at(i)->cl;
					return false;
				}
			}
	}
	return true;
}

clause * resolution(clause * a, clause * b, int d)
{
	int i,j,k;
	clause * cl = new clause;

	j = 0;
	k = 0;

	for(i = 0; i <= a->list.size()+b->list.size(); i++)
	{
		if(a->list.at(j) == b->list.at(k))
		{
			cl->list.push_back(a->list.at(j));
			j++;
			k++;
		}

		if( ( a->list.at(j) < b->list.at(k)) && j < a->list.size())
		{
				cl->list.push_back(a->list.at(j++));
		}
		else if(k < b->list.size())
		{
				cl->list.push_back(b->list.at(k++));
		}

		if( (b->list.at(j) < a->list.at(k) )  && k < b->list.size())
		{
				cl->list.push_back(b->list.at(k++));
		}
		else if(j < a->list.size())
		{
				cl->list.push_back(a->list.at(j++));
		}

	}

	for(i = 0; i < cl->list.size(); i++)
	{
		if(d%2 == 0)
		{
			if(cl->list.at(i) == d)
				cl->list.erase(cl->list.begin() + i);
				cl->list.erase(cl->list.begin() + i-1);
		}
		else
		{
			if(cl->list.at(i) == d)
				cl->list.erase(cl->list.begin() + i);
				cl->list.erase(cl->list.begin() + i);
		}
	}

	return cl;

}

//Learn clause and add it..
clause * learnconflict(clause * a, clause * b, int d)
{

	clause * lc;

	lc = resolution(a, b, d);
	//Form conflict clause
//	addconflictclause(lc);



	return lc;

}

//Check if the decision is related to conflict
bool impdecision(int d, clause * cl)
{
	int i;
	if(cl == NULL) return false;

	for( i = 0; i < cl->list.size(); i++)
	{
		if(d == cl->list.at(i)) return true;

	}
	return false;
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

bool Solve(){
		lit* temp;
		stack<lit*> S;
		queue<f_clause*> Q;
		bool backtrack = false;
		clause * ccl;
		clause * fcl;

		cout << "Started solving" << endl;
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
					if(checkSAT())return true; //FF

					//If NO then queue empty, pick a variable based on heuristics and select its value.
					//And, push it into the stack as the next free decision.
					if(!Q.empty())
					{
						temp = Q.front()->fd;
						fcl  = Q.front()->fcl;
						delete Q.front();
						Q.pop();
						{
							temp->forced = true;
							temp->assign = true;
							getcomp(temp)->assign = true;
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

					//Learn conflict clause:
					ccl = learnconflict(ccl, fcl, temp->id);

					while(!Q.empty())
						{
							delete Q.front();
							Q.pop(); //Empty queue upon conflicts.
						}

					//Backtracking the current assignment immediately.
					Undo(temp);

					//Reset flags in Lit
					temp->visited = false;
					temp->forced  = false;
					temp->assign = false;
					getcomp(temp)->assign = false;
					S.pop();
				}
			}
			else    //Backtrack or Undo.
			{
				//Add a flag for "not in conflict clause case" - TBD
				if(temp->forced == false && temp->visited == false && impdecision(temp->id,ccl))
				{
					backtrack = false;
					Undo(temp); //FF

					//Reset flags in Lit
					temp->visited = false;
					temp->forced  = false;
					S.pop();

					temp = getcomp(temp); //FF
					temp->visited = true;
					S.push(temp);

				}
				else
				{
					//Undo changes to clauses
					backtrack = true;
					Undo(temp);

					//Reset flags in Lit
					temp->visited = false;
					temp->forced  = false;
					temp->assign = false;
					getcomp(temp)->assign = false;
					S.pop();
				}
			}
		}

	return false;
}
