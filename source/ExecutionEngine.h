#ifndef EXECUTIONENGINE_H
#define EXECUTIONENGINE_H

#include "Statistics.h"


using namespace std;

class ExecutionEngine {
	private:
		Statistics *s;


	public:

		ExecutionEngine(Statistics *st);
		~ExecutionEngine();	

		void execute();
		void select();
		void createTable();
		void insertInto();
		void dropTable();
		void setOutput(char *mode);
		void clear();	
		
};



#endif