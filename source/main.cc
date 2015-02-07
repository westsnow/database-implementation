
#include <iostream>
#include "Record.h"
#include <stdlib.h>
#include "DBFile.h"
using namespace std;

extern "C" {
	int yyparse(void);   // defined in y.tab.c
}

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

	dbfile.Create("/Users/westsnow/Documents/DBIDATA/output/nation.b", heap, NULL);
	dbfile.Open("/Users/westsnow/Documents/DBIDATA/output/nation.b");



	dbfile.Load(*(new Schema ("/Users/westsnow/GitHub/dbi/source/catalog", "nation")),  "/Users/westsnow/Documents/DBIDATA/input/nation.tbl");
	
//	dbfile.MoveFirst();
	Record r = Record();
//
//	cout<<"cur page number: "<<dbfile.getCurPageNumber()<<endl;
//	dbfile.GetNext(r);
//			r.Print(new Schema ("catalog", "nation"));

	
//	dbfile.Add(r);

	dbfile.MoveFirst();

	CNF cnf;
	Record literal;

	cout << " Enter CNF predicate (when done press ctrl-D):\n\t";
	if (yyparse() != 0) {
		std::cout << "Can't parse your CNF.\n";
		exit (1);
	}
	cnf.GrowFromParseTree (final, new Schema ("/Users/westsnow/GitHub/dbi/source/catalog", "nation"), literal);


	for(int i = 0; i < 100; ++i){
		if(dbfile.GetNext(r, cnf, literal)){
			r.Print(new Schema ("/Users/westsnow/GitHub/dbi/source/catalog", "nation"));
			cout<<"cur page number: "<<dbfile.getCurPageNumber()<<endl;
		}else{
			cout<<"end of file"<<endl;
			break;
		}
	}



	

	dbfile.Close();
}


