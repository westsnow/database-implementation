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
	page_buffer = Page();
}

int DBFile::Create (char *f_path, fType f_type, void *startup) {

	File file;
	ofstream header_file;

	// construct path of the header meta-data file
	char header_path[100];
	char final_path[100];
	sprintf (final_path, "%s%s.b", bin_path, f_path);
	sprintf (header_path, "%s.header", final_path);


	// write meta-data file depending on type of file
	header_file.open(header_path);
	if(!header_file.is_open())
		return 0;

	switch(f_type){
		case heap: header_file <<"heap"<<endl; break;
	}

	header_file.close();

	file.Open(0,final_path);


	return 1;
}

void DBFile::Load (Schema &f_schema, char *loadpath) {
	cout<<loadpath;

	FILE *tableFile = fopen (loadpath, "r");
	Record temp;
	//open file to write records

	File file;
	char final_path[100];
	sprintf(final_path, "%s%s.b", bin_path, "region");
	cout<<final_path;

	file.Open(1, final_path);

	//empty buffer before using
	page_buffer.EmptyItOut();

	//try to fill buffer, when full save to file
	while (temp.SuckNextRecord (&f_schema, tableFile) == 1) {
		if(page_buffer.Append(&temp) == 0){
			file.AddPageToEnd(&page_buffer);
		}
		page_buffer.EmptyItOut();
		page_buffer.Append(&temp);
	}
	file.AddPageToEnd(&page_buffer);

}

int DBFile::Open (char *f_path) {
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
