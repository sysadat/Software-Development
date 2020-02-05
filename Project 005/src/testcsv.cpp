#include "CSVReader.h"
#include "CSVWriter.h"
#include "XMLReader.h"
#include "XMLWriter.h"
#include <sstream>
#include <fstream>
#include <gtest/gtest.h>
#include <iostream>

using namespace std;
void printVector(std::vector<std::string> v) {
	int i = 1;
	for (auto &s : v) {
		cout << i++ << " ";
		cout << s << endl;
	}
	cout << endl;
}
TEST(CSVReader, BasicTest) {
	std::stringstream Input("1,2 , 3, 4, 5\x0d\x0a");
	CCSVReader Reader(Input);
	std::vector< std::string > Row;
	EXPECT_TRUE(Reader.ReadRow(Row));
	EXPECT_EQ(Row.size(), 5);
	if(Row.size() >= 5) {
		EXPECT_EQ(Row[0], "1");
		EXPECT_EQ(Row[1], "2");
		EXPECT_EQ(Row[2], "3");
		EXPECT_EQ(Row[3], "4");
		EXPECT_EQ(Row[4], "5");
	}
	EXPECT_FALSE(Reader.ReadRow(Row));

	std::stringstream Input2("1, 2, 33 ,    4, 5\n");
	CCSVReader Reader2(Input2);
	std::vector <std::string> Row2;
	Reader2.ReadRow(Row2);
	EXPECT_EQ(Row2.size(), 5);
	if (Row2.size() >= 5) {
		EXPECT_EQ(Row2[0], "1");
		EXPECT_EQ(Row2[1], "2");
		EXPECT_EQ(Row2[2], "33");
		EXPECT_EQ(Row2[3], "4");
		EXPECT_EQ(Row2[4], "5");
	}

	std::stringstream Input3(",,,,,,\x0a");
	CCSVReader Reader3(Input3);
	std::vector <std::string> Row3;
	Reader3.ReadRow(Row3);
	printVector(Row3);
	EXPECT_EQ(Row3.size(), 7);
	if (Row3.size() >= 7) {
		EXPECT_EQ(Row3[0], "");
		EXPECT_EQ(Row3[1], "");
		EXPECT_EQ(Row3[2], "");
		EXPECT_EQ(Row3[3], "");
		EXPECT_EQ(Row3[4], "");
		EXPECT_EQ(Row3[5], "");
		EXPECT_EQ(Row3[6], "");
	}

	/* char test04_data[] = "\"I call our world Flatland,\x0a" */
	/* 	"not because we call it so,\x0a" */
	/* 	"but to make its nature clearer\x0a" */
	/* 	"to you, my happy readers,\x0a" */
	/* 	"who are privileged to live in Space.\""; */

  /* std::stringstream test_4(test04_data); */
	/* std::vector< std::string > Row5; */
	/* CCSVReader Reader5(test_4); */
	/* Reader5.ReadRow(Row5); */
	/* EXPECT_EQ(Row5.size(), 1); */
	/* EXPECT_EQ(Row5[0], "I call our world Flatland," */
                     /* "not because we call it so," */
                     /* "but to make its nature clearer" */
                     /* "to you, my happy readers," */
	/* 				 "who are privileged to live in Space."); */

	char test05_data[] = "\"\"\"a,b\"\"\",,\" \"\"\"\" \",\"\"\"\"\" \",\" \"\"\"\"\",\"\"\"\"\"\"";

	std::stringstream test_05(test05_data);
	std::vector<std::string> Row6;
	CCSVReader Reader6(test_05);
	Reader6.ReadRow(Row6);
	EXPECT_EQ(Row6[0], "\"a,b\"");
	EXPECT_EQ(Row6[1], "");
	EXPECT_EQ(Row6[2], " \"\" ");
	EXPECT_EQ(Row6[3], "\"\" ");
	EXPECT_EQ(Row6[4], " \"\"");
	EXPECT_EQ(Row6[5], "\"\"");
	char test06_data[] = "\" a, b ,c \", a b  c,";

	std::vector<std::string> Row7;
	std::stringstream test_06(test06_data);
	CCSVReader Reader7(test_06);
	Reader7.ReadRow(Row7);
	/* printVector(Row7); */

	char test07_data[] = "\" \"\" \" \" \"\" \"";
	char test07b_data[] = "\" \"\" \" \" \"\" \"";
}

TEST(CSVWriter, BasicTest) {	
	std::stringstream Output;
	CCSVWriter Writer(Output);
	std::vector < std::string > Row {"1", "2", "3", "4", "5"};

	EXPECT_TRUE(Writer.WriteRow(Row));
	EXPECT_EQ(Output.str(), "\"1\",\"2\",\"3\",\"4\",\"5\"\n");
	cout << Output.str() << endl;

	std::stringstream Output2;
	CCSVWriter Writer2(Output2);
	/* std::vector< std::string > Row2 {  } */
}
// <elem /> === <elem></elem>
