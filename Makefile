# CXX Make variable for compiler
CXX=g++
# Make variable for compiler options
#	-std=c++11  C/C++ variant to use, e.g. C++ 2011
#	-g          include information for symbolic debugger e.g. gdb 
CXXFLAGS=-std=c++11 -g

paging : main.o 
	$(CXX) $(CXXFLAGS) -o pagingwithtlb $^

main.o: main.cpp

pagetable.o: PageTable.cpp PageTable.h

clean :
	rm *.o
