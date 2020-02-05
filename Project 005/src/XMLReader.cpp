#include "XMLEntity.h"
#include "XMLReader.h"
#include "string.h"
#include <istream>
#include <expat.h>
#include <iostream>

using namespace std;

void CXMLReader::startHandler(void* data, const char* el, char const** attr) {
	SXMLEntity* UserData = static_cast<SXMLEntity *>(data);
	UserData->DNameData = std::string(el);
	for(size_t i = 0 ; attr[i]; i+=2 ) {
		UserData->SetAttribute(attr[i], attr[i+1]);
	}
}
void CXMLReader::endHandler(void *data, const char* el) {
	SXMLEntity* UserData = static_cast<SXMLEntity *>(data);
}
CXMLReader::CXMLReader(std::istream &is) : Input(is) {
	Parser = XML_ParserCreate(NULL);
	XML_SetElementHandler(Parser, startHandler, endHandler);
}
CXMLReader::~CXMLReader() {
	XML_ParserFree(Parser);
}

bool CXMLReader::End() {
	return isDone;
}
bool CXMLReader::ReadEntity(SXMLEntity &entity, bool skipcdata) {
	if (isDone) { return false; }
	char buffer[2048];
	size_t bytes_read;	
	XML_SetUserData(Parser, &entity);
	Input.getline(buffer, 2048);
	if (Input.peek() == EOF) {
		isDone = true;
	}
	bytes_read = strlen(buffer);
	XML_Parse(Parser, buffer, bytes_read, 0);
	return true;
}
