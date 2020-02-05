#include "CSVWriter.h"
#include "StringUtils.h"
#include <ostream>
#include <iostream>
#include <string>
#include <vector>

// Replace Ampersand first
// If not you will replace the Ampersand in the escaped form
// No need library for XMLWriter
CCSVWriter::CCSVWriter(std::ostream &ou) : Output(ou) {

}

CCSVWriter::~CCSVWriter() {

}

bool CCSVWriter::WriteRow(const std::vector< std::string > &row) {
	// Check the write to the stream 
	std::vector< std::string > OutputRow;
	for (auto &Field : row) {
		OutputRow.push_back(std::string("\"") + StringUtils::Replace(Field, "\"", "\"\"") + "\"");
	}
	Output << StringUtils::Join(",", OutputRow) << "\n";
	return Output.good();
}
