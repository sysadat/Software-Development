#include "StringUtils.h"
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <string>
#include <iostream>


using std::string; using std::cout; using std::endl; using std::vector;

namespace StringUtils{

std::string Slice(const std::string &str, ssize_t start, ssize_t end){
	string retString = "";
	start < 0 ? start = str.length() + start : start;
	end < 0 ? end = str.length() + end : end;
	end == 0 ? end = str.length() : end;
	for (int i = start; i < end; i++) {
		retString += str[i];
	}
	return retString;
}

std::string Capitalize(const std::string &str){
	string retString = "";
	bool found_first_char = false;
	for (auto c : str) {
		if (found_first_char) {
			if (int(c) >= 65 && int(c) <= 97) {
				c = char(int(c)+32);
			}
		}
		if (!found_first_char) {
			if (int(c) >= 97 && int(c) <= 122) {
				c = char(int(c)-32);
			}
		}
		found_first_char = true;
		retString += c;
	}
	return retString;
}

std::string Title(const std::string &str){
	bool flag = true;
	string retString = "";
	for (size_t i = 0; i < str.size(); i++) {
		int asciiVal = str[i];
		char asciiChar = str[i];
		// Flag will be true if there is a leading white space.
		// Vice versa
		if (flag) {
			// Check if the char is lower case
			if (asciiVal >= 97 && asciiVal <= 122) {
				 asciiChar =  char(asciiVal - 32);
			}
		} else {
			if (asciiVal >= 65 && asciiVal <= 90) {
				asciiChar = char(asciiVal + 32);
			}
		}
		if (asciiVal >= 32 && asciiVal <= 64 ) {
			flag = true;
		} else {
			flag = false;
		}
		retString += asciiChar;
	}
	return retString;
}

std::string LStrip(const std::string &str){
	string retString = "";
	bool first_char_found = false;
	for (size_t i = 0; i < str.size(); i++) {
		if (!first_char_found && (int(str[i]) != 32)) {
			if (str[i] != '\r') {
				first_char_found = true;
			}
		}
		if (first_char_found) {
			retString += str[i];
		}
	}
	return retString;
}

std::string RStrip(const std::string &str){
	string retString = "";
	size_t trail = 0;
	// Note: The end of a string is \0
	for (size_t i = str.size() - 1; i > 0; i--) {
		if (int(str[i]) != 32) {
			if (str[i] != '\r')
				break;
		}
		++trail;
	}
	for (size_t i = 0; i < str.size() - trail; i++) {
		retString += str[i];
	}

	return retString;
}

std::string Strip(const std::string &str){
	// copy the string
	string retString = str;
	retString = RStrip(retString);
	retString = LStrip(retString);
	return retString;
}

std::string Center(const std::string &str, int width, char fill){
	int left_pad_size = (width - str.length()) / 2;
	int right_pad_size = width - str.length() - left_pad_size;
	string left_filler = string(left_pad_size, fill);
	string right_filler = string(right_pad_size, fill);
	return left_filler + str + right_filler;
}

std::string LJust(const std::string &str, int width, char fill){
	string retString = str;
	int len = retString.length();
	if (len > width) {
		return str;
	}
	for (int i = len; i < width; i++) {
		retString += fill;
	}
	return retString;
}

std::string RJust(const std::string &str, int width, char fill){
	int len = str.length();
	if (len > width) {
		return str;
	}
	string retString = string(width - len, fill);
	retString += str;

	return retString;
}

std::string Replace(const std::string &str, const std::string &old, const std::string &rep){
	int len = old.size();
	string retString = "";
	for (size_t i = 0; i < str.size(); i++) {
		string substring = Slice(str, i, i+len);
		if (substring == old) {
			retString += rep;
			i += len-1;
		} else {
			retString += str[i];
		}
	}
	return retString;
}

std::vector< std::string > Split(const std::string &str, const std::string &splt){
	vector<string> retVector;
	int len = splt.size();
	string seen;
	if (splt == "") {
		for(auto ch : str) {
			if(ch != ' ' && ch != '\n' && ch != '\t') {
				seen += ch;
			} else {
				if (seen.length() > 0) {
					retVector.push_back(seen);
					// From cpp reference
					seen.clear();
				}
			}
		}
	} else {
		for (size_t i = 0; i < str.size(); i++) {
			if (Slice(str, i, i+len) == splt) {
				retVector.push_back(seen);
				seen.clear();
				i += len-1;
			} else {seen += str[i];}

		}
	}
	retVector.push_back(seen);
	return retVector;
}

std::string Join(const std::string &str, const std::vector< std::string > &vect){
	string retString;
	for (vector<string>::const_iterator p = vect.begin(); p != vect.end(); ++p) {
		retString += *p;
		if (p != vect.end() - 1) {
			retString += str;
		}
	}
	return retString;
}

std::string ExpandTabs(const std::string &str, int tabsize){
	int tabTracker = 0;
	string retString = "";
	for (auto ch : str) {
		tabTracker = tabTracker == tabsize ? 0 : tabTracker;
		tabTracker++;
		if (ch == '\t') {
			tabsize - tabTracker >= 0 ? retString += string(tabsize-tabTracker+1,' ') : "";
			tabTracker = 0;
		} else if (ch == ' ') {
			continue;
		} else {
			retString += ch;
		}
	}

	return retString;
}


char toLowerCase(const char c) {
	if (int(c) >= 65 && int(c) <= 90) {
		return char(int(c) + 32);
	}
	return c;
}

string toLowerCase(const string s) {
	string retString = "";
	for (auto ch : s) {
		if (int(ch) >= 65 && int(ch) <= 90) {
			retString += char(int(ch) + 32);
		} else {
			retString += ch;
		}
	}
	return retString;
}

int EditDistance(const std::string &left, const std::string &right, bool ignorecase) {
	vector<string> leftv = Split(left, " ");
	vector<string> rightv = Split(right, " ");
	int cost = 0;
	// Referenced the Wiki Professor Nitta gave as reference
	for (size_t i = 0 ; i < leftv.size(); i++) {
		string longer_string = leftv[i].length() > rightv[i].length() ? leftv[i] : rightv[i];
		string shorter_string = leftv[i].length() > rightv[i].length() ? rightv[i] : leftv[i];
		if ( ignorecase == true ) { longer_string = toLowerCase(longer_string); shorter_string = toLowerCase(shorter_string); }
		size_t longer_length= longer_string.length();
		size_t shorter_length = shorter_string.length();
		int backward_cost = 0, forward_cost = 0;
		for (size_t j = 1 ; j <= shorter_length; j++) {
			if (longer_string[longer_length - j] != shorter_string[shorter_length - j]) {
				backward_cost++;
			}
		}
		for (size_t j = 0; j < shorter_length; j++) {
			if (longer_string[j] != shorter_string[j]) {
				forward_cost++;
			}
		}
		cost += longer_length - shorter_length;
		forward_cost > backward_cost ? cost += backward_cost : cost += forward_cost;
	}
	return cost;
}

}
