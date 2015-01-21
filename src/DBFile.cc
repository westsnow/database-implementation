#include "include/TwoWayList.h"
#include "include/Record.h"
#include "include/Schema.h"
#include "include/File.h"
#include "include/Comparison.h"
#include "include/ComparisonEngine.h"
#include "include/DBFile.h"
#include "include/Defs.h"

// stub file .. replace it with your own DBFile.cc

DBFile::DBFile () {

}

int DBFile::Create (char *f_path, fType f_type, void *startup) {
}

void DBFile::Load (Schema &f_schema, char *loadpath) {
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
