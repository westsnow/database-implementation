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

using namespace std;

Optimizer::Optimizer(Statistics *st){
	s = st;
	s->init();
}

void Optimizer::planQuery(){

	createTableNodes();

}

int evalOrder(vector<TableNode*> operands, Statistics *s, int minCost){
	return 0;

}

void Optimizer::createTableNodes(){
	
	int pipeid = 1;
	TableList *tmp = tables;
	while(tmp != NULL){

		string name(tmp->tableName);
		string alias(tmp->aliasAs);
		//s->CopyRel(tmp->tableName, tmp->aliasAs);
		TableNode *t = new TableNode(name, alias, pipeid);
		t->relatedSelectCNF(boolean, tmp->tableName, tmp->aliasAs, s);
		t->toString();
		
		
		tableNodes.push_back(t);
		
		tmp = tmp->next;
		pipeid += 1;
	} 
	std::vector<TableNode*> tableList(tableNodes);
	sort (tableList.begin(),tableList.end());
	int minCost = INT_MAX;
	int cost;
	do {
		cost=evalOrder(tableList, s, minCost);
		
		// for(int i=0; i<tableList.size(); i++){
		// 	cout<<tableList[i]->tableName<<"-";
		// }
		cout<<endl;
		if(cost<minCost && cost>0) {
			minCost = cost; 
			tableNodes = tableList; 
		}
	} while ( next_permutation(tableList.begin(), tableList.end()) );



}



/*
	TableNode Methods
*/

//TableNode::TableNode(CNF cond, string tableName, string tableAlias, outPipeID, string fileName){
TableNode::TableNode(string name, string alias, int outPipeID){
	
	tableName = name;
	tableAlias =  alias;
	this->outPipeID ;
	fileName = "";
}

void TableNode::relatedSelectCNF(AndList *boolean, char *c_name, char *c_alias, Statistics *s){
	string name(c_name);
	string alias(c_alias);
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

	outSchema = new Schema(catalog_path, c_name, c_alias);
	
	if(andFinal != NULL){
		
		//Record literal;
		//need to check this!!!
		cond.GrowFromParseTree (andFinal, outSchema, literal);

		

	}else{
		
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

	cout<<"\t";


}







