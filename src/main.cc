
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

//	 char *dbfile_dir = "region";
	 DBFile dbfile = DBFile();
//	 cout << "DBFile will be created at " << dbfile_dir << endl;
//	 dbfile.Create (dbfile_dir, heap, NULL);
//
//	 char *tbl_path = "../debug_data/small/region.tbl"; // construct path of the tpch flat text file
//	 cout << "tpch file will be loaded from: " << tbl_path << endl;
//
//	 dbfile.Load(*(new Schema ("catalog", "region")), tbl_path);
//	 dbfile.Close ();

	// char *fileLocation = "../binary_data/region.b";
	// DBFile dbfile = DBFile();
	// if(dbfile.FileExists(fileLocation)){
	// 	cout<<"file already exists"<<endl;
	// }else{
	// 	dbfile.Create(fileLocation, heap, NULL);
 //    }

 //    dbfile.Load(*(new Schema ("catalog", "region")),  "../debug_data/small/region.tbl" ,fileLocation);

	dbfile.Create("../binary_data/nation.b", heap, NULL);
	dbfile.Open("../binary_data/nation.b");

	dbfile.Load(*(new Schema ("catalog", "nation")),  "../debug_data/small/nation.tbl");
	cout<<"moving first"<<endl;
	dbfile.MoveFirst();
	Record r = Record();
	dbfile.GetNext(r);
	r.Print(new Schema ("catalog", "nation"));
	dbfile.GetNext(r);
	r.Print(new Schema ("catalog", "nation"));

	dbfile.GetNext(r);
	r.Print(new Schema ("catalog", "nation"));

	dbfile.Close();
}


