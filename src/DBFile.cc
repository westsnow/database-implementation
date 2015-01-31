#include <iostream>
#include "TwoWayList.cc"
#include "include/TwoWayList.h"
#include "include/Record.h"
#include "include/Schema.h"
#include "include/File.h"
#include "include/Comparison.h"
#include "include/ComparisonEngine.h"
#include "include/DBFile.h"
#include "include/Defs.h"
#include <fstream>

// stub file .. replace it with your own DBFile.cc

DBFile::DBFile () {
	opened_file = new File();
	curr_page = new Page();
	page_number = 0;
}

DBFile::~DBFile () {

}

int DBFile::getCurPageNumber(){
	return page_number;
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

	return 1;
}


int DBFile::Load (Schema &f_schema, char *loadpath) {

	if( !opened_file->isOpen())
		return 0;


	Page page_buffer = Page();

	cout<<"table file localtion: "<<loadpath<<endl;

	FILE *tableFile = fopen (loadpath, "r");
	Record tmp;
	//open file to write records





   	while(tmp.SuckNextRecord(&f_schema, tableFile) ){
		//tmp.Print(&lineitem);
		if(page_buffer.Append(&tmp)){
				cout<<"the page's numRecs is "<<page_buffer.numRecs<<" and curSizeInBytes is " << page_buffer.curSizeInBytes<<endl;
		}
		else{
			opened_file->AddPageToEnd(&page_buffer);
			page_buffer.EmptyItOut();
			page_buffer.Append(&tmp);
			cout<<"file's length is " << opened_file->GetLength()<<endl;
		}
	}
	opened_file -> AddPageToEnd(&page_buffer);
	page_buffer.EmptyItOut();
	cout<<"file has "<<opened_file->GetLength()<<" pages"<<endl;
	return 1;

}

int DBFile::Open (char *f_path) {
	opened_file->Open(1, f_path);
	cout<<"there are "<<opened_file->GetLength()<<" pages in the file"<<endl;
	return 1;
}

void DBFile::MoveFirst () {
	page_number = 0;
	if(curr_page->numRecs > 0) curr_page->EmptyItOut();
	opened_file->GetPage(curr_page, page_number);
}

int DBFile::Close () {
	cout<<"Closing DBFile";
	opened_file->Close();
	return 1;
}

int DBFile::Add (Record &rec) {
	
	Page oPage = Page();
	opened_file->GetPage(&oPage, opened_file->GetLength() - 1);
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
		if(opened_file->GetLength()>page_number){
			opened_file->GetPage(curr_page, page_number);
			curr_page->GetFirst(&fetchme);
		}
		else return 0;
	}
	return 1;
}

int DBFile::GetNext (Record &fetchme, CNF &cnf, Record &literal) {
}
