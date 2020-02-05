#include "Path.h"
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <vector>
#include <string>
#include "StringUtils.h"

using std::string; using std::vector;
struct CPath::SImplementation{
    // Your code goes here
	string absPathName;
	string directoryName;
	string baseName;
};

CPath::CPath() : DImplementation(std::make_unique< SImplementation >()){
    // Your code goes here

}

CPath::CPath(const CPath &path) : DImplementation(std::make_unique< SImplementation >()){
    // Your code goes here
	string absPath = path.ToString();
	vector<string> splitted = StringUtils::Split(absPath, "/");
	DImplementation->directoryName = "";
	DImplementation->absPathName = absPath;
	string dName;
	if (splitted.size() > 2) {
		DImplementation->directoryName = splitted[0] + "/" + splitted[1];
	} else {
		DImplementation->directoryName = splitted[0];
	}
	DImplementation->baseName = splitted[splitted.size() - 1];
}

CPath::CPath(const std::string &path): DImplementation(std::make_unique< SImplementation >()) {
    // Your code goes here
	DImplementation->absPathName = path;
	vector<string> splitted = StringUtils::Split(path, "/");
	DImplementation->directoryName = "";
	string dName;
	if (splitted.size() > 2) {
		for (size_t i = 0; i < splitted.size() - 1; i++) {
			i == splitted.size() - 2 ? dName += splitted[i] : dName += splitted[i] + "/";
		}
		DImplementation->directoryName = dName;
	} else {
		DImplementation->directoryName = splitted[0];
	}
	DImplementation->baseName = splitted[splitted.size() - 1];
}

CPath::~CPath(){
    // Your code goes here
	DImplementation.reset();
}

CPath &CPath::operator=(const CPath &path){
    // Your code goes here
	DImplementation->absPathName = path.ToString();
}

CPath CPath::operator+(const CPath &path) const{
    // Your code goes here
	return DImplementation->absPathName + "/" + path.ToString();
}

bool CPath::operator==(const CPath &path) const{
    // Your code goes here
	return DImplementation->absPathName == path.ToString();
}

bool CPath::operator!=(const CPath &path) const{
    // Your code goes here
	return DImplementation->absPathName != path.ToString();
}

CPath CPath::Directory() const{
    // Your code goes here
	if (DImplementation->absPathName == "") {
		return CPath();
	}
	return CPath(DImplementation->directoryName);
}

std::string CPath::BaseName() const{
    // Your code goes here
	return DImplementation->baseName;
}

std::string CPath::Extension() const{
    // Your code goes here
	string baseName = BaseName();
	for (auto c : baseName) {
		if (c == '.') {
			return "." + StringUtils::Split(baseName, ".")[1];
		}
	}
	return "";
}

bool CPath::IsAbsolute() const{
    // Your code goes here
	if (DImplementation->absPathName.length() == 0) return false;
	return DImplementation->absPathName[0] != '.';
}

std::string CPath::ToString() const{
    // Your code goes here
	return DImplementation->absPathName;
}

CPath::operator std::string() const{
    // Your code goes here
	return ToString();
}

CPath CPath::AbsolutePath() const{
    // Your code goes here
	return DImplementation->absPathName;
}

CPath CPath::CommonPath(const CPath &path) const{
    // Your code goes here
	vector<string> splitted = StringUtils::Split(AbsolutePath(), "/");
	vector<string> pathSplitted = StringUtils::Split(path.AbsolutePath(), "/");
	string common;
	for (size_t i = 0 ; i < splitted.size(); i++) {
		if (splitted[i] == pathSplitted[i]) common+= splitted[i] + "/";
		else { break; }
	}
	return CPath(common);
}

CPath CPath::NormalizePath() const{
    // Your code goes here
	string absPath = DImplementation->absPathName;
	string normalizedPath = StringUtils::Replace(absPath, "./", "");
	return CPath(normalizedPath);
}

CPath CPath::RelativePathTo(const CPath &path) const{
    // Your code goes here
	return RelativePath(CPath(ToString()), path);
}

// Helper function provided to get the current working directory
std::string CPath::GetCWD(){
    std::vector< char > CurrentPathName;

    CurrentPathName.resize(1024);
    while(NULL == getcwd(CurrentPathName.data(), CurrentPathName.size())){
        CurrentPathName.resize(CurrentPathName.size() * 2);
    }
    return CurrentPathName.data();
}

// Helper funciton provided to get the home directory
std::string CPath::GetHome(){
    return std::getenv("HOME");
}

CPath CPath::AbsolutePath(const CPath &path){
    // Your code goes here
	return path.AbsolutePath();
}

CPath CPath::CurrentPath(){
    // Your code goes here
	return CPath(GetCWD());
}

CPath CPath::CommonPath(const CPath &path1, const CPath &path2){
    // Your code goes here
	vector<string> splitted1 = StringUtils::Split(path1.AbsolutePath(), "/");
	vector<string> splitted2 = StringUtils::Split(path2.AbsolutePath(), "/");
	string common;
	for (size_t i = 0 ; i < splitted1.size(); i++) {
		if (splitted1[i] == splitted2[i]) common+= splitted1[i] + "/";
		else { break; }
	}
	return CPath(common);
}

CPath CPath::ExpandUserPath(const CPath &path){
    // Your code goes here
	string expanded = GetHome() + StringUtils::Replace(path.ToString(), "~/", "/");
	return CPath(expanded);
}

CPath CPath::HomePath(){
    // Your code goes here
	return CPath(GetHome());
}

CPath CPath::NormalizePath(const CPath &path){
    // Your code goes here
	string absPath = path.ToString();
	string normalizedPath = StringUtils::Replace(absPath, "./", "");
	return CPath(normalizedPath);
}

CPath CPath::RelativePath(const CPath &path, const CPath &startpath){
    // Your code goes here
	string relativePath = CommonPath(path, startpath);
	CPath normalizedPathCPath = CPath(StringUtils::Replace(relativePath, "./", ""));
	string normalizedPath = normalizedPathCPath.ToString();
	string currentPath = NormalizePath(startpath).ToString();
	return CPath("../" + StringUtils::Slice(currentPath, normalizedPath.length()));
}
