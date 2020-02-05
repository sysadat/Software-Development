#include <iostream>
#include "CSVReader.h"
#include "StringUtils.h"
#include "csv.h"
#include <istream>
#include <fstream>
#include <string>
#include "string.h"
#include <vector>

using std::cout; using std::endl; using std::string; using std::vector;

void printV(std::vector<string> v) {
	for (auto &s : v) {
		cout << s << endl;
	}
}
void CCSVReader::CallbackField(void *s, size_t len, void *data) {
	CCSVReader *Ptr = static_cast<CCSVReader*>(data);
	string S = std::string(static_cast<char *>(s), len);
	/* S = StringUtils::Replace(S, "\\n", "\n"); */
	Ptr->RowBuffer.push_back(S);
}
void CCSVReader::CallbackRow(int ch, void *data) {
	// When it's the end of a row
	CCSVReader *Ptr = static_cast<CCSVReader*>(data);
	/* cout << ch << endl; */
	/* Ptr->EOR = true; */
}

CCSVReader::CCSVReader(std::istream &in) : Input(in) {
	csv_init(&Parser, 0);
}

CCSVReader::~CCSVReader() {
	csv_free(&Parser);
}
static int is_space(unsigned char c) {
	if (c == CSV_SPACE || c == CSV_TAB) return 1;
	return 0;
}

static int is_term(unsigned char c) {
	if (c == CSV_CR || c == CSV_LF || c == '\x0a' || c == '\r' || c == '\n') return 1;
	return 0;
}
bool CCSVReader::End() const {
	return EOR;
}
bool CCSVReader::ReadRow(std::vector< std::string > &row) {
	if (End()) { return false; }
	csv_set_space_func(&Parser, is_space);
	csv_set_term_func(&Parser, is_term);
	size_t bytes_read;
	char buf[2048];
	/* char tempbuf[2048]; */
	Input.getline(buf, 2048);
	if (Input.peek() == EOF) {
		EOR = true;
	}
	bytes_read = strlen(buf);
	/* while (Input.getline(tempbuf, 40000)) { */
	/* 	if (!strlen(buf)) { */
	/* 		strcpy(buf, tempbuf); */
	/* 	} else { */
	/* 		strcat(buf, tempbuf); */
	/* 	} */
	/* } */
	if (csv_parse(&Parser, buf , bytes_read , CallbackField, CallbackRow, this) != strlen(buf)) {
		fprintf(stderr, "Error while parsing.\n");
		csv_strerror(csv_error(&Parser));
		exit(EXIT_FAILURE);
	};
	csv_fini(&Parser, CallbackField, CallbackRow, this);
	row = RowBuffer;
	// Resets both buffer
	if (strlen(buf))
		buf[0] = '\0';
	/* if (strlen(tempbuf)) */
	/* 	tempbuf[0] = '\0'; */

	return true;
}
