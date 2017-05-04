#include <stack>
#include <queue>
#include <algorithm>

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
	return literals.at(a->id-1 + 2*(a->id%2));

}


//To make first decison
lit * firstdecision(){

	lit* temp;
	temp = literals.at(1);
	temp->assign = true;
	getcomp(temp)->assign = true;
	return temp;

}

//Update vsid
bool updatevsid(clause * cl)
{
	for(int i = 0; i < cl->list.size(); i++)
	{
		literals.at(cl->list.at(i))->vsid++;
	}

	resetvsid++;
	if(resetvsid == 50)
	{
		for(int i = 0; i < literals.size(); i++)
		{
			literals.at(i)->vsid /= 4;
		}
		resetvsid = 0;
	}

	return true;
}

//Assign next unassigned literal.
lit * branchdecision(lit * temp){
	

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
		cout << "A " << mid << endl;	
		return literals.at(mid);
	}
}



//Undo changes
void Undo(lit * a)
{
	int i;
	lit * temp;
	temp = getcomp(a);
	cout << "U " << a->id << endl;

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

	return;
}

int addconflictclause(vector<int> &larray)
{
	p_clause * lc;
	clause	 * cl;
	int size;
	int i, l;
	int count;

	size = larray.size();
	count = 0;

 	//sort(larray.begin(), larray.end());

 	for(i = 0; i < size-1; i++)
	{
		if(larray.at(i) % 2 == 1)
		{
			if(larray.at(i)+1 == larray.at(i+1))
			{
				//Clause is redundant.
				cout<<"ERROR!! Conflict clause is invalid!!\n";
				exit(0);
			}
		}
	}

	lc = new p_clause();

	//make clause
	cl = new clause;
	lc->cl = cl;

	/*for(i = 0; i < size; i++)
	{
		cout<< larray.at(i) << "..";
	}
	cout<<"\n";*/


	for(i = 0; i < size; i++)
	{
		l = larray.at(i);

		if(!literals.at(l)->assign) count++;

		//make clause
		cl->list.push_back(l);
		literals.at(l)->lc.push_back(lc);
	}

	lc->UAcount = count; //TBD
	lclauses.push_back(lc);

	return 0;
}

//Remove conflict clause
bool removelc(int id, p_clause * lc)
{

	vector<p_clause*>::iterator it;

	it = find(literals.at(id)->lc.begin(), literals.at(id)->lc.end(), lc);

	if(it != literals.at(id)->lc.end())
	{
		literals.at(id)->lc.erase(it);
		return true;
	}

	return false;
}

bool removeconflictclause(p_clause * lc)
{
	clause * cl;
	cl = lc->cl;
	
	for (int i = 0; i < cl->list.size(); i++)
	{
		removelc(cl->list.at(i),lc);
	}

	//remove from lclauses
	vector<p_clause*>::iterator it;

	it = find(lclauses.begin(), lclauses.end(),lc);

	if(it != lclauses.end())
	{
		lclauses.erase(it);
	}
	else return false;

	delete lc;

	return true;
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
				forced = findforceddecision(temp->pc.at(i)->cl);
				if(!forced->fd->forced)
				{
					forced->fd->forced = true;
					b->push(forced);
				}
			}
			if(temp->lc.at(i)->UAcount == 0) //Unresolved clause, all literals assigned => UNSAT clause.
				{
					ccl = temp->lc.at(i)->cl;
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

	for(i = 0; i < a->list.size()+b->list.size(); i++)
	{

		if(j < a->list.size() && k < b->list.size())
		{

			if(b->list.at(k) == d)
			{
				k++;
				continue;
			}
			if(a->list.at(j) == (d-1) + 2*(d%2))
			{
				j++;
				continue;
			}
			
			if(a->list.at(j) == b->list.at(k))
			{
				cl->list.push_back(a->list.at(j));
				j++;
				k++;
			}

			else if(  a->list.at(j) < b->list.at(k))
			{
					cl->list.push_back(a->list.at(j++));
			}


			else if( b->list.at(j) < a->list.at(k) )
			{
					cl->list.push_back(b->list.at(k++));

			}
		}

		else if ( k >= b->list.size() && j < a->list.size())
		{
			if(a->list.at(j) == (d-1) + 2*(d%2))
			{
				j++;
				continue;
			}
			cl->list.push_back(a->list.at(j++));
		}
		else if ( j >= a->list.size() && k < b->list.size())
		{
			if(b->list.at(k) == d)
			{
				k++;
				continue;
			}
			cl->list.push_back(b->list.at(k++));
		}

	}


	for(i = 0; i < a->list.size(); i++)
	{
		cout << a->list.at(i)<< " ";
	}

	for(i = 0; i < b->list.size(); i++)
	{
		cout << b->list.at(i)<< " ";
	}
	cout << endl;
	return cl;

}

//Learn clause and add it..
clause * learnconflict(clause * a, clause * b, int d)
{

	clause * lc;

	lc = resolution(a, b, d);

//	updatevsid(lc);
	//Form conflict clause 
//	addconflictclause(lc->list);


//	return NULL;
	return lc;

}

//Check if the decision is related to conflict
bool impdecision(int d, clause * cl)
{
/*
	int i;
	if(cl == NULL) return true;

	for( i = 0; i < cl->list.size(); i++)
	{
		if( (d -1 + 2*(d%2)) == cl->list.at(i)) return true;

	}
	return false;
*/

	return true;
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
		clause * ccl = NULL;
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

					cout << "C " << ccl->list.size() << " " << fcl->list.size() << endl;

					//Learn conflict clause:
					ccl = learnconflict(ccl, fcl, temp->id);

					while(!Q.empty())
						{
							Q.front()->fd->forced = false;
							delete Q.front();
							Q.pop(); //Empty queue upon conflicts.
						}
/*
					//Backtracking the current assignment immediately.
					Undo(temp);

					//Reset flags in Lit
					temp->visited = false;
					temp->forced  = false;
					temp->assign = false;
					getcomp(temp)->assign = false;
					S.pop(); */
				} 
			}
			else    //Backtrack or Undo.
			{
				//Add a flag for "not in conflict clause case" - TBD
				if(temp->forced == false && temp->visited == false && impdecision(temp->id,ccl))
				{
					backtrack = false;
					ccl = NULL;
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
