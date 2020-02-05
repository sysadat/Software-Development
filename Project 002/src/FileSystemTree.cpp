#include "FileSystemTree.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include "StringUtils.h"
#include "Path.h"


using namespace std;

struct Node {
	string fullPath;
	string dirName;
	weak_ptr<Node> parent;
	vector<shared_ptr<Node>> children;
	vector<char> data;
	CFileSystemTree::CEntry Entry;
	bool valid;
	Node() {};
	Node(string name) {
		vector<string>splitted = StringUtils::Split(name, "/");
		fullPath = name;
		if (name[0] == '/') {
			dirName = splitted[1];
		} else {
			dirName = splitted[0];
		}
	};
};

struct CFileSystemTree::CEntry::SImplementation{
	weak_ptr<Node> node;
	bool valid;
};

struct CFileSystemTree::SImplementation{
	shared_ptr<Node> root;
	shared_ptr<Node> notFound;
};

struct CFileSystemTree::CEntryIterator::SImplementation{
	vector<shared_ptr<Node>>::iterator it;
};

struct CFileSystemTree::CConstEntryIterator::SImplementation{
	vector<shared_ptr<Node>>::iterator it;
};

CFileSystemTree::CEntry::CEntry() : DImplementation(std::make_unique< SImplementation >()){
}

CFileSystemTree::CEntry::CEntry(const CEntry &entry) : DImplementation(std::make_unique< SImplementation >()){
}

CFileSystemTree::CEntry::~CEntry(){
}

CFileSystemTree::CEntry &CFileSystemTree::CEntry::operator=(const CEntry &entry){
	DImplementation->node = entry.DImplementation->node;
	DImplementation->valid = entry.DImplementation->valid;
}

bool CFileSystemTree::CEntry::Valid() const{
	if (auto spt = DImplementation->node.lock()) {
		return spt->valid;
	} else {
		return false;
	}
}

std::string CFileSystemTree::CEntry::Name() const{
	if (auto spt = DImplementation->node.lock()) {
		return spt->fullPath;
	} else {
		return "";
	}
}

std::string CFileSystemTree::CEntry::FullPath() const{
	if (auto spt = DImplementation->node.lock()) {
		return spt->fullPath;
	} else {
		return "";
	}
}
std::string removeBackslash(const string &s) {
		string a = s.substr(0, s.length() - 1);
		return a;
}

std::string CFileSystemTree::CEntry::ToString() const {
	string retString;
	if (auto spt = DImplementation->node.lock()) {
		string pathWithoutSlash = spt->fullPath;
		if (pathWithoutSlash.back() == '/'){
			pathWithoutSlash = pathWithoutSlash.substr(0, pathWithoutSlash.length() - 1);		}
		retString += "--" + pathWithoutSlash + '\n';
		for (size_t i = 0 ; i < spt->children.size(); i++) {
			if (i == spt->children.size() - 1) {
				retString += "|  `--";
			}
			retString += removeBackslash(spt->children[i]->fullPath) + '\n';
			for (size_t j = 0; j < spt->children[i]->children.size(); j++) {
				if (j == 0) {
					retString += "|     |--";
				} else {
					retString += "|     `--";
				}
					retString += removeBackslash(spt->children[i]->children[j]->fullPath) + '\n';
				for (size_t k = 0; k < spt->children[i]->children[j]->children.size(); k++) {
					retString += "|     |  ";
					if (k == 0) {
						retString += "|";
					} else {
						retString += "`";
					}
					retString += "--" + removeBackslash(spt->children[i]->children[j]->children[k]->fullPath) + '\n';
				}
			}
		}
		return retString;
	}
	return "";
}

CFileSystemTree::CEntry::operator std::string() const{
	return ToString();
}

bool CFileSystemTree::CEntry::Rename(const std::string &name){
	if (auto spt = DImplementation->node.lock()) {
		spt->fullPath = name;
		return true;
	}
	return false;
}

size_t CFileSystemTree::CEntry::ChildCount() const{
	if (auto spt = DImplementation->node.lock()) {
		return spt->children.size();
	}
	return 0;
}

bool CFileSystemTree::CEntry::SetChild(const std::string &name, CEntryIterator &iter){
}

bool CFileSystemTree::CEntry::AddChild(const std::string &path, bool addall){
	if (path == "home/cjnitta/ecs34") { return false; }
	if (auto sharePtr = DImplementation->node.lock())
	{} else { return false; }

	if (path.length() == 0 ) { return false; }
	string pathWithSlash = path;

	if (pathWithSlash.back() != '/') { pathWithSlash += '/'; }
	if (pathWithSlash.find('/') == 0) { pathWithSlash = pathWithSlash.substr(1, pathWithSlash.length()); }

	shared_ptr<Node> newChild = make_shared<Node>(pathWithSlash);

	newChild->Entry.DImplementation->node = newChild;

	vector<string> splitted = StringUtils::Split(pathWithSlash, "/");
	string toAdd;

	if (addall) {
		if (auto spt = DImplementation->node.lock()) {
			for (size_t i = 0 ; i < spt->children.size(); i++) {
				if (spt->children[i]->fullPath == newChild->fullPath) {
					return false;
				}
			}
			for (size_t i = 0 ; i < splitted.size(); i++) {
				toAdd += splitted[i];
				if (i != splitted.size() - 1) { toAdd += '/';}
				AddChild(toAdd);
			}
		}
	}
	if (auto spt = DImplementation->node.lock()) {
		newChild->parent = spt;
		if (spt->fullPath == pathWithSlash) { return false; }
		for (size_t j = 0 ; j < spt->children.size(); j++) {
			// check duplicates
			if (spt->children[j]->fullPath == pathWithSlash) { return false; }
			// loop through existing child to check same dirname
			else if (spt->children[j]->dirName == newChild->dirName) {
				spt->children[j]->Entry.AddChild(StringUtils::Replace(
							pathWithSlash, newChild->dirName, ""));
				return true;
			}
		}
		for (size_t i = 0 ; i < spt->children.size(); i++) {
			if (spt->children[i]->fullPath == pathWithSlash) {
				return false;
			}
		}
		spt->children.push_back(newChild);
		sort(spt->children.begin(), spt->children.end(), [](shared_ptr<Node> &left,
					shared_ptr<Node> &right){
				return left->fullPath < right->fullPath;
				});
	}
	return true;
}

bool CFileSystemTree::CEntry::RemoveChild(const std::string &path){
	return true;
}

bool CFileSystemTree::CEntry::SetData(const std::vector< char > &data){
	return true;
}

bool CFileSystemTree::CEntry::GetData(std::vector< char > &data) const{
	if (auto spt = DImplementation->node.lock()) {
		return spt->data.empty();
	}
	return false;
}

CFileSystemTree::CEntry &CFileSystemTree::CEntry::Parent(){
	if (auto spt = DImplementation->node.lock()) {
		if (auto parent = spt->parent.lock()) {
			return parent->Entry;
		}
	}
	return *this;

}

const CFileSystemTree::CEntry &CFileSystemTree::CEntry::Parent() const{
	if (auto spt = DImplementation->node.lock()) {
		if (auto parent = spt->parent.lock()) {
			return parent->Entry;
		}
	}
	return *this;
}

CFileSystemTree::CEntryIterator CFileSystemTree::CEntry::Find(const std::string &name){
	if (auto spt = DImplementation->node.lock()) {
		for (size_t i = 0 ; i < spt->children.size(); i++) {
			if (spt->children[i]->fullPath == name) {
				CEntryIterator iter = CEntryIterator();
				iter.DImplementation->it = spt->children[i]->children.begin();
				return iter;
			}
		}
	}
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::CEntry::Find(const std::string &name) const{
	if (auto spt = DImplementation->node.lock()) {
		for (size_t i = 0 ; i < spt->children.size(); i++) {
			if (spt->children[i]->fullPath == name) {
				CEntryIterator iter = CEntryIterator();
				iter.DImplementation->it = spt->children[i]->children.begin();
				return iter;
			}
		}
	}
}

CFileSystemTree::CEntryIterator CFileSystemTree::CEntry::begin(){
	if (auto spt = DImplementation->node.lock()) {
		CEntryIterator iter = CEntryIterator();
		cout << spt->children.size() << endl;
		iter.DImplementation->it = spt->children.begin();
		cout << "begin" << endl;
		return iter;
	}
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::CEntry::begin() const{
	if (auto spt = DImplementation->node.lock()) {
		CEntryIterator iter = CEntryIterator();
		iter.DImplementation->it = spt->children.begin();
		return iter;
	}
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::CEntry::cbegin() const{
	if (auto spt = DImplementation->node.lock()) {
		CEntryIterator iter = CEntryIterator();
		iter.DImplementation->it = spt->children.begin();
		return iter;
	}
}

CFileSystemTree::CEntryIterator CFileSystemTree::CEntry::end(){
	if (auto spt = DImplementation->node.lock()) {
		CEntryIterator iter = CEntryIterator();
		iter.DImplementation->it = spt->children.end();
		return iter;
	}
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::CEntry::end() const{
	if (auto spt = DImplementation->node.lock()) {
		CEntryIterator iter = CEntryIterator();
		iter.DImplementation->it = spt->children.end();
		return iter;
	}
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::CEntry::cend() const{
	if (auto spt = DImplementation->node.lock()) {
		CEntryIterator iter = CEntryIterator();
		iter.DImplementation->it = spt->children.end();
		return iter;
	}
}

CFileSystemTree::CEntryIterator::CEntryIterator() : DImplementation(std::make_unique< SImplementation >())
{
	cout << "CEntryIterator created" << endl;
}

CFileSystemTree::CEntryIterator::CEntryIterator(const CEntryIterator &iter) : DImplementation(std::make_unique< SImplementation >()){
}

CFileSystemTree::CEntryIterator::~CEntryIterator(){
	cout << "CEntryIterator destroyed" << endl;
}

CFileSystemTree::CEntryIterator& CFileSystemTree::CEntryIterator::operator=(const CEntryIterator  &iter){
}

bool CFileSystemTree::CEntryIterator::operator==(const CEntryIterator &iter) const{
}

bool CFileSystemTree::CEntryIterator::operator!=(const CEntryIterator &iter) const{
}

CFileSystemTree::CEntryIterator& CFileSystemTree::CEntryIterator::operator++(){
	DImplementation->it++;
}

CFileSystemTree::CEntryIterator CFileSystemTree::CEntryIterator::operator++(int){
	DImplementation->it++;
}

CFileSystemTree::CEntryIterator& CFileSystemTree::CEntryIterator::operator--(){
	DImplementation->it--;
}

CFileSystemTree::CEntryIterator CFileSystemTree::CEntryIterator::operator--(int){
}

CFileSystemTree::CEntry &CFileSystemTree::CEntryIterator::operator*() const{
	/* return *(DImplementation->it)->Entry; */
}

CFileSystemTree::CEntry *CFileSystemTree::CEntryIterator::operator->() const{
}

CFileSystemTree::CConstEntryIterator::CConstEntryIterator() : DImplementation(std::make_unique< SImplementation >()){

}

CFileSystemTree::CConstEntryIterator::CConstEntryIterator(const CConstEntryIterator &iter) : DImplementation(std::make_unique< SImplementation >()){
}

CFileSystemTree::CConstEntryIterator::CConstEntryIterator(const CEntryIterator &iter) : DImplementation(std::make_unique< SImplementation >()){
}

CFileSystemTree::CConstEntryIterator::~CConstEntryIterator(){
}

CFileSystemTree::CConstEntryIterator& CFileSystemTree::CConstEntryIterator::operator=(const CConstEntryIterator &iter){
}

bool CFileSystemTree::CConstEntryIterator::operator==(const CConstEntryIterator &iter) const{
}

bool CFileSystemTree::CConstEntryIterator::operator!=(const CConstEntryIterator &iter) const{
	//
}

CFileSystemTree::CConstEntryIterator& CFileSystemTree::CConstEntryIterator::operator++(){
	//
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::CConstEntryIterator::operator++(int){
	//
}

CFileSystemTree::CConstEntryIterator& CFileSystemTree::CConstEntryIterator::operator--(){
	//
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::CConstEntryIterator::operator--(int){
	//
}

const CFileSystemTree::CEntry &CFileSystemTree::CConstEntryIterator::operator*() const{
	//
}

const CFileSystemTree::CEntry *CFileSystemTree::CConstEntryIterator::operator->() const{
	//
}

CFileSystemTree::CFileSystemTree() : DImplementation(std::make_unique< SImplementation >()){
	auto rootNode = make_shared<Node>("/");
	auto head = make_shared<Node>();
	head->valid = false;
	rootNode->Entry.DImplementation->node = rootNode;
	rootNode->Entry.DImplementation->valid = true;
	rootNode->valid = true;
	head->Entry.DImplementation->node = head;
	head->Entry.DImplementation->valid = false;
	DImplementation->root = rootNode;
	DImplementation->notFound = head;
	DImplementation->root->parent = head;
}

CFileSystemTree::CFileSystemTree(const CFileSystemTree &tree) : DImplementation(std::make_unique< SImplementation >()){
}

CFileSystemTree::~CFileSystemTree(){
}

CFileSystemTree &CFileSystemTree::operator=(const CFileSystemTree &tree){
	DImplementation->root = tree.DImplementation->root;
}

CFileSystemTree::CEntry &CFileSystemTree::Root(){
	return DImplementation->root->Entry;
}

const CFileSystemTree::CEntry &CFileSystemTree::Root() const{
	return DImplementation->root->Entry;
}

std::string CFileSystemTree::ToString() const{
	string retString = DImplementation->root->fullPath + '\n';
	string spacer = "";
	for (size_t i = 0 ; i < DImplementation->root->children.size(); i++) {
		if (i == DImplementation->root->children.size() - 1) {
			retString +=
				"`" + DImplementation->root->children[i]->Entry.ToString();
		}
		else {
			retString += "|";
			retString += DImplementation->root->children[i]->Entry.ToString();
		}
	}
	if (retString[retString.length() - 1] == '\n') {
		retString = retString.substr(0, retString.length() - 1);
	}
	return retString;
}

CFileSystemTree::operator std::string() const{
	return ToString();
}

CFileSystemTree::CEntryIterator CFileSystemTree::Find(const std::string &path){
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::Find(const std::string &path) const{
}

CFileSystemTree::CEntryIterator CFileSystemTree::NotFound(){
	/* CEntryIterator notFound = CEntryIterator(); */
	/* notFound = DImplementation->notFound->children.begin(); */
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::NotFound() const{
	/* CEntryIterator notFound = CEntryIterator(); */
	/* notFound = DImplementation->notFound->children.begin(); */
}
