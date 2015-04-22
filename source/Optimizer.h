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
		void createSumNodes();	
		void createProjectNodes();	
		void createDistinctNodes();
};


class QueryPlanNode{
	
	public:
		vector <QueryPlanNode*> children;
		double cost;
		Schema *outSchema;
		int outPipeID;

		virtual string toString() = 0;

};

//Leaf node select from file!
class TableNode : public QueryPlanNode { 
	
	public:
		CNF cond;
		Record literal;
		char *tableName;
		char *tableAlias;
		string fileName;
		

		void relatedSelectCNF(AndList *boolean, Statistics *s);
		TableNode(char *name, char *alias, int outPipeID);
		string toString();

};

//Project Node
class ProjectNode : public QueryPlanNode { 
	public:
		int *keepMe;
		int numAttsIn;
		int numAttsOut;	

		int inPipeID;
		ProjectNode(NameList* atts, QueryPlanNode* root, int pipeid);
		string toString(); 
};


class JoinNode : public QueryPlanNode { 
	public:
		int rightPipeID;
		int leftPipeID;
		CNF cond;
		Record literal;
		
		void relatedJoinCNF(AndList *boolean, Statistics *s);
		JoinNode(int leftPipeID, int rightPipeID, int outPipeID);
		string toString();
};

class DuplicateRemovalNode : public QueryPlanNode {
	public:
		int inPipeID;
		DuplicateRemovalNode(QueryPlanNode* root, int outPipeID);

		string toString();		
};

class SumNode : public QueryPlanNode {
	public:
		int inPipeID;
		Function computeMe;
		SumNode(struct FuncOperator* parseTree, QueryPlanNode* root, int outPipeID);
		string toString();	
};

class GroupByNode : public QueryPlanNode {
	
	public:
		int inPipeID;
		OrderMaker groupOrder;
		Function computeMe;
		GroupByNode(struct NameList* nameList, struct FuncOperator* parseTree, QueryPlanNode* root, int outPipeID);
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