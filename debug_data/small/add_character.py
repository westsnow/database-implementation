import os
import sys
import glob
import fileinput

for tbl in os.listdir(sys.path[0]):
	if tbl.endswith(".tbl"):	

		for line in fileinput.input(tbl, inplace=1):
		    print line.strip() + "|"