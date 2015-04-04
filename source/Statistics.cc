#include "Statistics.h"


// typedef struct _RelStat{
// 	int numTuples;
// 	hash_map<char*, int> attInfo;
// } RelStat;




Statistics::Statistics()
{
}
Statistics::Statistics(Statistics &copyMe)
{
	relInfo = copyMe.relInfo;
}
Statistics::~Statistics()
{
	
}

bool Statistics::RelExists(char * relName){
	hash_map<char*, RelStat*>::iterator i = relInfo.find(relName);
	if( i == relInfo.end() )
		return false;
	return true;
}

void Statistics::AddRel(char *relName, int numTuples)
{
	if( !RelExists(relName) ){
		RelStat* tmp = new RelStat;
		tmp->numTuples = numTuples;
		relInfo[relName] = tmp;
	}else{
		relInfo[relName]->numTuples = numTuples;
	}
}
void Statistics::AddAtt(char *relName, char *attName, int numDistincts)
{
	if(RelExists(relName))
		relInfo[relName]->attInfo[attName] = numDistincts;
}
void Statistics::CopyRel(char *oldName, char *newName)
{
	if(RelExists(oldName)){
		RelStat *newRel = new RelStat( *(relInfo[oldName]) );
		relInfo[newName] = newRel;
	}
}
	
void Statistics::Read(char *fromWhere)
{
}
void Statistics::Write(char *fromWhere)
{
}

void  Statistics::Apply(struct AndList *parseTree, char *relNames[], int numToJoin)
{
}
double Statistics::Estimate(struct AndList *parseTree, char **relNames, int numToJoin)
{
}

