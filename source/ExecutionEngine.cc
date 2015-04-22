
#include "ExecutionEngine.h"
#include "Optimizer.h"
#include "Optimizer.h"

extern char *newtable;
extern char *newfile;
extern char *oldtable;
extern char *deoutput;
extern struct NameList *attsToSelect;
extern struct FuncOperator *finalFunction; 



ExecutionEngine::ExecutionEngine(Statistics *st){
	s = st;
}

void ExecutionEngine::execute(){
	if (newtable) {
      createTable();
    } else if (oldtable && newfile) {
      insertInto();
    } else if (oldtable && !newfile) {
      dropTable();
    } else if (deoutput) {
      setOutput(deoutput);
    } else if (attsToSelect || finalFunction) {
      select();
    } 
    clear();
}

void ExecutionEngine::createTable(){
	cout<<"Create Table";
}

void ExecutionEngine::insertInto(){
	cout<<"Inser into";
}

void ExecutionEngine::dropTable(){
	cout<<"Drop Table";
}

void ExecutionEngine::setOutput(char *mode){
	cout<<" setOutput";
}

void ExecutionEngine::select(){

	Optimizer *optimizer = new Optimizer(s);
	optimizer->planQuery();
  optimizer->executeQuery();

}
 void ExecutionEngine::clear(){
 	
 	

 }

// from parser