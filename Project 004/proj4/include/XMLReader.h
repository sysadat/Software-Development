#ifndef XMLREADER_H
#define XMLREADER_H

#include "XMLEntity.h"
#include <istream>
#include <expat.h>

class CXMLReader{
    private:
		std::istream &Input; 
		bool isDone = false;
		XML_Parser Parser;
		SXMLEntity Entity;
		static void startHandler(void *data, const char *el, const char **attr);
		static void endHandler(void *data, const char *el);
    public:
        CXMLReader(std::istream &is);
        ~CXMLReader();
        
        bool End();
        bool ReadEntity(SXMLEntity &entity, bool skipcdata = false);
};

#endif
