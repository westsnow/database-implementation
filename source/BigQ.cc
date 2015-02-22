#include "BigQ.h"

using namespace std;

std::vector<int> recordCount;
std::vector<int> pageCount;

void swap(std::vector<Record*> &run, int i, int j){
	if(i == j)
		return;
	Record* tmp = run[i];
	run[i] = run[j];
	run[j] = tmp;
}
// 3 2 1 5 
void BigQ::sortVector(std::vector<Record*> &run, OrderMaker &sortorder){
	if(run.size() < 2)
		return;
	// Schema* sch = new Schema ("/Users/westsnow/GitHub/database-implementation/source/catalog", "nation");
	for(int i = 0; i < run.size(); ++i){
		for(int j = 0; j < run.size() - i-1; ++j){
			// printf("now comparing.........................................\n");
			// run[j]->Print(sch);
			// run[j+1]->Print(sch);
			// printf("the result is %d\n", ceng.Compare(run[j], run[j+1], &sortorder));
			if(ceng.Compare(run[j], run[j+1], &sortorder) == 1){
				swap(run, j, j+1);
			}
		}
	}
}



void* producerRunPipe(void *arg){
	thread_info *ti = (thread_info *) arg;

	File tmpFile;
	tmpFile.Open(1, "./generatedRuns.tmp");

	int currPage = 0;

	//number pages in file, used in case I hit the last page
	int fileSize = tmpFile.GetLength();
	Page p = Page();
	//While there are pages left to produce on this run
	while(  (ti->index*ti->runLen)+currPage <  (ti->index+1)*ti->runLen &&
			(ti->index*ti->runLen)+currPage <  fileSize -1
		){
		//Get current page
		tmpFile.GetPage(&p, (ti->index*ti->runLen)+currPage );
		Record *r = new Record();
		while(p.GetFirst(r)){
			//printf("run %d inserting into pipe \n", ti->index);

			ti->runBuffer->Insert(r);
		}

		currPage++;
	}
	tmpFile.Close();

	//printf("shutting down buffer");
	ti->runBuffer->ShutDown();
}

void initializeHeap(std::vector<Pipe*> *runBuffers, std::vector<Record*> *heap,int numRuns, int runLength){
	printf("initializing \n");

	for (int i=0;i<numRuns;i++){
		int buffsz = 16;
		//printf("Initializing pipe %d, with buffer_size %d \n", i,buffsz);
		Pipe *runPipe = new Pipe(buffsz);

		// printf("Initializing Producer Thread \n");
		pthread_t runThread;

		thread_info *ti = new thread_info();
		ti->runBuffer = runPipe;
		ti->runLen = runLength;
		ti->index = i;

		pthread_create (&runThread, NULL, producerRunPipe, (void *)ti);

		// printf("Storing pipe pointer \n");

		runBuffers->push_back( runPipe) ;

		//Check here if segfault
		Record *r = new Record;
		runPipe->Remove(r);
		heap->push_back(r);

	}

}



void* consumeInnerPipe (void *arg) {
	
	Pipe *pipe = (Pipe *) arg;
	File tmpFile;
	Record tmp;
	Page page;
	tmpFile.Open(0, "./generatedRuns.tmp");

	//record how many pages each run has.
 	pageCount.clear();
 	pageCount.resize(recordCount.size());
 	//record how many records have been added to the current run
 	int recordNum = 0;
  	//record how many pages have been added to the current run
 	int pageNum = 1;

 	int runNum = 0;


    while(pipe->Remove(&tmp)){
  //   	printf("sucked a record from inner pipe\n");
		// tmp.Print(new Schema ("/Users/westsnow/GitHub/database-implementation/source/catalog", "nation"));
		if(!page.Append(&tmp)){
			//tmp.Print(new Schema ("/Users/westsnow/GitHub/database-implementation/source/catalog", "nation"));
			pageNum++;
			tmpFile.AddPageToEnd(&page);
			page.EmptyItOut();
			page.Append(&tmp);
			recordNum++;
		}else{
			recordNum++;
		}
		if(recordNum == recordCount[runNum]){
			if( !page.isEmpty() ){
				//pageNum++;
				tmpFile.AddPageToEnd(&page);
				page.EmptyItOut();
			}
			pageCount[runNum++] = pageNum;
			pageNum = 1;
		}
	}
	if( !page.isEmpty())
		tmpFile.AddPageToEnd(&page);
	//printf("Pages in file: %d", tmpFile.GetLength());
	tmpFile.Close();
}



BigQ :: BigQ (Pipe &in, Pipe &out, OrderMaker &sortorder, int runlen) {
	printf("big q working...\n");
	// read data from in pipe sort them into runlen pages, STEP 1, run generation.
	int runSize = 0;
	buffsz = 100; // pipe cache size
	Pipe innerPipe (buffsz);
	pthread_t thread1;
	pthread_create (&thread1, NULL, consumeInnerPipe, (void *)&innerPipe);

	std::vector<Record*> run;
	recordCount.clear();
	Page page;
	Record tmp;
	int pageNum = 0;
	int totalPageNum = 0;
// Schema* sch = new Schema ("/Users/westsnow/GitHub/database-implementation/source/catalog", "nation");
	while(in.Remove(&tmp)){
		if(!page.Append(&tmp)){
				// tmp.Print(sch);
				Record recRetrive;
				//current page is full, put it to vector, waiting for sort.
				// printf("this page has %d records\n", page.getRecNum());
				while( page.GetFirst(&recRetrive)){
					// recRetrive.Print( new Schema ("/Users/westsnow/GitHub/database-implementation/source/catalog", "nation")  );
					// printf("get one rec\n");
					// printf("run's length is %d\n", run.size());
					Record *r = new Record();
					r->Copy(&recRetrive);
					run.push_back(r);
				}
				page.EmptyItOut();
				page.Append(&tmp);
				pageNum++;
				totalPageNum++;
				//printf("page num is now %d\n", pageNum);
				if(pageNum >= runlen){
					pageNum = 0;
					sortVector(run, sortorder);
					recordCount.push_back((int)run.size());
					printf("vector's size is %d now\n", run.size());
					for(int i = 0; i < run.size(); ++i){
						// run[i]->Print(sch);
						innerPipe.Insert(run[i]);
						// vec[1] = run.size();
					}
					runSize++;
					run.clear();
				}
		}else{
			//printf("Appended\n");
		}	
	}
	//retrive records from last page;
	Record recRetrive;
	while( page.GetFirst(&recRetrive)){
		Record *r = new Record();
		r->Copy(&recRetrive);
		run.push_back(r);
	}
	sortVector(run, sortorder);
	if(run.size() > 0){
		recordCount.push_back((int)run.size());
		runSize++;
		totalPageNum++;
	}
	for(int i = 0; i < run.size(); ++i){
		innerPipe.Insert(run[i]);
	}
	innerPipe.ShutDown();
    pthread_join (thread1, NULL);

    printf("%d runs has been generated, %d runs?... there are %d total pages\n", runSize, recordCount.size(), totalPageNum);


    /*
    //try to read a page from generated file
    File myfile;
    myfile.Open(1, "./generatedRuns.tmp");
    Page p;
    int pn;
    myfile.GetPage(&p, totalPageNum-1);
    Record recP;
    while( p.GetFirst(&recP)){
		recP.Print( new Schema ("/Users/Migue/Documents/workspace/database-implementation/source/catalog", "part")  );
	}*/


	printf("done... phase one there are %d runs\n", runSize);



	/*
		PHASE TWO -  MERGE RUNS
	*/

	printf("start... phase two\n");
	std::vector<Record*>	heap;
	std::vector<Pipe*> 		runBuffers;


	//Initialize one pipe per run, and creates a producer for each pipe
	//Each producer will extract rows from its run in the file and feed the pipe
	//Heap is initialized with first row of each run (consume from its runBuffer[i])
	initializeHeap(&runBuffers, &heap, runSize, runlen);


	Record 				*min;
	int 				minIndex;

	int 				cont = 0;
	while(heap.size()>0){ //While there is something to extract

		min = heap[0];
		minIndex = 0;
		for(int i=1; i<heap.size(); i++){
			//printf("comp %d \n",ceng.Compare(min, heap[i], &sortorder));
			if(ceng.Compare(min, heap[i], &sortorder) == 1){
				min = heap[i];
				minIndex = i;
			}
		}

		cont++;
		out.Insert(min);
		//printf("output min %d \n", cont);

		if(runBuffers[minIndex]->Remove( heap[minIndex] ) == 0){
			printf("Consumed Run \n");
			runBuffers.erase(runBuffers.begin()+minIndex);
			heap.erase(heap.begin()+minIndex);

		}






	}


	printf("phase two finished \n");

	out.ShutDown ();

}





BigQ::~BigQ () {
}
