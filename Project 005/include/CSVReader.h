#ifndef CSVREADER_H
#define CSVREADER_H

#include <istream>
#include <fstream>
#include <string>
#include <vector>
#include "csv.h"
#include <sstream>

class CCSVReader{
	private:
		static void CallbackField(void *s, size_t len, void *data);
		static void CallbackRow(int ch, void *data);
		bool EOR = false;
		std::vector< std::string > RowBuffer;
		std::istream& Input;	
		struct csv_parser Parser;
    public:
        CCSVReader(std::istream &in);
        ~CCSVReader();        
        bool End() const;
        bool ReadRow(std::vector< std::string > &row);
};

#endif
