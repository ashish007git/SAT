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

void updateimparr(int id, clause * cl)
{
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

void undoimparr(int id)
{
	for(int j = 0; j < literals.size(); j++)
		imparr[j][id] = 0;

	return;
}

clause * impconflictclause(int id)
{
	clause * cl = new clause;
	for(int  i = 1 ; i < literals.size(); i++)
	{
		if(imparr[i][id] || imparr[i][id - 1 + 2*(id%2)])
			cl->list.push_back(i);
	}
	
	sort(cl->list.begin(),cl->list.end());
	return cl;
}

/*
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
*/
