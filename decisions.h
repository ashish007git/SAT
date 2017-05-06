#ifndef DECISIONS_H_
#define DECISIONS_H_

#include "utils.h"
#include "types.h"

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

#endif /* DECISIONS_H_ */
