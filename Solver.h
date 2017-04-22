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
				cout << "A " << temp->id << endl;
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

	cout << "U " << a->id << endl;
	return;
}

//Find Forced decisions
lit* findforceddecision(clause* cl)
{
	lit* temp;

	for(int i = 0; i < cl->list.size(); i++)
	{
		temp = literals.at(cl->list.at(i));
		if( !temp->assign)
		{
			return temp;
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
bool evalUNSATclauses(lit * a, queue<lit*> *b)
{

	int i ;
	lit * temp;
	lit* forced;
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
				cout << "F " << forced->id << endl;
			}
			if(temp->pc.at(i)->UAcount == 0) //Unresolved clause, all literals assigned => UNSAT clause.
				return false;
		}
	}
	return true;
}

bool checkSAT()
{
	int i;
	p_clause* temp;
	for( i = 0; i < clauses.size(); i++)
	{
		temp = clauses.at(i);
		cout << temp->Scount << " ";
		if(clauses.at(i)->Scount == 0)
		{
			cout << "\n";
			return false;
		}
	}
	cout << "\n";

	return true;
}

bool Solve(){
		lit* temp;
		stack<lit*> S;
		queue<lit*> Q;
		bool backtrack = false;

		cout << "Started solving" << endl;
		//To decide first variable
		temp = firstdecision(); //FF

		cout << "A " << temp->id << endl;
		cout << "\n" << clauses.size() << "\n";
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
				if(evalUNSATclauses(temp, &Q)) //FF
				{

					//No conflict:
					//Check for SATISFIABILITY, if YES exit.
					if(checkSAT())return true; //FF

					//If NO then queue empty, pick a variable based on heuristics and select its value.
					//And, push it into the stack as the next free decision.
					if(!Q.empty())
					{
						temp = Q.front();
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
					while(!Q.empty()) Q.pop(); //Empty queue upon conflicts.

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
				if(temp->forced == false && temp->visited == false)
				{
					backtrack = false;
					Undo(temp); //FF

					//Reset flags in Lit
					temp->visited = false;
					temp->forced  = false;
					S.pop();

					temp = getcomp(temp); //FF
					temp->visited = true;
					cout << "B " << temp->id << endl;
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
