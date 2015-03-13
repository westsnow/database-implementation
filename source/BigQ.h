#ifndef BIGQ_H
#define BIGQ_H
#include <pthread.h>
#include <iostream>
#include <vector>
#include "Pipe.h"
#include "File.h"
#include "Record.h"



struct thread_info {    /* Used as argument to thread_start() */
   Pipe *runBuffer;
   int runLen;
   int index;
   int pageCount;
   int id;
};

using namespace std;



class BigQ {

public:

	BigQ (Pipe &in, Pipe &out, OrderMaker &sortorder, int runlen);
	void sortVector(std::vector<Record*> &run, OrderMaker &sortorder);
	~BigQ ();

private:
	ComparisonEngine 	ceng;
	pthread_t worker;
	// void* consumeInnerPipe (void *arg);

	int buffsz; // pipe cache size

	Pipe &in, &out;
	OrderMaker sortorder;
	int runlen;

	static void *startThread(void * arg);
	void * WorkerThread(void);


};

#endif
