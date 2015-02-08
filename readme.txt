Organazation:
./
./readme.txt
./Makefile
./source/
./bin/
./gtest
./GTestLib

1. code files are in source directory.
2. XXX.o and executable files are in bin directory
3. gtest contains codes for google test.
4. GTestLib contains some .h and lib files that google test file may need.

Run & Test my program
1. To run the test.cc, just type "make", then go to ./bin -> "./test".
	before running the test.cc, you need to modify the test.cc file, set the three file paths. 
2. To run google test, type "make gtest", then go to ./bin -> ".gtest"
	However, GTestLib can be used under OSX, if you wanna run it on linux, you need to modify line 3 and set GoogleTestDir = "your google test's lib and header file path"