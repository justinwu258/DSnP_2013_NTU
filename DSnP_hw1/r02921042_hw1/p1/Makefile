#CXXFLAGS="-arch i386 -arch x86_64"
p1a: p1a.o
	g++  p1a.main.cpp  p1a.cpp -o p1a.out

p1b: p1b.o
	g++  p1b.main.cpp  p1b.cpp -o p1b.out

bug: p1.bug.cpp
	g++  -g p1.bug.cpp p1a.cpp  -o p1.bug
