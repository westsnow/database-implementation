#ifndef STATISTICS_
#define STATISTICS_
#include "ParseTree.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#ifdef __GNUC__
#include <ext/hash_map>
#else
#include <hash_map>
#endif

namespace std
{
using namespace __gnu_cxx;
}

using namespace std;

class RelStat{
public:
	int numTuples;
	hash_map<string, int> attInfo;
	RelStat(){}
	RelStat(RelStat &copyMe){
		numTuples = copyMe.numTuples;
		attInfo = copyMe.attInfo;
	}
	RelStat& operator=(const RelStat& other){
		numTuples = other.numTuples;
		attInfo = other.attInfo;
		return *this;
	}
	bool attrExists(string attrName);
	int getValue(string attName);
};



class Statistics
{
public:
	hash_map<string, RelStat*> relInfo;
	Statistics();
	Statistics(Statistics &copyMe);	 // Performs deep copy
	~Statistics();

	bool RelExists(string relName);

	void AddRel(char *relName, int numTuples);
	void AddAtt(char *relName, char *attName, int numDistincts);
	void CopyRel(char *oldName, char *newName);
	
	void Read(char *fromWhere);
	void Write(char *fromWhere);

	void  Apply(struct AndList *parseTree, char *relNames[], int numToJoin);
	double Estimate(struct AndList *parseTree, char **relNames, int numToJoin);
private:
	string getTableNameFromAttr(string attrName);

};

#endif
