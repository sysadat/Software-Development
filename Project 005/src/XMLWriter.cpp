#include "XMLEntity.h"
#include "XMLWriter.h"
#include "StringUtils.h"
#include <istream>
#include <string>
#include <iostream>

using namespace std;

CXMLWriter::CXMLWriter(std::ostream &ou) : Output(ou) {
	/* cout << "Wrtier initialized" << endl; */
}
CXMLWriter::~CXMLWriter() {
	/* cout << "Wrtier Destroyed" << endl; */
}
bool CXMLWriter::Flush() {
	/* cout << "Flush" << endl; */
}
bool CXMLWriter::WriteEntity(const SXMLEntity &entity) {
	string ReturnString;
	if (entity.DType == SXMLEntity::EType::CompleteElement) {
		ReturnString = "<" + entity.DNameData;
		for (auto &attr : entity.DAttributes) {
			ReturnString += " " + std::get<0>(attr) + "=\"" + std::get<1>(attr) + "\"";
		}
		ReturnString += "/>";
	} else if (entity.DType == SXMLEntity::EType::StartElement) {
		ReturnString += "<" + entity.DNameData;
		for (auto &attr : entity.DAttributes) {
			ReturnString += " " + std::get<0>(attr) + "=\"" + std::get<1>(attr) + "\"";
		}
		ReturnString += ">";
	} else if (entity.DType == SXMLEntity::EType::EndElement) {
		ReturnString += "</" + entity.DNameData + ">";
	}
	Output << ReturnString << endl;
	return Output.good();
}
