/*
 * utils.h
 *
 *  Created on: May 5, 2017
 *      Author: raghavv
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <algorithm>

using namespace SAT;
using namespace std;

//Get complement of give literal
lit * getcomp(lit * a)
{
	if(myDebug == HIGH) cout << __FUNCTION__ << endl;

	if(a->id == 0)
	{
		cout<< "Invalid variable to get comp" << endl;
		return NULL;
	}
	return literals.at(a->id-1 + 2*(a->id%2));

}

//Update vsid
bool updatevsid(clause * cl)
{
	if(myDebug == HIGH) cout << __FUNCTION__ << endl;

	if(cl == NULL) return true;

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


void updateimparr(int id, clause * cl)
{
	if(myDebug == HIGH) cout << __FUNCTION__ << endl;

	for(int i = 0 ; i < cl->list.size(); i++)
	{
		if((id+2*(id%2)-1) != cl->list.at(i))
		{
			if(getcomp(literals.at(cl->list.at(i)))->forced)
			{
				for(int j = 1; j < literals.size(); j++)
					imparr[j][id] = imparr[j][id] | imparr[j][cl->list.at(i)];

			}
			else imparr[cl->list.at(i)][id] = 1;
		}
	}

	return;
}

void impclearrow(int id)
{
	if(myDebug == HIGH) cout << __FUNCTION__ << endl;

	for(int j = 0; j < literals.size(); j++)
	{
		imparr[id][j] = 0;
		imparr[id-1 + 2*(id%2)][j] = 0;
	}

	return;
}

void undoimparr(int id)
{
	if(myDebug == HIGH) cout << __FUNCTION__ << endl;

	for(int j = 0; j < literals.size(); j++)
		imparr[j][id] = 0;

	return;
}

clause * impconflictclause(int id)
{
	if(myDebug == HIGH) cout << __FUNCTION__ << endl;

	clause * cl = new clause;
	for(int  i = 1 ; i < literals.size(); i++)
	{
		if(imparr[i][id] || imparr[i][id - 1 + 2*(id%2)])
			cl->list.push_back(i);
	}

	sort(cl->list.begin(),cl->list.end());
	return cl;
}


int addconflictclause(clause * cl)
{
	if(myDebug == HIGH) cout << __FUNCTION__ << endl;

	p_clause * lc;
	int size;
	int i, l;

	size = cl->list.size();

	lc = new p_clause();
	lc->cl = cl;

	for(i = 0; i < size; i++)
	{
		l = cl->list.at(i);

		//make clause
		literals.at(l)->lc.push_back(lc);
	}

	lc->UAcount = 0;
	lclauses.push_back(lc);

	return 0;
}

//Remove conflict clause
bool removelc(int id, p_clause * lc)
{
	if(myDebug == HIGH) cout << __FUNCTION__ << endl;

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
	if(myDebug == HIGH) cout << __FUNCTION__ << endl;

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


//Learn clause and add it..
clause * learnconflict(clause * a, clause * b, int d)
{
	if(myDebug == HIGH) cout << __FUNCTION__ << endl;

	clause * lc;

	lc = impconflictclause(d);

	if(myDebug == MED || myDebug == HIGH)
	{
		cout << "CONFLICT CLAUSE : ";
		for(int i = 0; i < lc->list.size(); i++)
		{
			cout << lc->list.at(i)<< " ";
		}
		cout << endl;
	}

	//This function is entered only when mySolver is NOT in CBCP.
	if(mySolver == NCBCP) return lc;
	else
	{
		//Form conflict clause
		if(mySolver == NHCL || mySolver == HCL) addconflictclause(lc);

		if(mySolver == HCL) updatevsid(lc);

		return lc;
	}
}

/*
clause * resolution(clause * a, clause * b, int d)
{
	if(myDebug == HIGH) cout << __FUNCTION__ << endl;

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
	if(myDebug == HIGH)
	{
		for(i = 0; i < a->list.size(); i++)
		{
			cout << a->list.at(i)<< " ";
		}
		for(i = 0; i < b->list.size(); i++)
		{
			cout << b->list.at(i)<< " ";
		}
		cout << endl;
	}
	return cl;
}
*/

#endif /* UTILS_H_ */
