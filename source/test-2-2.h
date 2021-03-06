#ifndef TEST_H
#define TEST_H
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>

#include "Pipe.h"
#include "DBFile.h"
#include "Record.h"
using namespace std;

// make sure that the information below is correct
 char *dbfile_dir = "/cise/homes/shuai/DBIDATA/output/"; // dir where binary heap files should be stored
 char *tpch_dir ="/cise/homes/shuai/DBIDATA/input/"; // dir where dbgen tpch files (extension *.tbl) can be found
 char *catalog_path = "/cise/homes/shuai/DBIDATA/catalog"; // full path of the catalog file

//Shuai's Settings
//char *dbfile_dir = "/Users/westsnow/Documents/DBIDATA/output/"; // dir where binary heap files should be stored
//char *tpch_dir ="/Users/westsnow/Documents/DBIDATA/input/"; // dir where dbgen tpch files (extension *.tbl) can be found
//char *catalog_path = "/Users/westsnow/GitHub/database-implementation/source/catalog"; // full path of the catalog file
//char *dbfile_dir = "/Users/westsnow/Documents/DBIDATAoutput/"; // dir where binary heap files should be stored
//char *tpch_dir ="/Users/westsnow/Documents/DBIDATA/input/"; // dir where dbgen tpch files (extension *.tbl) can be found
//char *catalog_path = "/Users/westsnow/GitHub/database-implementation/source/catalog"; // full path of the catalog file


//Miguel's Settings
//   char *dbfile_dir = "/Users/Migue/Documents/DBIDATA/output/"; // dir where binary heap files should be stored
 //  char *tpch_dir ="/Users/Migue/Documents/DBIDATA/input/"; // dir where dbgen tpch files (extension *.tbl) can be found
  // char *catalog_path = "/Users/Migue/Development/workspace_cpp/database-implementation/source/catalog"; // full path of the catalog file


// char *catalog_path = "catalog";
// char *dbfile_dir = "";
// char *tpch_dir ="/cise/tmp/dbi_sp11/DATA/1G/";


extern "C" {
	int yyparse(void);   // defined in y.tab.c
}

extern struct AndList *final;

typedef struct {
	Pipe *pipe;
	OrderMaker *order;
	bool print;
	bool write;
}testutil;

class relation {

private:
	char *rname;
	char *prefix;
	char rpath[100]; 
	Schema *rschema;
public:
	relation (char *_name, Schema *_schema, char *_prefix) :
		rname (_name), rschema (_schema), prefix (_prefix) {
		sprintf (rpath, "%s%s.bin", prefix, rname);
	}
	char* name () { return rname; }
	char* path () { return rpath; }
	Schema* schema () { return rschema;}
	void info () {
		cout << " relation info\n";
		cout << "\t name: " << name () << endl;
		cout << "\t path: " << path () << endl;
	}

	void get_cnf (CNF &cnf_pred, Record &literal) {
		cout << "\n enter CNF predicate (when done press ctrl-D):\n\t";
  		if (yyparse() != 0) {
			cout << " Error: can't parse your CNF.\n";
			exit (1);
		}
		cnf_pred.GrowFromParseTree (final, schema (), literal); // constructs CNF predicate
	}
	void get_sort_order (OrderMaker &sortorder) {
		cout << "\n specify sort ordering (when done press ctrl-D):\n\t ";
  		if (yyparse() != 0) {
			cout << " Error: can't parse your CNF.\n";
			exit (1);
		}
		Record literal;
		CNF sort_pred;
		sort_pred.GrowFromParseTree (final, schema (), literal); // constructs CNF predicate
		OrderMaker dummy;
		sort_pred.GetSortOrders (sortorder, dummy);
	}
};


relation *rel;


char *supplier = "supplier"; 
char *partsupp = "partsupp"; 
char *part = "part"; 
char *nation = "nation"; 
char *customer = "customer"; 
char *orders = "orders"; 
char *region = "region"; 
char *lineitem = "lineitem"; 

relation *s, *p, *ps, *n, *li, *r, *o, *c;

void setup () {
	cout << " \n** IMPORTANT: MAKE SURE THE INFORMATION BELOW IS CORRECT **\n";
	cout << " catalog location: \t" << catalog_path << endl;
	cout << " tpch files dir: \t" << tpch_dir << endl;
	cout << " heap files dir: \t" << dbfile_dir << endl;
	cout << " \n\n";

	s = new relation (supplier, new Schema (catalog_path, supplier), dbfile_dir);
	ps = new relation (partsupp, new Schema (catalog_path, partsupp), dbfile_dir);
	p = new relation (part, new Schema (catalog_path, part), dbfile_dir);
	n = new relation (nation, new Schema (catalog_path, nation), dbfile_dir);
	li = new relation (lineitem, new Schema (catalog_path, lineitem), dbfile_dir);
	r = new relation (region, new Schema (catalog_path, region), dbfile_dir);
	o = new relation (orders, new Schema (catalog_path, orders), dbfile_dir);
	c = new relation (customer, new Schema (catalog_path, customer), dbfile_dir);
}

void cleanup () {
	delete s, p, ps, n, li, r, o, c;
}

#endif
