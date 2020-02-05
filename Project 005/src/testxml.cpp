#include "XMLReader.h"
#include "XMLWriter.h"
#include <sstream>
#include <fstream>
#include <gtest/gtest.h>
#include <iostream>

using namespace std;

TEST(XMLReader, BasicTest) {
	EXPECT_EQ(true, true);
	SXMLEntity testEntity;
	testEntity.SetAttribute("elem", "hello");
	EXPECT_TRUE(testEntity.AttributeExists("elem"));
	EXPECT_EQ(testEntity.AttributeValue("elem"), "hello");

	std::stringstream test1("<elem attr=\"lmao\" name=\"haha\" id=\"lmaoooo\"></elem>");
	CXMLReader Reader(test1);
	SXMLEntity ToRead;
	EXPECT_FALSE(Reader.End());
	Reader.ReadEntity(ToRead);
	EXPECT_EQ(ToRead.DNameData, "elem");
	EXPECT_TRUE(ToRead.AttributeExists("attr"));
	EXPECT_TRUE(ToRead.AttributeExists("name"));
	EXPECT_TRUE(ToRead.AttributeExists("id"));
	EXPECT_FALSE(ToRead.AttributeExists("fake"));
	EXPECT_EQ(ToRead.AttributeValue("attr"), "lmao");
	EXPECT_EQ(ToRead.AttributeValue("name"), "haha");
	EXPECT_EQ(ToRead.AttributeValue("id"), "lmaoooo");
	EXPECT_TRUE(Reader.End());
	EXPECT_FALSE(Reader.ReadEntity(ToRead));
	ifstream TestFile ("src/ABC_114_2.xml");
	SXMLEntity ToRead2;
	SXMLEntity ToRead3;
	CXMLReader Reader2(TestFile);
	EXPECT_TRUE(Reader2.ReadEntity(ToRead2));
	EXPECT_EQ(ToRead2.DNameData, "course");
	EXPECT_TRUE(Reader2.ReadEntity(ToRead3));
	EXPECT_EQ(ToRead3.DNameData, "student");

	std::stringstream toWrite;
	CXMLWriter Writer(toWrite);
	ToRead.DType = SXMLEntity::EType::CompleteElement;
	Writer.WriteEntity(ToRead);
	EXPECT_EQ(toWrite.str(), "<elem attr=\"lmao\" name=\"haha\" id=\"lmaoooo\"/>\n");
	cout << toWrite.str() << endl;

}
