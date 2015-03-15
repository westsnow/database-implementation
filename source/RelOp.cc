#include "RelOp.h"


void* SelectFileWorkerThread(void *arg){
	SelectFile sf = (SelectFile *) arg;
	Record r;
	ComparisonEngine comp;
	sf.inFile.MoveFirst();

	while(sf.inFile.GetNext(r)){
		if (comp.Compare(&r, &sf.literal, &sf.selOp)){
					sf.outPipe.Insert(&r);
		}
	}
	sf.outPipe.ShutDown();
}

void SelectFile::Run (DBFile &_inFile, Pipe &_outPipe, CNF &_selOp, Record &_literal):
		inFile(_inFile), outPipe(_outPipe), selOp(_selOp), literal(_literal){

	pthread_create (&worker_thread, NULL, SelectFileWorkerThread, this);

}


void SelectFile::WaitUntilDone () {
	pthread_join (worker_thread, NULL);
}

void SelectFile::Use_n_Pages (int runlen) {

}


void* SelectPipeWorkerThread(void *arg){
	SelectPipe sp = (SelectPipe *) arg;
	Record r;
	ComparisonEngine comp;
	while(sp.inPipe.Remove(&r)){
		if (comp.Compare(&r, &sp.literal, &sp.selOp)){
					sp.outPipe.Insert(&r);
		}
	}
	sp.outPipe.ShutDown();
}


void SelectPipe::Run (Pipe &_inPipe, Pipe &_outPipe, CNF &_selOp, Record &_literal):
	inPipe(_inPipe), outPipe(_outPipe), selOp(_selOp), literal(_literal){

	pthread_create (&worker_thread, NULL, SelectPipeWorkerThread, this);
}

void SelectPipe::WaitUntilDone () {
	pthread_join (worker_thread, NULL);
}

void SelectPipe::Use_n_Pages (int runlen) {

}

