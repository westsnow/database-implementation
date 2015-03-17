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

