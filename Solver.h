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
	
	cout << literals.size() << endl;
	return literals.at(1);

}

//To make Branch decision
lit * branchdecision(lit * temp){

	if(temp->id == literals.size()-1)return NULL;
	else
	{
		if(temp->id%2 ==0) return literals.at(temp->id+1) ;
		else return literals.at(temp->id + 2);
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
	temp = getcomp(a);
	for(i = 0; i < temp->pc.size(); i++)
	{
		if(temp->pc.at(i)->Scount == 0)
		{
			temp->pc.at(i)->UAcount--;
			if(temp->pc.at(i)->UAcount == 0)
				return false;
		}	
	}

	
	return true;
}

bool checkSAT()
{
	int i;
	for( i = 0; i < clauses.size(); i++)
	{
		if(clauses.at(i)->Scount == 0) return false;
	}
	
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

		cout << "Decided 1st branch " << endl;
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
						temp->forced = true;
						S.push(temp);

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
					S.pop();
				}
			}
		}




	return false;
}
