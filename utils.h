
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
	
	return cl;
}
