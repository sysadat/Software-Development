CXX = g++

INCLUDE = -I src/
CXXFLAGS = -std=c++14 $(INCLUDE)
PROJ_NAME = proj3
TESTMANCALA_NAME = testmancala

obj/%.o: src/%.cpp
	mkdir -p obj
	$(CXX) $(CXXFLAGS) -c $< -o $@

all: bin/mancala test 

test: testbin/testmancala
	mkdir -p testbin
	testbin/testmancala

bin/mancala : obj/main.o obj/MancalaBoard.o
	mkdir -p bin
	$(CXX) $(CXXFLAGS) obj/main.o obj/MancalaBoard.o -o bin/mancala -DDEBUG

testbin/testmancala: obj/main.o obj/MancalaBoard.o obj/testmancala.o
	mkdir -p testbin
	$(CXX) $(CXXFLAGS) obj/MancalaBoard.o obj/testmancala.o -o testbin/testmancala -g -ggdb -D_GLIBCXX_DEBUG -DDEBUG -lgtest -lgtest_main -lpthread

clean: 
	rm obj/*
