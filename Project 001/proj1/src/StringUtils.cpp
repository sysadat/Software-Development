#include "StringUtils.h"
#include <algorithm>
#include <cctype>
#include <cstdio>

namespace StringUtils{

std::string Slice(const std::string &str, ssize_t start, ssize_t end){
    // Your code goes here
    std::string newString;
    for (int i = start; i < end; i++){
      newString = str[i];
    }
    return newString;
}

std::string Capitalize(const std::string &str){
    // Your code goes here
    std::string Ret;
    for(auto Ch : str){
      Ret += tolower(Ch);
    }
    Ret[0] = toupper(Ret[0]);
    return Ret;
}

std::string Title(const std::string &str){
    // Your code goes here
}

std::string LStrip(const std::string &str){
    // Your code goes here
}

std::string RStrip(const std::string &str){
    // Your code goes here
}

std::string Strip(const std::string &str){
    // Your code goes here
}

std::string Center(const std::string &str, int width, char fill){
    // Your code goes here
}

std::string LJust(const std::string &str, int width, char fill){
    // Your code goes here
}

std::string RJust(const std::string &str, int width, char fill){
    // Your code goes here
}

std::string Replace(const std::string &str, const std::string &old, const std::string &rep){
    // Your code goes here
}

std::vector< std::string > Split(const std::string &str, const std::string &splt){
    // Your code goes here
}

std::string Join(const std::string &str, const std::vector< std::string > &vect){
    // Your code goes here
}

std::string ExpandTabs(const std::string &str, int tabsize){
    // Your code goes here
}

int EditDistance(const std::string &left, const std::string &right, bool ignorecase){
    // Your code goes here
}

}
