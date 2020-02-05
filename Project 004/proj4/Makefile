CXX = g++

CURDIR = $(shell pwd)
BINDIR = ./bin
TESTBIN = ./testbin
LIBDIR = ./lib
OBJDIR = ./obj
INCLUDEDIR = ./include
SRCDIR = ./src
LIBCSVDIR = libcsv-3.0.3
LIBCSV_NAME = libcsv.a

INCLUDE = -I $(INCLUDEDIR)
CXXFLAGS = -std=c++14 $(INCLUDE)
TESTLDFLAGS = -lgtest_main -lgtest -lpthread

STRING_UTILS_OBJ = $(OBJDIR)/StringUtils.o
CSV_READER_OBJ = $(OBJDIR)/CSVReader.o
CSV_WRITER_OBJ = $(OBJDIR)/CSVWriter.o
XML_READER_OBJ = $(OBJDIR)/XMLReader.o
XML_WRITER_OBJ = $(OBJDIR)/XMLWriter.o
CSV_TEST_OBJ = $(OBJDIR)/testcsv.o
XML_TEST_OBJ = $(OBJDIR)/testxml.o
MAIN_OBJ = $(OBJDIR)/main.o

CSV_TEST_NAME = testcsv 
XML_TEST_NAME = testxml

all: directories $(LIBDIR)/$(LIBCSV_NAME) runcsvtest runxmltest bin/main

runcsvtest: $(TESTBIN)/$(CSV_TEST_NAME) 
	$(TESTBIN)/$(CSV_TEST_NAME) 

runxmltest: $(TESTBIN)/$(XML_TEST_NAME)
	$(TESTBIN)/$(XML_TEST_NAME)

bin/main : $(MAIN_OBJ) $(INCLUDEDIR)/XMLEntity.h $(INCLUDEDIR)/XMLReader.h $(INCLUDEDIR)/CSVReader.h $(INCLUDEDIR)/CSVWriter.h $(INCLUDEDIR)/StringUtils.h
	$(CXX) $(MAIN_OBJ) $(STRING_UTILS_OBJ) $(CSV_WRITER_OBJ) $(CSV_READER_OBJ) $(XML_READER_OBJ) $(XML_WRITER_OBJ) -lexpat $(LIBDIR)/$(LIBCSV_NAME) $(CXXFLAGS) -o bin/main 

# $(MAIN_OBJ): $(SRCDIR)/main.cpp $(INCLUDEDIR)/StringUtils.h $(INCLUDEDIR)/XMLEntity.h $(INCLUDEDIR)/XMLReader.h $(INCLUDEDIR)/CSVReader.h $(INCLUDEDIR)/CSVWriter.h
$(MAIN_OBJ) : $(SRCDIR)/main.cpp $(INCLUDEDIR)/XMLEntity.h $(INCLUDEDIR)/XMLReader.h $(INCLUDEDIR)/CSVReader.h $(INCLUDEDIR)/CSVWriter.h $(INCLUDEDIR)/StringUtils.h
	$(CXX) $(SRCDIR)/main.cpp -c $(CXXFLAGS) -o $(MAIN_OBJ)

$(XML_READER_OBJ): $(SRCDIR)/XMLReader.cpp $(INCLUDEDIR)/XMLEntity.h $(INCLUDEDIR)/XMLReader.h
	$(CXX) $(SRCDIR)/XMLReader.cpp -c $(CXXFLAGS) $(LIBDIR)/$(LIBCSV_NAME) -o $(XML_READER_OBJ)

$(XML_WRITER_OBJ): $(SRCDIR)/XMLWriter.cpp $(INCLUDEDIR)/XMLEntity.h $(INCLUDEDIR)/XMLWriter.h
	$(CXX) $(SRCDIR)/XMLWriter.cpp -c $(CXXFLAGS) $(LIBDIR)/$(LIBCSV_NAME) -o $(XML_WRITER_OBJ)
$(CSV_READER_OBJ): $(SRCDIR)/CSVReader.cpp $(INCLUDEDIR)/CSVReader.h
	$(CXX) $(SRCDIR)/CSVReader.cpp -c $(CXXFLAGS) $(LIBDIR)/$(LIBCSV_NAME) -o $(CSV_READER_OBJ)

$(STRING_UTILS_OBJ): $(SRCDIR)/StringUtils.cpp $(INCLUDEDIR)/StringUtils.h
	$(CXX) $(SRCDIR)/StringUtils.cpp -c $(CXXFLAGS) $(LIBDIR)/$(LIBCSV_NAME) -o $(STRING_UTILS_OBJ)

$(CSV_WRITER_OBJ): $(SRCDIR)/CSVWriter.cpp $(INCLUDEDIR)/CSVWriter.h $(INCLUDEDIR)/StringUtils.h
	$(CXX) $(SRCDIR)/CSVWriter.cpp -c $(CXXFLAGS) $(LIBDIR)/$(LIBCSV_NAME) -o $(CSV_WRITER_OBJ)

$(CSV_TEST_OBJ): $(SRCDIR)/testcsv.cpp $(INCLUDEDIR)/CSVReader.h $(INCLUDEDIR)/CSVWriter.h
	$(CXX) $(SRCDIR)/testcsv.cpp $(CXXFLAGS) -c $(LIBDIR)/$(LIBCSV_NAME) -o $(CSV_TEST_OBJ)

$(XML_TEST_OBJ): $(SRCDIR)/testxml.cpp $(INCLUDEDIR)/XMLReader.h $(INCLUDEDIR)/XMLWriter.h $(INCLUDEDIR)/XMLEntity.h
	$(CXX) $(SRCDIR)/testxml.cpp $(CXXFLAGS) -c -lexpat -o $(XML_TEST_OBJ)

$(TESTBIN)/$(CSV_TEST_NAME): $(CSV_READER_OBJ) $(CSV_WRITER_OBJ) $(CSV_TEST_OBJ) $(STRING_UTILS_OBJ) $(LIBDIR)/$(LIBCSV_NAME)
	$(CXX) $(CSV_READER_OBJ) $(CSV_WRITER_OBJ) $(CSV_TEST_OBJ) $(STRING_UTILS_OBJ) $(LIBDIR)/$(LIBCSV_NAME) $(CXXFLAGS) $(TESTLDFLAGS) -o $(TESTBIN)/$(CSV_TEST_NAME)

$(TESTBIN)/$(XML_TEST_NAME): $(XML_WRITER_OBJ) $(XML_READER_OBJ) $(XML_TEST_OBJ) $(STRING_UTILS_OBJ) $(LIBDIR)/$(LIBCSV_NAME) 
	$(CXX) $(XML_READER_OBJ) $(XML_WRITER_OBJ) $(XML_TEST_OBJ) $(STRING_UTILS_OBJ) $(LIBDIR)/$(LIBCSV_NAME) $(CXXFLAGS) -lexpat $(TESTLDFLAGS) -o $(TESTBIN)/$(XML_TEST_NAME)

$(LIBCSVDIR)/Makefile:
	cd $(LIBCSVDIR); ./configure --prefix=$(CURDIR); cd ..

$(LIBCSVDIR)/libcsv.la: $(LIBCSVDIR)/Makefile
	cd $(LIBCSVDIR); make; cd ..

$(LIBDIR)/$(LIBCSV_NAME): $(LIBCSVDIR)/libcsv.la

directories: $(BINDIR) $(TESTBIN) $(OBJDIR)

$(BINDIR): 
	mkdir $(BINDIR)

$(TESTBIN): 
	mkdir $(TESTBIN)

$(OBJDIR):
	mkdir $(OBJDIR)

clean:
	rm -f lib/*
	rm -f include/csv.h
	cd $(LIBCSVDIR); make clean; cd ..
	rm -f $(LIBCSVDIR)/Makefile
