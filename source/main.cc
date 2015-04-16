#include <iostream>
#include <string>
#include "ParseTree.h"
#include "Optimizer.h"

using namespace std;

extern "C" struct YY_BUFFER_STATE *yy_scan_string(const char*);
extern "C" {
	int yyparse(void);

}
extern struct TableList *tables;



int main()
{
	bool run = true;
	cout<<"Write your SQL query, to execute press enter ant then ctrl + D"<<endl;
	while(run){
		
		cout<<"sql>> ";
		yyparse();
		TableList *tmp = tables;
		while(tmp != NULL){
			printf("%s\n", tmp->tableName);
			printf("%s\n", tmp->aliasAs);
			tmp = tmp->next;
		}
		Optimizer *o = new Optimizer();
		
		run = false;

	}

	return 1;
}
