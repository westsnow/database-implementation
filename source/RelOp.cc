#include "RelOp.h"


void* SelectFileWorkerThread(void *arg){
	SelectFileStruct *sf = (SelectFileStruct *) arg;
	Record r;
	ComparisonEngine comp;
	printf("start");
	sf->inFile->MoveFirst();
	printf("move first");
	while(sf->inFile->GetNext(r)){
		if (comp.Compare(&r, sf->literal, sf->selOp)){
			//r.Print(new Schema ("/Users/Migue/Development/workspace_cpp/database-implementation/source/catalog", "partsupp")) ;

			sf->outPipe->Insert(&r);
		}
	}
	sf->outPipe->ShutDown();

}

void RelationalOp::WaitUntilDone () {
	pthread_join (worker_thread, NULL);
}

void SelectFile::Run (DBFile &inFile, Pipe &outPipe, CNF &selOp, Record &literal){

	SelectFileStruct *sfs = new SelectFileStruct();

	sfs->inFile = &inFile;
	sfs->outPipe = &outPipe;
	sfs->selOp = &selOp;
	sfs->literal = &literal;
	pthread_create (&worker_thread, NULL, SelectFileWorkerThread, (void *) sfs);

}


void SelectFile::Use_n_Pages (int runlen) {

}



void* SelectPipeWorkerThread(void *arg){
	SelectPipeStruct *sp = (SelectPipeStruct *) arg;
	Record r;
	ComparisonEngine comp;
	while(sp->inPipe->Remove(&r)){
		if (comp.Compare(&r, sp->literal, sp->selOp)){
					sp->outPipe->Insert(&r);
		}
	}
	sp->outPipe->ShutDown();
}


void SelectPipe::Run (Pipe &inPipe, Pipe &outPipe, CNF &selOp, Record &literal){
	SelectPipeStruct *sps = new SelectPipeStruct();

	sps->inPipe = &inPipe;
	sps->outPipe = &outPipe;
	sps->selOp = &selOp;
	sps->literal = &literal;
	pthread_create (&worker_thread, NULL, SelectPipeWorkerThread, (void *)sps);
}

void SelectPipe::Use_n_Pages (int runlen) {

}

void* DuplicateRemovalThread(void *arg){

	DuplicateRemovalStruct *drs = (DuplicateRemovalStruct *) arg;

	ComparisonEngine comp;
	OrderMaker *order = new OrderMaker(drs->schema);
	Pipe tmp(100);


	BigQ bigQ( *(drs->inPipe), tmp, *(order), 10);

	Record *record = new Record();
	Record *previous = new Record();
	previous = NULL;

	while(tmp.Remove(record)){
		count1++;
		// record->Print(drs->schema);
		// comp.Compare(previous, record,  drs->orderMaker) != 0 
		if(previous == NULL ){
			previous = new Record();
			previous->Copy(record);
		}
		if( comp.Compare(previous, record,  order) != 0 ){
			drs->outPipe->Insert(previous);
			count2++;
			previous->Copy(record);
		}
	}

	if(!previous->isNULL())
		drs->outPipe->Insert(previous);
	drs->outPipe->ShutDown();
}


void DuplicateRemoval::Run (Pipe &inPipe, Pipe &outPipe, Schema &mySchema){
	DuplicateRemovalStruct *drs = new DuplicateRemovalStruct();

	drs->inPipe = &inPipe;
	drs->outPipe = &outPipe;
	drs->schema = &mySchema;

	pthread_create (&worker_thread, NULL, DuplicateRemovalThread, (void *)drs);
}


void* WriteOutWorkerThread(void *arg){
	WriteOutStruct *wos = (WriteOutStruct *) arg;
	Schema *schema = wos->schema;
	Record lTempRecord;
	while( wos->inPipe->Remove(&lTempRecord)){
		int lNumOfAttr = schema->GetNumAtts();
		Attribute *lAttr = schema->GetAtts();
		for (int i = 0; i < lNumOfAttr; i++) {
			fprintf(wos->file, "%s%s",lAttr[i].name,":[");
			int lRecordValue = ((int *) lTempRecord.bits)[i + 1];
			if (lAttr[i].myType == Int) {
	  			int *mInt = (int *) &(lTempRecord.bits[lRecordValue]);
	  			fprintf(wos->file, "%d", *mInt);
			}
			else if (lAttr[i].myType == Double) {
	  			double *mDouble = (double *) &(lTempRecord.bits[lRecordValue]);
	  			fprintf(wos->file, "%f", *mDouble);
			}
			else if (lAttr[i].myType == String) {
	   			char *mString = (char *) &(lTempRecord.bits[lRecordValue]);
	   			fprintf(wos->file, "%s", *mString);
			}
			fprintf(wos->file, "%s", "]");
			if (i != lNumOfAttr - 1) {
	    		fprintf(wos->file, "%s", ",");
	    	}
		}
		fprintf(wos->file, "\n");
	}
	fclose(wos->file);
}

void WriteOut::Run (Pipe &inPipe, FILE *outFile, Schema &mySchema){
	WriteOutStruct *wos = new WriteOutStruct();

	wos->inPipe = &inPipe;
	wos->file = outFile;
	wos->schema = &mySchema;

	pthread_create (&worker_thread, NULL, WriteOutWorkerThread, (void *)wos);
}


/*
	
	PROJECT METHODS

*/

void* ProjectWorkerThread(void *arg){
	
	ProjectStruct *ps = (ProjectStruct *) arg;
	Record r;
	ComparisonEngine comp;
	printf("Project worked started \n");	
	while(ps->inPipe->Remove(&r)){
		
		r.Project(ps->keepMe, ps->numAttsOutput, ps->numAttsInput);
		ps->outPipe->Insert(&r);
	}
	ps->outPipe->ShutDown();
}


void Project::Run (Pipe &inPipe, Pipe &outPipe, int *keepMe, int numAttsInput, int numAttsOutput){
	
	ProjectStruct *ps = new ProjectStruct();
	ps->inPipe = &inPipe;
	ps->outPipe = &outPipe;
	ps->keepMe = keepMe;
	ps->numAttsInput = numAttsInput;
	ps->numAttsOutput = numAttsOutput;

	pthread_create (&worker_thread, NULL, ProjectWorkerThread, (void *)ps);
}

void Project::Use_n_Pages (int runlen) {

}

