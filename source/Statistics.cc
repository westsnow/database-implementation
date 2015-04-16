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

// hash_map<string, int>::iterator i = dict.find("apple");

bool RelStat::attrExists(string attName){
	// printf("\n\nprint the relation\n\n");
	// for(hash_map<char*, int>::iterator iter = attInfo.begin(); iter !=  attInfo.end(); ++iter){
	// 	if( strcmp(attName, iter->first) == 0){
	// 		printf("!!!!!!!!!!find!!!!!!!!\n");
	// 	}
	// 	printf("%s to %d\n", iter->first, iter->second);
	// }
	// char* name = "l_returnflag";
	// char name[30];
	// strcpy(name, "l_returnflag");

	hash_map<string, int>::iterator ite = attInfo.find(attName);
	if( ite == attInfo.end()){
		// cout<<"no luck to find "<<attName<<endl;
		return false;
	}
	return true;
}

int RelStat::getValue(string attName){
	hash_map<string, int>::iterator i = attInfo.find(attName);
	if( i == attInfo.end())
		return -1;
	return i->second;
}


bool Statistics::RelExists(string relName){
	hash_map<string, RelStat*>::iterator i = relInfo.find(relName);
	if( i == relInfo.end() )
		return false;
	return true;
}

void Statistics::AddRel(char *relName, int numTuples)
{
	string s_relName(relName);
	if( !RelExists(s_relName) ){
		RelStat* tmp = new RelStat;
		tmp->numTuples = numTuples;
		relInfo[s_relName] = tmp;
	}else{
		relInfo[s_relName]->numTuples = numTuples;
	}
}
void Statistics::AddAtt(char *relName, char *attName, int numDistincts)
{
	string s_relName(relName);
	string s_attName(attName);
	if(RelExists(s_relName))
		relInfo[s_relName]->attInfo[s_attName] = numDistincts;
}
void Statistics::CopyRel(char *oldName, char *newName)
{
	string s_oldName(oldName);
	string s_newName(newName);
	if(RelExists(s_oldName)){
		hash_map<string, int> newAttInfo;
		RelStat *newRel = new RelStat( *(relInfo[s_oldName]) );
		for (auto it = newRel->attInfo.begin(); it != newRel->attInfo.end(); ++it){
			string newAttName = s_newName + "." +it->first;
			//cout<<newAttName<<endl;
			//cout<<it->first
			newAttInfo[newAttName] = it->second;
		}
		newRel->attInfo = newAttInfo;

		relInfo[s_newName] = newRel;
	}
}
//	void Statistics::Read(char *fromWhere){}
//	void Statistics::Write(char *fromWhere){}

 void Statistics::Read(char *fromWhere){

 	ifstream stat_file;
 	string line;
 	cout<<"before reading file"<<endl;
 	stat_file.open(fromWhere);
 	cout<<"have read file"<<endl;
 	string relName;

 	while(getline(stat_file, line)){

 		if(line.substr(0,1).compare("-")!= 0){
 			int idx = line.find(" ");
 			relName = "";
 			relName = line.substr(1,idx-1);
 			int numTuples = atoi(line.substr(idx+1, line.length()-idx-1).c_str());
 			RelStat* tmp = new RelStat;
 			tmp->numTuples = numTuples;
 			relInfo[relName] = tmp;
 		}
 		else{
 			int idx = line.find(" ");
 			string attName = "";
 			attName = line.substr(0,idx);
 			int numDistincts = atoi(line.substr(idx+1, line.length()-idx-1).c_str());
 			relInfo[relName]->attInfo[attName] = numDistincts;
 		}
 	}

 }
 void Statistics::Write(char *fromWhere){

 	ofstream stat_file;

 	// write
 	stat_file.open(fromWhere);

 	for (hash_map<string, RelStat*>::iterator r = relInfo.begin();
 			r != relInfo.end();
 			++r) // for each item in the hash map:
 	{
 	    stat_file<<r->first<<" "<<r->second->numTuples<<endl;
 	    for (hash_map<string, int>::iterator att = r->second->attInfo.begin();
 	    			att != r->second->attInfo.end();
 	    			++att) // for each item in the hash map:
 	    	{
 	    		stat_file<<"-"<<att->first<<" "<<att->second<<endl;
 	    	}
 	}


 	stat_file.close();

 }
string Statistics::getTableNameFromAttr(string attrName){
	//printf("%s to find\n", attrName.c_str());
	for( hash_map<string, RelStat*>::iterator iter = relInfo.begin(); iter != relInfo.end(); ++iter ){
		//cout<<iter->first<<endl;
		if(iter->second->attrExists(attrName))
			return iter->first;
	}
	return "";
}
double getOrListFraction(vector<double> orFraction){
	double result = 1.0;
	for(int i = 0; i < orFraction.size(); ++i){
		result *= (1 - orFraction[i]);
	}
	return 1 - result;
}
double getAndListFraction(vector<double> andFraction){
	double result = 1.0;
	for(int i = 0; i < andFraction.size(); ++i){
		result *= andFraction[i];
	}
	return result;
}




void  Statistics::Apply(struct AndList *parseTree, char *relNames[], int numToJoin){

	struct AndList* andList = parseTree;
		struct OrList* orList = NULL;
		double result = 0.0;
		double fraction = 1.0;

		bool hasJoin = false;

		std::vector<double> andFraction;
		string joinedTableName = "";

		while(andList != NULL){
			// double andFraction = 1.0;
			orList = andList->left;
			std::vector<double> orFraction;
			bool hasJoinInner = false;
			while(orList != NULL){
				struct ComparisonOp *pCom = orList->left;
				struct Operand* leftOperand = pCom->left;
				struct Operand* rightOperand = pCom->right;
				string leftValue(leftOperand->value);
				string rightValue(rightOperand->value);
				string tableNameOfLeft = getTableNameFromAttr(leftOperand->value);

				string tableNameOfRight = getTableNameFromAttr(rightOperand->value);

				if(tableNameOfLeft == "" && tableNameOfRight == ""){
					printf("fatal error, attribute (%s,%s) not found in any table\n", leftOperand->value, rightOperand->value);
					exit(1);
				}
				//estimate join
				if(leftOperand->code == 4 && rightOperand->code == 4){
					hasJoin = true;
					hasJoinInner = true;
					int mul1 = relInfo[tableNameOfLeft]->numTuples;
					int mul2 = relInfo[tableNameOfRight]->numTuples;
					int dis1 = relInfo[tableNameOfLeft]->attInfo[leftValue];
					int dis2 = relInfo[tableNameOfRight]->attInfo[rightValue];
					//in case causing integer overflow
					double res = ((double)mul1/max(dis1,dis2)) * mul2;
					andFraction.push_back(res);
					//andList = andList->rightAnd;

					hash_map<string, int> joinedAttInfo;

					for (auto it = relInfo[tableNameOfLeft]->attInfo.begin(); it != relInfo[tableNameOfLeft]->attInfo.end(); ++it){
						joinedAttInfo[it->first] = it->second;
					}

					for (auto it = relInfo[tableNameOfRight]->attInfo.begin(); it != relInfo[tableNameOfRight]->attInfo.end(); ++it){
						joinedAttInfo[it->first] = it->second;
					}

					RelStat *joinedStat = new RelStat();

					joinedStat->attInfo = joinedAttInfo;
					joinedTableName = tableNameOfLeft+"|"+tableNameOfRight;
					relInfo[joinedTableName] = joinedStat;
					relInfo.erase(tableNameOfRight);
					relInfo.erase(tableNameOfLeft);

					break;
				}else{
					double f;
					if(leftOperand->code == 4)
						f = 1.0/relInfo[tableNameOfLeft]->getValue(leftValue) ;
					else
						f = 1.0/relInfo[tableNameOfRight]->getValue(rightValue);
					if(pCom->code != 3)
						f = (1.0/3);
					
					orFraction.push_back(f);
				}

				orList = orList->rightOr;
			}
			if(!hasJoinInner){
				andFraction.push_back(getOrListFraction(orFraction));
			}
			//get next andList

			andList = andList->rightAnd;
		}
		if(!hasJoin){

			string tableName(relNames[0]);
			andFraction.push_back(relInfo[tableName]->numTuples);
			getAndListFraction(andFraction);

		}
		if(joinedTableName != "")
			relInfo[joinedTableName]->numTuples = getAndListFraction(andFraction);



}


int max(int int1, int int2){
	return int1>int2?int1:int2;
}
double Statistics::Estimate(struct AndList *parseTree, char **relNames, int numToJoin)
{
	struct AndList* andList = parseTree;
		struct OrList* orList = NULL;
		double result = 0.0;
		double fraction = 1.0;

		bool hasJoin = false;

		std::vector<double> andFraction;
		while(andList != NULL){
			// double andFraction = 1.0;
			orList = andList->left;
			std::vector<double> orFraction;
			bool hasJoinInner = false;
			string preLeftValue = "";
			while(orList != NULL){
				struct ComparisonOp *pCom = orList->left;
				struct Operand* leftOperand = pCom->left;
				struct Operand* rightOperand = pCom->right;
				string leftValue(leftOperand->value);
				string rightValue(rightOperand->value);
				string tableNameOfLeft = getTableNameFromAttr(leftOperand->value);

				string tableNameOfRight = getTableNameFromAttr(rightOperand->value);

				if(tableNameOfLeft == "" && tableNameOfRight == ""){
					printf("fatal error, attribute (%s,%s) not found in any table\n", leftOperand->value, rightOperand->value);
					exit(1);
				}
				//estimate join
				if(leftOperand->code == 4 && rightOperand->code == 4){
					hasJoin = true;
					hasJoinInner = true;

					int mul1 = relInfo[tableNameOfLeft]->numTuples;

					int mul2 = relInfo[tableNameOfRight]->numTuples;

					int dis1 = relInfo[tableNameOfLeft]->attInfo[leftValue];

					int dis2 = relInfo[tableNameOfRight]->attInfo[rightValue];

					//in case causing integer overflow
					double res = ((double)mul1/max(dis1,dis2)) * mul2;
					andFraction.push_back(res);
					//andList = andList->rightAnd;
					break;
				}else{
					double f;
					if(leftOperand->code == 4)
						f = 1.0/relInfo[tableNameOfLeft]->getValue(leftValue) ;
					else
						f = 1.0/relInfo[tableNameOfRight]->getValue(rightValue);
					if(pCom->code != 3)
						f = (1.0/3);
					if(preLeftValue == leftValue){
						orFraction[orFraction.size()-1] += f;
					}else
						orFraction.push_back(f);
					preLeftValue = leftValue;
				}
				//get next orList
				//cout<<"next or"<<endl;
				orList = orList->rightOr;
			}
			if(!hasJoinInner){

				andFraction.push_back(getOrListFraction(orFraction));
			}
			//get next andList

			andList = andList->rightAnd;
		}
		if(!hasJoin){
			string tableName(relNames[0]);
			andFraction.push_back(relInfo[tableName]->numTuples);
		}

		return getAndListFraction(andFraction);

}

void Statistics::init(){

	char *relName[] = {"nation", "region", "part", "customer", "lineitem", "orders", "supplier","partsupp"};
	
	//nation
	AddRel(relName[0],25);	
	AddAtt(relName[0], "n_nationkey",25);
	AddAtt(relName[0], "n_regionkey",5);
	AddAtt(relName[0], "n_name",25);
	
	
	//region
	AddRel(relName[1],5);
	AddAtt(relName[1], "r_regionkey",5);
	AddAtt(relName[1], "r_name",5);
	
	//part
	AddRel(relName[2],200000);
	AddAtt(relName[2], "p_partkey",200000);
	AddAtt(relName[2], "p_size",50);
	AddAtt(relName[2], "p_name", 199996);
	AddAtt(relName[2], "p_container",40);
	
	//customer
	AddRel(relName[3],150000);
	AddAtt(relName[3], "c_custkey",150000);
	AddAtt(relName[3], "c_nationkey",25);
	AddAtt(relName[3], "c_mktsegment",5);
	AddAtt(relName[3], "c_name",333);
	
	//lineitem
	AddRel(relName[4],6001215);
	AddAtt(relName[4], "l_returnflag",3);
	AddAtt(relName[4], "l_discount",11);
	AddAtt(relName[4], "l_shipmode",7);
	AddAtt(relName[4], "l_orderkey",1500000);
	AddAtt(relName[4], "l_receiptdate",1500000);
	AddAtt(relName[4], "l_partkey",200000);
	AddAtt(relName[4], "l_shipinstruct",4);
	AddAtt(relName[4], "l_quantity",7655);
	
	//orders
	AddRel(relName[5],1500000);
	AddAtt(relName[5], "o_orderkey",1500000);
	AddAtt(relName[5], "o_custkey",150000);
	AddAtt(relName[5], "o_orderdate",150000);
	
	//supplier
	AddRel(relName[6],10000);
	AddAtt(relName[6], "s_suppkey",10000);
	AddAtt(relName[6], "s_nationkey",25);
	AddAtt(relName[6], "s_acctbal",765);
	
	//partsupp
	AddRel(relName[7],800000);
	AddAtt(relName[7], "ps_suppkey", 10000);
	AddAtt(relName[7], "ps_partkey", 200000);

}






