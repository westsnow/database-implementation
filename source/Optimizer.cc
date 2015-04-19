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

char* catalog_path = "/Users/Migue/DBIDATA/catalog";

using namespace std;

Optimizer::Optimizer(Statistics *st){
	s = st;
	s->init();
}

void Optimizer::planQuery(){

	createTableNodes();

}

CNF* Optimizer::relatedSelectCNF(AndList *boolean, char *c_name, char *c_alias){
	string name(c_name);
	string alias(c_alias);
	AndList *andTmp = boolean;
	AndList *andFinal = NULL;
	CNF result;

	while(andTmp != NULL){

		OrList *orList = andTmp->left;

		while(orList != NULL){
			struct ComparisonOp *pCom = orList->left;
			struct Operand* leftOperand = pCom->left;
			printf("%d\n", leftOperand->code);
			struct Operand* rightOperand = pCom->right;
			printf("%d\n", rightOperand->code);
			string leftValue(leftOperand->value);
			string rightValue(rightOperand->value);
			string tableNameOfLeft = s->getTableNameFromAttr(leftOperand->value);
			string tableNameOfRight = s->getTableNameFromAttr(rightOperand->value);

			if((leftOperand->code == 3 && rightOperand->code != 3) || (leftOperand->code != 3 && rightOperand->code == 3)){
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
	if(andFinal != NULL){
		
		Record literal;
		//need to check this!!!
		result.GrowFromParseTree (andFinal, new Schema(catalog_path, c_name, c_alias), literal);

		result.Print();

	}else{
		cout<<"Nothing";
	}
		

}

void Optimizer::createTableNodes(){
	
	int pipeid = 1;
	TableList *tmp = tables;
	while(tmp != NULL){

		string name(tmp->tableName);
		string alias(tmp->aliasAs);
		s->CopyRel(tmp->tableName, tmp->aliasAs);
		CNF *related = relatedSelectCNF(boolean, tmp->tableName, tmp->aliasAs);
		TableNode *t = new TableNode(name, alias, pipeid);
		
		tableNodes.push_back(*(t));
		
		tmp = tmp->next;
		pipeid += 1;
	} 



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

string TableNode::toString(){

}







