#ifndef OPTIMIZER_H
#define OPTIMIZER_H
#include <string>
#include <vector>
#include "Comparison.h"
#include "Function.h"
#include "Statistics.h"


using namespace std;

class QueryPlanNode; class TableNode;

class Optimizer{
	private:
		QueryPlanNode *planRoot;
		Statistics *s;
		vector<TableNode> tableNodes;

	public:

		Optimizer(Statistics *st);
		~Optimizer();

		void planQuery();
		void createTableNodes();
		CNF* relatedSelectCNF(AndList *boolean, char *name, char *alias);
		
};


class QueryPlanNode{
	
	public:
		QueryPlanNode *children = NULL;
		int cost;

};

//Leaf node select from file!
class TableNode : public QueryPlanNode { 
	
	public:
		CNF cond;
		string tableName;
		string tableAlias;
		int outPipeID;
		string fileName;

		TableNode(string name, string alias, int outPipeID);
		string toString();

};

//Project Node
class ProjectNode : public QueryPlanNode { 
	public:
		int *keepMe;
		int numAttsInput;
		int numAttsOutput;	

		int inPipeID;
		int outPipeID;

		string toString(); 
};


class JoinNode : public QueryPlanNode { 
	public:
		int rightPipeID;
		int leftPipeID;
		int outPipeID;
		CNF cond;

		string toString();
};

class DuplicateRemovalNode : public QueryPlanNode {
	public:
		int inPipeID;
		int outPipeID;
		Schema mySchema;

		string toString();		
};

class SumNode : public QueryPlanNode {
	public:
		int inPipeID;
		int outPipeID;
		Function computeMe;

		string toString();	
};

class GroupByNode : public QueryPlanNode {
	
	public:
		int inPipeID;
		int outPipe;
		OrderMaker groupAtts;
		Function computeMe;

		string toString();
};

class WriteOutNode : public QueryPlanNode {
	public:
		int inPipe;
		string outFileName;
		Schema mySchema;

		string toString();
	
};


#endif /* OPTIMIZER_H */