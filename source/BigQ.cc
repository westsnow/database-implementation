#include "BigQ.h"
#include <vector>

using namespace std;

void* consumeInnerPipe (void *arg) {
	
	Pipe *pipe = (Pipe *) arg;
	File tmpFile;
	Record tmp;
	Page page;
	tmpFile.Open(0, "./generatedRuns.tmp");

    while(pipe->Remove(&tmp)){
    	//printf("sucked a record from inner pipe\n");
		//tmp.Print(new Schema ("/Users/westsnow/GitHub/database-implementation/source/catalog", "nation"));
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
	printf("big q working...\n");
	// read data from in pipe sort them into runlen pages, STEP 1, run generation.
	int runSize = 0;
	buffsz = 100; // pipe cache size
	Pipe innerPipe (buffsz);
	pthread_t thread1;
	pthread_create (&thread1, NULL, consumeInnerPipe, (void *)&innerPipe);

	std::vector<Record*> run;
	Page page;
	Record tmp;
	int pageNum = 0;
	int totalPageNum = 0;
	while(in.Remove(&tmp)){
		if(!page.Append(&tmp)){
				Record recRetrive;
				//current page is full, put it to vector, waiting for sort.
				// printf("this page has %d records\n", page.getRecNum());
				while( page.GetFirst(&recRetrive)){
					//recRetrive.Print( new Schema ("/Users/westsnow/GitHub/database-implementation/source/catalog", "nation")  );
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
					//sort(run);//how
					for(int i = 0; i < run.size(); ++i){
						innerPipe.Insert(run[i]);
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
	//sort(run);
	if(run.size() > 0){
		runSize++;
		totalPageNum++;
	}
	for(int i = 0; i < run.size(); ++i){
		innerPipe.Insert(run[i]);
	}
	innerPipe.ShutDown();
    pthread_join (thread1, NULL);

    printf("%d runs has been generated, there are %d total pages\n", runSize, totalPageNum);



    //try to read a page from generated file
    File myfile;
    myfile.Open(1, "./generatedRuns.tmp");
    Page p;
    int pn;
    myfile.GetPage(&p, totalPageNum-1);
    Record recP;
    while( p.GetFirst(&recP)){
		recP.Print( new Schema ("/Users/westsnow/GitHub/database-implementation/source/catalog", "part")  );
	}
	printf("done...\n");

    // construct priority queue over sorted runs and dump sorted data 
 	// into the out pipe

    // finally shut down the out pipe

	out.ShutDown ();
}





BigQ::~BigQ () {
}
