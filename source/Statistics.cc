#include "Statistics.h"
#include <stdio.h>
#include <string.h>

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
	
void Statistics::Read(char *fromWhere){

	ifstream stat_file;
	string line;
	stat_file.open(fromWhere);
	char *relName;
	if(stat_file.is_open()){
		cout<<"Open";
	}
	while(getline(stat_file, line)){
		cout<<line<<endl;
		if(!line.substr(0,1).compare("-")!= 0){
			int idx = line.find(" ");
			relName = new char[line.length()+1];
			strcpy(relName, line.substr(1,idx-1).c_str());
			cout<<relName<<endl;
			cout<<line.substr(idx+1, line.length()-idx-1).c_str()<<endl;
			int numTuples = atoi(line.substr(idx+1, line.length()-idx-1).c_str());

			RelStat* tmp = new RelStat;
			tmp->numTuples = numTuples;
			relInfo[relName] = tmp;
		}
		else{
			int idx = line.find(" ");
			char *attName = new char [line.length()+1];
			strcpy(attName, line.substr(0,idx).c_str());
			cout<<attName<<endl;
			cout<<line.substr(idx+1, line.length()-idx-1).c_str()<<endl;
			int numDistincts = atoi(line.substr(idx+1, line.length()-idx-1).c_str());
			relInfo[relName]->attInfo[attName] = numDistincts;
		}
	}

}
void Statistics::Write(char *fromWhere){

	ofstream stat_file;

	// write
	stat_file.open(fromWhere);

	for (hash_map<char*, RelStat*>::iterator r = relInfo.begin();
			r != relInfo.end();
			++r) // for each item in the hash map:
	{
	    stat_file<<r->first<<" "<<r->second->numTuples<<endl;
	    for (hash_map<char*, int>::iterator att = r->second->attInfo.begin();
	    			att != r->second->attInfo.end();
	    			++att) // for each item in the hash map:
	    	{
	    		stat_file<<"-"<<att->first<<" "<<att->second<<endl;
	    	}
	}


	stat_file.close();

}

void  Statistics::Apply(struct AndList *parseTree, char *relNames[], int numToJoin)
{
}
double Statistics::Estimate(struct AndList *parseTree, char **relNames, int numToJoin)
{
}

