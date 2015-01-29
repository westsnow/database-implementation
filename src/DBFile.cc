#include <iostream>
#include "include/TwoWayList.h"
#include "include/Record.h"
#include "include/Schema.h"
#include "include/File.h"
#include "include/Comparison.h"
#include "include/ComparisonEngine.h"
#include "include/DBFile.h"
#include "include/Defs.h"
#include <string.h>
#include <fstream>

// stub file .. replace it with your own DBFile.cc

DBFile::DBFile () {
}

int DBFile::Create (char *f_path, fType f_type, void *startup) {

	File file;
	// ofstream header_file;

	// construct path of the header meta-data file
	// char header_path[100];
	char final_path[100];
	sprintf (final_path, "%s%s.b", bin_path, f_path);
	// sprintf (header_path, "%s.header", final_path);


	// // write meta-data file depending on type of file
	// header_file.open(header_path);
	// if(!header_file.is_open())
	// 	return 0;

	// switch(f_type){
	// 	case heap: header_file <<"heap"<<endl; break;
	// }

	// header_file.close();

	file.Open(0,final_path);


	return 1;
}

void DBFile::Load (Schema &f_schema, char *loadpath) {

	Page page_buffer = Page();

	cout<<"table file localtion: "<<loadpath<<endl;

	FILE *tableFile = fopen (loadpath, "r");
	Record tmp;
	//open file to write records

	File file;
	char final_path[100];
	sprintf(final_path, "%s%s.b", bin_path, f_schema.getSchemaName());
	cout<<"path of db file : "<<final_path<<endl;

	file.Open(0, final_path);

	//empty buffer before using
	page_buffer.EmptyItOut();


   	while(tmp.SuckNextRecord(&f_schema, tableFile) ){
		//tmp.Print(&lineitem);
		if(page_buffer.Append(&tmp)){
				cout<<"the page's numRecs is "<<page_buffer.numRecs<<" and curSizeInBytes is " << page_buffer.curSizeInBytes<<endl;
		}
		else{
			file.AddPageToEnd(&page_buffer);
			page_buffer.EmptyItOut();
			cout<<"file's length is " << file.GetLength()<<endl;
		}
	}
	file.AddPageToEnd(&page_buffer);
	cout<<"file has "<<file.GetLength()<<" pages"<<endl;
	file.Close();

}

int DBFile::Open (char *f_path) {
	File file = File();
	file.Open(1, f_path);
	cout<<"there are "<<file.GetLength()<<" pages int the file"<<endl;  
}

void DBFile::MoveFirst () {
}

int DBFile::Close () {
}

void DBFile::Add (Record &rec) {
}

int DBFile::GetNext (Record &fetchme) {
}

int DBFile::GetNext (Record &fetchme, CNF &cnf, Record &literal) {
}
