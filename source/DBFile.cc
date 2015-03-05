#include "TwoWayList.h"
#include "Record.h"
#include "Schema.h"
#include "File.h"
#include "Comparison.h"
#include "ComparisonEngine.h"
#include "DBFile.h"
#include "Defs.h"
#include <fstream>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
// stub file .. replace it with your own DBFile.cc


//Heap
heap::heap(){
	opened_file = new File();
	curr_page = new Page();
	page_number = 0;
}


int  heap::Create (char *f_path, fType f_type, void *startup) {

	File file;
	//ofstream header_file;

	// construct path of the header meta-data file
	// char header_path[100];
	// char final_path[100];
	// sprintf (final_path, "%s%s.b", bin_path, f_path);
	// sprintf (header_path, "%s.header", final_path);


	// // write meta-data file depending on type of file
	// header_file.open(header_path);
	// if(!header_file.is_open())
	// 	return 0;

	// switch(f_type){
	// 	case heap: header_file <<"heap"<<endl; break;
	// }

	// header_file.close();

	file.Open(0,f_path);
	file.Close();
	opened_file->Open(1, f_path);
	return 1;
}


int heap::Load (Schema &f_schema, char *loadpath) {

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

int heap::Open (char *f_path) {
	opened_file->Open(1, f_path);
	//cout<<"there are "<<opened_file->GetLength()<<" pages in the file"<<endl;
	return 1;
}

void heap::MoveFirst () {
	page_number = 0;
	if(curr_page->numRecs > 0) curr_page->EmptyItOut();
	opened_file->GetPage(curr_page, page_number);
}

int heap::Close () {
	//cout<<"Closing DBFile";
	opened_file->Close();
	return 1;
}

int heap::Add (Record &rec) {
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

int heap::GetNext (Record &fetchme) {
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

int heap::GetNext (Record &fetchme, CNF &cnf, Record &literal) {
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


//DBFile



DBFile::DBFile () {

}

DBFile::~DBFile () {

}


int DBFile::getPageNumber(){
	return opened_file->GetLength();
}

int  DBFile::Create (char *f_path, fType f_type, void *startup) {

	File file;
	// ofstream header_file;

	// construct path of the header meta-data file
	// char header_path[100];
	// char final_path[100];
	// sprintf (final_path, "%s%s.b", bin_path, f_path);
	// sprintf (header_path, "%s.header", final_path);


	// // write meta-data file depending on type of file
	// header_file.open(header_path);
	// if(!header_file.is_open())
	// 	return 0;

	// switch(f_type){
	// 	case heap: header_file <<"heap"<<endl; break;
	// }

	// header_file.close();

	file.Open(0,f_path);
	file.Close();
	opened_file->Open(1, f_path);
	return 1;
}


int DBFile::Load (Schema &f_schema, char *loadpath) {

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

int DBFile::Open (char *f_path) {
	opened_file->Open(1, f_path);
	//cout<<"there are "<<opened_file->GetLength()<<" pages in the file"<<endl;
	return 1;
}

void DBFile::MoveFirst () {
	page_number = 0;
	if(curr_page->numRecs > 0) curr_page->EmptyItOut();
	opened_file->GetPage(curr_page, page_number);
}

int DBFile::Close () {
	//cout<<"Closing DBFile";
	opened_file->Close();
	return 1;
}

int DBFile::Add (Record &rec) {
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

int DBFile::GetNext (Record &fetchme) {
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

// int DBFile::GetNext (Record &fetchme, CNF &cnf, Record &literal) {
// 	ComparisonEngine comp;

// 	GetNext(fetchme);

// 	while(!comp.Compare(&fetchme, &literal, &cnf)){
// 		if (GetNext(fetchme) == 0 ){
// 			return 0;
// 		}
// 	}

// 	return 1;

// }



int DBFile::GetNext (Record &fetchme, CNF &cnf, Record &literal) {
	//??? consume
	ComparisonEngine comp;
	while (GetNext(fetchme)){
		if (comp.Compare(&fetchme, &literal, &cnf)){
			return 1;
		}
	}

	return 0;
}
