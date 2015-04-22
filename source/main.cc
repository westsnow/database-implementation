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
	Statistics *s = new Statistics();
	Optimizer *optimizer = new Optimizer(s);
	cout<<"Write your SQL query, to execute press enter ant then ctrl + D"<<endl;
	while(run){
		
		cout<<"sql>> ";
		yyparse();
		
		optimizer->planQuery();
		
		run = false;

	}

	return 1;
}
