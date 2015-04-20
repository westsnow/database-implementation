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
		vector<TableNode*> tableNodes;

	public:

		Optimizer(Statistics *st);
		~Optimizer();

		void traverse(QueryPlanNode *root);
		void planQuery();
		void createTableNodes();
		void createJoinNodes();
		
		
};


class QueryPlanNode{
	
	public:
		vector <QueryPlanNode*> children;
		int cost;

		virtual string toString() = 0;

};

//Leaf node select from file!
class TableNode : public QueryPlanNode { 
	
	public:
		CNF cond;
		Record literal;
		char *tableName;
		char *tableAlias;
		int outPipeID;
		string fileName;
		Schema *outSchema;

		void relatedSelectCNF(AndList *boolean, Statistics *s);
		TableNode(char *name, char *alias, int outPipeID);
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
		Record literal;
		Schema *outSchema;

		void relatedJoinCNF(AndList *boolean, Statistics *s);
		JoinNode(int leftPipeID, int rightPipeID, int outPipeID);
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