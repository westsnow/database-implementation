#include "BigQ.h"
#include <vector>

void* consumeInnerPipe (void *arg) {
	
	Pipe *pipe = (Pipe *) arg;
	File tmpFile;
	Record tmp;
	Page page;
	tmpFile.Open(0, "generatedRuns.tmp");

	while(pipe->Remove(&tmp)){
		if(!page.Append(&tmp)){
			tmpFile.AddPageToEnd(&page);
			page.EmptyItOut();
			page.Append(&tmp);
		}
	}
	if( !page.isEmpty())
		tmpFile.AddPageToEnd(&page);
	tmpFile.Close();
}

BigQ :: BigQ (Pipe &in, Pipe &out, OrderMaker &sortorder, int runlen) {
	// read data from in pipe sort them into runlen pages, STEP 1, run generation.
	int runSize = 0;
	buffsz = 100; // pipe cache size
	Pipe innerPipe (buffsz);
	pthread_t thread1;
	pthread_create (&thread1, NULL, consumeInnerPipe, (void *)&innerPipe);

	std::vector<Record> run;
	Page page;
	Record tmp;
	int pageNum = 0;
	while(in.Remove(&tmp)){
		if(!page.Append(&tmp)){
				Record recRetrive;
				//current page is full, put it to vector, waiting for sort.
				while( page.GetFirst(&recRetrive)){
					run.push_back(recRetrive);
				}
				page.EmptyItOut();
				page.Append(&tmp);
				pageNum++;
				if(pageNum >= runlen){
					pageNum = 0;
					//sort(run);//how
					for(int i = 0; i < run.size(); ++i){
						innerPipe.Insert(&run[i]);
					}
					runSize++;
					run.clear();
				}
		}	
	}
	//sort(run);
	for(int i = 0; i < run.size(); ++i){
		innerPipe.Insert(&run[i]);
	}
	runSize++;
	innerPipe.ShutDown();
    pthread_join (thread1, NULL);

    // construct priority queue over sorted runs and dump sorted data 
 	// into the out pipe

    // finally shut down the out pipe
	out.ShutDown ();
}

//void *consumer (void *arg) {






BigQ::~BigQ () {
}
