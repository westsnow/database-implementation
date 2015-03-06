#include "DBFile.h"
#include <fstream>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
// stub file .. replace it with your own DBFile.cc


GeneralDBFile::GeneralDBFile(){

}

GeneralDBFile::~GeneralDBFile(){

}
//Heap
Heap::Heap(){
	opened_file = new File();
	curr_page = new Page();
	page_number = 0;
}

Heap::~Heap(){

}


int Heap::Create (char *f_path, fType f_type, void *startup) {

	File file;

	ofstream header_file;

	// construct path of the header meta-data file
	char header_path[100];
	sprintf (header_path, "%s.header", f_path);
	// write meta-data file depending on type of file
	header_file.open(header_path);
	if(!header_file.is_open())
		return 0;
	header_file <<"heap"<<endl;
	header_file.close();


	file.Open(0,f_path);
	file.Close();
	opened_file->Open(1, f_path);
	return 1;
	
}


int Heap::Load (Schema &f_schema, char *loadpath) {

	if( !opened_file->isOpen())
		return 0;


	Page page_buffer = Page();


	FILE *tableFile = fopen (loadpath, "r");
	Record tmp;
	//open file to write records

   	while(tmp.SuckNextRecord(&f_schema, tableFile) ){
		//tmp.Print(&lineitem);
		if(page_buffer.Append(&tmp)){
		}
		else{
			opened_file->AddPageToEnd(&page_buffer);
			page_buffer.EmptyItOut();
			page_buffer.Append(&tmp);
		}
	}
	opened_file -> AddPageToEnd(&page_buffer);
	page_buffer.EmptyItOut();
	// cout<<"file has "<<opened_file->GetLength()<<" pages"<<endl;
	return 1;

}

int Heap::Open (char *f_path) {
	opened_file->Open(1, f_path);
	//cout<<"there are "<<opened_file->GetLength()<<" pages in the file"<<endl;
	return 1;
}

void Heap::MoveFirst () {
	page_number = 0;
	if(curr_page->numRecs > 0) curr_page->EmptyItOut();
	opened_file->GetPage(curr_page, page_number);
}

int Heap::Close () {
	//cout<<"Closing DBFile";
	opened_file->Close();
	return 1;
}

int Heap::Add (Record &rec) {
	//rec.Print( new Schema ("/Users/Migue/Documents/workspace/database-implementation/source/catalog", "part")  );
	Page oPage = Page();

	opened_file->GetPage(&oPage, opened_file->GetLength() - 2);

	//if the last page is full
	if( !oPage.Append(&rec) ){
		Page newPage = Page();
		newPage.Append(&rec);
		opened_file->AddPageToEnd(&newPage);
	}else{
		opened_file->AddPage(&oPage, opened_file->GetLength() - 2);
	}
	return 1;
}

int Heap::GetNext (Record &fetchme) {
	if(curr_page->numRecs > 0) {
		curr_page->GetFirst(&fetchme);
	}
	else{
		page_number++;
		if(opened_file->GetLength()> (page_number + 1) ){
			opened_file->GetPage(curr_page, page_number);
			curr_page->GetFirst(&fetchme);
		}
		else return 0;
	}
	return 1;
}

int Heap::GetNext (Record &fetchme, CNF &cnf, Record &literal) {
	//??? consume
	ComparisonEngine comp;
	while (GetNext(fetchme)){
		if (comp.Compare(&fetchme, &literal, &cnf)){
			return 1;
		}
	}

	return 0;
}

//Sorted
Sorted::Sorted(){
	state = reading;
	opened_file = new File();
	curr_page = new Page();
	page_number = 0;
}

Sorted::~Sorted(){
	
}

int Sorted::Create (char *f_path, fType f_type, void *startup) {

	File 			file;
	ofstream 		header_file;
	SortInfo 		*si = (SortInfo *) startup;

	// construct path of the header meta-data file
	char header_path[100];
	sprintf (header_path, "%s.header", f_path);
	// write meta-data file depending on type of file

	header_file.open(header_path);
	if(!header_file.is_open())
		return 0;
	header_file <<"sorted"<<endl;
	header_file <<si->runLength<<endl;
	header_file <<si->myOrder->numAtts<<endl;

	for(int i=0;i<si->myOrder->numAtts;i++)
		header_file <<si->myOrder->whichAtts[i]<<endl;

	for(int i=0;i<si->myOrder->numAtts;i++)
		header_file <<si->myOrder->whichTypes[i]<<endl;

	header_file.close();


	file.Open(0,f_path);
	file.Close();
	opened_file->Open(1, f_path);
	return 1;

}

int Sorted::Load (Schema &f_schema, char *loadpath) {
	return 1;
}

int Sorted::Open (char *f_path) {
	opened_file->Open(1, f_path);
	//cout<<"there are "<<opened_file->GetLength()<<" pages in the file"<<endl;
	return 1;

}


void Sorted::MoveFirst () {

}

int Sorted::Close () {
	opened_file->Close();
	return 1;
}

int Sorted::switchToReadMode() {
	if(state == writting){
		//close inpipe, then the bigq will start phase 2
		inpipe->ShutDown();
		//do two way merge, merge records from putpipe and current dbfile into a new file.
		//create a new file
		File newFile;
		char new_path[100];
		sprintf (new_path, "%s.newFile", cur_path);
		newFile.Open(0, new_path);
		//begin merging
		Page page;
		Record pipeRec;
		Record fileRec;
		Heap heapFile;
		// play a little trick, treat this current file as a heap dbfile, whose interface will make life easier.
		heapFile.Open(cur_path);
		heapFile.MoveFirst();
		while(true){
			Record* tmp = NULL;
			if(pipeRec.isNULL())
				outpipe->Remove (&pipeRec);
			if(fileRec.isNULL())
				heapFile.GetNext(fileRec);
			if(  pipeRec.isNULL() && fileRec.isNULL()){
				if(!page.isEmpty())
					newFile.AddPageToEnd(&page);
				break;
			}
			if( pipeRec.isNULL())
				tmp = &fileRec;
			if(fileRec.isNULL())
				tmp = &pipeRec;
			if(tmp == NULL){
				tmp = ceng.Compare(&pipeRec, &fileRec, si->myOrder) == 1 ? &fileRec : &pipeRec;
			}
			//add tmp to the new file;
			if( !page.Append(tmp) ){
				newFile.AddPageToEnd(&page);
				page.EmptyItOut();
				page.Append(tmp);
			}
		}
		remove( cur_path );
		rename(new_path, cur_path);
	}
	// interchange the charater of current file and newfile

	return 1;
}

int Sorted::switchToWriteMode(){
	if(state == reading){
		int buffsz = 128;
		inpipe = new Pipe(buffsz);
		outpipe = new Pipe(buffsz);
		bigQ = new BigQ(*inpipe, *outpipe, *(si->myOrder), si->runLength);
	}
	return 1;
}
int Sorted::Add (Record &rec) {
	switchToWriteMode();
	inpipe->Insert (&rec);
	return 1;
}

int Sorted::GetNext (Record &fetchme) {
	switchToReadMode();
	return 1;
}

int Sorted::GetNext (Record &fetchme, CNF &cnf, Record &literal) {
	return 1;
}

//DBFile



DBFile::DBFile () {
	generalVar = NULL;
}

DBFile::~DBFile () {

}


int  DBFile::Create (char *f_path, fType f_type, void *startup) {
	switch(f_type){
		case heap:
			generalVar = new Heap();
			break;
		case sorted:
			generalVar = new Sorted();
			break;
		default:
			//do nothing
			break;
	}
	return generalVar->Create(f_path, f_type, startup);
}


int DBFile::Load (Schema &f_schema, char *loadpath) {
	return generalVar->Load(f_schema, loadpath);
}

int DBFile::Open (char *f_path) {

	string line;
	ifstream header_file;


	// construct path of the header meta-data file
	char header_path[100];
	sprintf (header_path, "%s.header", f_path);
	// write meta-data file depending on type of file
	header_file.open(header_path);
	if(!header_file.is_open())
		return 0;
	getline(header_file, line);
	// header_file.close();
	if( generalVar == NULL){
		if(line=="heap")
			generalVar = new Heap();

		else if(line == "sorted"){
			Sorted *sorted = new Sorted();
			generalVar = sorted;
			OrderMaker order;

			//Get Run Length from header file
			getline(header_file, line);
			int runLen = std::stoi(line);

			//Get Number of sorting attributes
			getline(header_file, line);
			order.numAtts = std::stoi(line);

			//Get Number of sorting attributes
			for(int i=0;i<order.numAtts;i++){
				getline(header_file, line);
				order.whichAtts[i] = std::stoi(line);
			}

			//Get Number of sorting attributes
			for(int i=0;i<order.numAtts;i++){
				getline(header_file, line);
				order.whichTypes[i] = (Type) std::stoi(line);
			}

			SortInfo startup = {&order, runLen};
			sorted->si = &startup;
			sorted->cur_path = f_path;
		}

	}
	printf("file opened");

	return generalVar->Open(f_path);
	//cout<<"there are "<<opened_file->GetLength()<<" pages in the file"<<endl;
}


void DBFile::MoveFirst () {
	generalVar->MoveFirst();
}

int DBFile::Close () {
	return generalVar->Close();
}

int DBFile::Add (Record &rec) {
	return generalVar->Add(rec);
}

int DBFile::GetNext (Record &fetchme) {
	return generalVar->GetNext(fetchme);
	
}

int DBFile::GetNext (Record &fetchme, CNF &cnf, Record &literal) {
	return generalVar->GetNext(fetchme, cnf, literal);
}
