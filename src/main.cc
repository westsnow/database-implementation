
#include <iostream>
#include "include/Record.h"
#include <stdlib.h>
#include "include/DBFile.h"
using namespace std;

/*extern "C" {
	int yyparse(void);   // defined in y.tab.c
}*/

extern struct AndList *final;

int main () {

	// char *dbfile_dir = "region";
	// DBFile dbfile = DBFile();
	// // cout << "DBFile will be created at " << dbfile_dir << endl;
	// // dbfile.Create (dbfile_dir, heap, NULL);

	// char *tbl_path = "../debug_data/small/region.tbl"; // construct path of the tpch flat text file
	// cout << "tpch file will be loaded from: " << tbl_path << endl;

	// dbfile.Load(*(new Schema ("catalog", "region")), tbl_path);
	// dbfile.Close ();

	DBFile dbfile = DBFile();
	dbfile.Open("../binary_data/region.b");

}


