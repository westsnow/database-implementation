#include "Optimizer.h"
#include "Schema.h"
#include <iostream>
#include <vector>


// from parser
extern FuncOperator* finalFunction;
extern TableList* tables;
extern AndList* boolean;
extern NameList* groupingAtts;
extern NameList* attsToSelect;
extern int distinctAtts;
extern int distinctFunc;

char* catalog_path = "/Users/Migue/Documents/DBIDATA/catalog";
int pipeid = 1;
using namespace std;

Optimizer::Optimizer(Statistics *st){
	s = st;
	s->init();
	planRoot = NULL;
}

void Optimizer::planQuery(){

	createTableNodes();
	createJoinNodes();
	traverse(planRoot);
}

//Work left to do!!!!!!! 
int evalOrder(vector<TableNode*> operands, Statistics *s, int minCost){
	
	return 0;

}

void Optimizer::createJoinNodes(){

	std::vector<TableNode*> tableList(tableNodes);
	sort (tableList.begin(),tableList.end());
	int minCost = INT_MAX;
	int cost;
	do {
		cost=evalOrder(tableList, s, minCost);
		
		// for(int i=0; i<tableList.size(); i++){
		// 	cout<<tableList[i]->tableName<<"-";
		// }
		//cout<<endl;
		if(cost < minCost && cost>0) {
			minCost = cost; 
			tableNodes = tableList; 
		}
	} while ( next_permutation(tableList.begin(), tableList.end()) );

	if(tableNodes.size()>1){
		TableNode* left = tableNodes[0];
		TableNode* right = tableNodes[1];

		JoinNode *prev = new JoinNode(left->outPipeID, right->outPipeID, pipeid);
		pipeid += 1;
		prev->children.push_back(left);
		prev->children.push_back(right);
		prev->relatedJoinCNF(boolean, s);
		//join->toString();
		//join->children[0]->toString();
		//join->children[1]->toString();
		for(int i=2; i<tableNodes.size(); i++){
			TableNode* right = tableNodes[i];
			JoinNode *join = new JoinNode(prev->outPipeID, right->outPipeID, pipeid);
			join->children.push_back(prev);
			join->children.push_back(right);	
			pipeid += 1;
			prev = join;
		}
		
		planRoot = prev;
	}
	
	

}

void Optimizer::createTableNodes(){
	
	
	TableList *tmp = tables;
	while(tmp != NULL){

		s->CopyRel(tmp->tableName, tmp->aliasAs);
		TableNode *t = new TableNode(tmp->tableName, tmp->aliasAs, pipeid);
		t->relatedSelectCNF(boolean, s);
		//t->toString();
		
		tableNodes.push_back(t);
		
		tmp = tmp->next;
		pipeid += 1;
	} 

}

void Optimizer::traverse(QueryPlanNode *root){
	
	if(!root->children.empty()){
		if(root->children.size() == 2){
			traverse(root->children[0]);
			root->toString();
			traverse(root->children[1]);
		}
	}else{
		root->toString();
	}

}


/*
	TableNode Methods
*/

//TableNode::TableNode(CNF cond, string tableName, string tableAlias, outPipeID, string fileName){
TableNode::TableNode(char *name, char *alias, int outPipeID){
	
	tableName = name;
	tableAlias =  alias;
	this->outPipeID = outPipeID;
	fileName = "";
}

void TableNode::relatedSelectCNF(AndList *boolean, Statistics *s){
	string name(tableName);
	string alias(tableAlias);
	AndList *andTmp = boolean;
	AndList *andFinal = NULL;
	

	while(andTmp != NULL){

		OrList *orList = andTmp->left;

		while(orList != NULL){
			struct ComparisonOp *pCom = orList->left;
			struct Operand* leftOperand = pCom->left;
			//printf("%d\n", leftOperand->code);
			struct Operand* rightOperand = pCom->right;
			//printf("%d\n", rightOperand->code);
			string leftValue(leftOperand->value);
			string rightValue(rightOperand->value);
			string tableNameOfLeft = s->getTableNameFromAttr(leftOperand->value);
			string tableNameOfRight = s->getTableNameFromAttr(rightOperand->value);

			if((leftOperand->code == NAME && rightOperand->code != NAME) || (leftOperand->code != NAME && rightOperand->code == NAME)){
				if((tableNameOfLeft.compare(alias) == 0) || (tableNameOfRight.compare(alias) == 0)){
					AndList *node = new AndList();
					if(andFinal == NULL){
						node->left = andTmp->left;
						node->rightAnd = NULL;
						
					}
					else{
						node->rightAnd = andFinal;
						node->left = andTmp->left;
					}
					andFinal = node;
					break;
				}
			}
			orList = orList->rightOr;
		}
		andTmp = andTmp->rightAnd;
	}

	outSchema = new Schema(catalog_path, tableName, tableAlias);
	
	if(andFinal != NULL){
		
		//Record literal;
		//need to check this!!!
		cond.GrowFromParseTree (andFinal, outSchema, literal);

		

	}else{
		/*
			Do we need to create dummy CNF for all rows in table?
		*/
		// Attribute *dummy = outSchema->GetAtts();
		// AndList *final = new AndList();
		// OrList *only = new OrList();
		// only->left->code = EQUALS;	
		// only->left->left->code = dummy->myType;
		// only->left->left->value = dummy->name;
		// only->left->right->code = dummy->myType;
		// only->left->right->value = dummy->name;
		// only->rightOr = NULL;
		// final->left = only;
		// final->rightAnd = NULL;
		// cond.GrowFromParseTree (final, outSchema, literal);
	}
		

}

string TableNode::toString(){

	cout<<"***************************"<<endl;
	cout<<"Table "<<tableName<<" AS "<<tableAlias<<endl;
	cout<<"Select From File Operation"<<endl;
	cout<<"Input file: "<<endl;
	cout<<"Output pipe Id: "<<outPipeID<<endl;
	cout<<"Applied CNF: "<<endl;
		cond.Print();
	cout<<"Output Schema:"<<endl;
		outSchema->Print();

}


/*
	JoinNode Methods
*/

JoinNode::JoinNode(int leftPipeID, int rightPipeID, int outPipeID){
	
	this->leftPipeID = leftPipeID;
	this->rightPipeID = rightPipeID;
	this->outPipeID = outPipeID;
	
}
void JoinNode::relatedJoinCNF(AndList *boolean, Statistics *s){



}
string JoinNode::toString(){

	cout<<"***************************"<<endl;
	cout<<"Join Operation"<<endl;
	cout<<"Left Input: "<<leftPipeID<<endl;
	cout<<"Right Input: "<<rightPipeID<<endl;
	cout<<"Out pipe: "<<outPipeID<<endl;
	cout<<"Join CNF: "<<endl;
		cond.Print();
	cout<<"Output Schema: "<<endl;
		//outSchema->Print();
}


