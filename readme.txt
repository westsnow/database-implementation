Team Members(name + ufid):
shuai wu		78892935
Miguel Rodriguez	81984646


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
1. To run the test.cc, just type "make test”, then go to ./bin -> "./test".
	before running the test.cc, you need to modify the test.cc file, set the three file paths. 
2. To run google test, type "make gtest", then go to ./bin -> ".gtest"
	However, GTestLib can be used under OSX, if you wanna run it on linux, you need to modify line 3 and set GoogleTestDir = "your google test's lib and header file path"

IMPORTANT NOTICE:
If you met a error “segment fault”, in most case, the reason is that the test.cc file can’t locate or have no permission to the data file. Please modify lines 12-14, set the path correctly, and then make files again. (please modify the paths if things go wrong!)

If you have any question about my code, feel free to contact me at hbwestsnow@gmail.com