#ifndef DBFILE_H
#define DBFILE_H

#include <fstream>
#include "TwoWayList.h"
#include "Record.h"
#include "Schema.h"
#include "File.h"
#include "Comparison.h"
#include "ComparisonEngine.h"

typedef enum {heap, sorted, tree} fType;

// stub DBFile header..replace it with your own DBFile.h 

class DBFile {
	File *opened_file;
	Page *curr_page;
	int page_number;
	//char *bin_path = "../binary_data/";

public:
	DBFile (); 
	~DBFile();

	int getPageNumber();
	int getCurPageNumber();
	int Create (char *fpath, fType file_type, void *startup);
	int Open (char *fpath);
	int Close ();

	//it's always a good iead to check if file exists before other operations.
	inline bool FileExists (const char* name) {
    	ifstream f(name);
    	if (f.good()) {
        	f.close();
        	return true;
    	} else {
        	f.close();
        	return false;
    	}   
	}

	int Load (Schema &myschema, char *loadpath);

	void MoveFirst ();
	int Add (Record &addme);
	int GetNext (Record &fetchme);
	int GetNext (Record &fetchme, CNF &cnf, Record &literal);

};
#endif
