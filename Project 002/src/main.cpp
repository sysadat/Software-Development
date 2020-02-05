#include <iostream>
#include "DirectoryListing.h"
#include "FileSystemTree.h"
#include "StringUtils.h"
#include "Path.h"
#include <fstream>
using namespace std;

void DFS(const std::string &path, CFileSystemTree::CEntry &entry);

int main(int argc, char *argv[]){
	CFileSystemTree TheTree;
	// Your code here
	if(argc >= 2){
		for(int i = 0; i < argc; i++){
			std::cout<<argv[i]<<std::endl;
		}
	}
	bool Done = false;
	std::string CurrentDirectory = "/";
	DFS(".",TheTree.Root());
	while(!Done){
		std::string InputLine;
		std::cout<<"> ";
		std::getline(std::cin, InputLine);
		if(InputLine == "exit"){
			Done = true;
		}
		else if(InputLine == "pwd"){
			std::cout<<CurrentDirectory<<std::endl;
		}
		else if(InputLine.substr(0,2) == "cd"){
			if (CurrentDirectory == "/" && InputLine == "cd ..") {
				continue;
			}
			if (InputLine == "cd .") {
				continue;
			}
			if (InputLine == "cd ~" || InputLine == "cd") {
				CurrentDirectory = "/";
			} else if (InputLine == "cd .." || InputLine == "cd ../") {
				CurrentDirectory = TheTree.Find(CurrentDirectory)->Parent().Name();
			} else {
				auto iter = TheTree.Find(InputLine.substr(3));
				if (iter != TheTree.NotFound() && iter->ChildCount() > 0) {
					CurrentDirectory = InputLine.substr(3);
				} else {
				}
			}
		}
		else if(InputLine.substr(0,3) == "cat") {
			string s = InputLine.substr(4);
			std::vector<char> v(s.begin(), s.end());
			std::ifstream myfile;
			string line;
			auto entry = TheTree.Find(s);
			if (entry == TheTree.NotFound()) {
				entry = TheTree.Find(CurrentDirectory+ "/" + s);
			}
			string toOpen = entry->FullPath();
			if (toOpen[0] == '/') {
				toOpen = toOpen.substr(1);
			}
			myfile.open (toOpen);
			if (myfile.is_open()) {
				while (getline(myfile, line)) {
					cout << line << endl;
				}
				myfile.close();
			} else {
			}
		}
		// works
		else if(InputLine.substr(0,4) == "tree"){
			if (InputLine.length() == 4) {
				if (CurrentDirectory == "/") {
					cout << std::string(TheTree) << endl;
				} else {
					auto iter = TheTree.Find(CurrentDirectory);
					std::cout << iter->ToString() << endl;
				}
			} else {
				string fname = InputLine.substr(5);
				CFileSystemTree::CEntryIterator iter = TheTree.Find(fname);
				cout << iter->ToString() << endl;
			}
		}
		else if(InputLine.substr(0,5) == "mkdir"){
			string newDir = InputLine.substr(6);
			if (CurrentDirectory == "/") {
				TheTree.Root().AddChild(newDir, true);
			} else {
				TheTree.Find(CurrentDirectory)->AddChild(newDir,true);
			}
		}
		else if(InputLine.substr(0,2) == "ls"){
			// ls CurrentDirectory
			if(InputLine.length() == 2){
				if (CurrentDirectory == "/") {
					for(auto iter = TheTree.Root().begin();
							iter != TheTree.Root().end(); iter++) {
						cout << iter->Name() << endl;
					}
				} else {
				for (auto iter = TheTree.Find(CurrentDirectory)->begin();
						iter != TheTree.Find(CurrentDirectory)->end();
						iter++){
					cout << iter->Name()<< endl;
				}
				}
			}
			else if(InputLine.length() > 2){
				//ls selected directory
				string findItem = InputLine.substr(3);
				for (auto iter = TheTree.Find(findItem)->begin();
						iter != TheTree.Find(findItem)->end(); iter++) {
					cout << iter->Name() << endl;
				}
			}
		} else if (InputLine.substr(0,2) == "cp") {
			vector<string> splitted = StringUtils::Split(InputLine);
			string src = splitted[1];
			string dest = splitted[2];
			auto srcIter = TheTree.Find(src);
			auto destIter = TheTree.Find(dest);
			if (srcIter == TheTree.NotFound()) {
				srcIter = TheTree.Find(CurrentDirectory + "/" + src);
			}
			if (destIter == TheTree.NotFound()) {
				destIter = TheTree.Find(CurrentDirectory + "/" + dest);
			}
			if (destIter != TheTree.NotFound()) {
				destIter->AddChild(src);
			} else {
				srcIter->Rename(dest);
			}
		}
		else if (InputLine.substr(0,2) == "mv") {
			vector<string> splitted = StringUtils::Split(InputLine);
			string src = splitted[1];
			string dest = splitted[2];
			auto srcIter = src == "/" ? TheTree.Root().begin() : TheTree.Find(src);
			auto destIter = dest == "/" ? TheTree.Root().begin() : TheTree.Find(dest);

			if (srcIter == TheTree.NotFound()) {
				srcIter = TheTree.Find(CurrentDirectory + "/" + src);
			} else {
			}
			if (destIter == TheTree.NotFound()) {
				destIter = TheTree.Find(CurrentDirectory + "/" + dest);
			} else {
			}
			if (destIter != TheTree.NotFound()) {
				destIter->Parent().AddChild(src);
				srcIter->Parent().RemoveChild(src);
			} else {
				srcIter->Rename(dest);
			}
		}
		else if (InputLine.substr(0,2) == "rm") {
			string toRemove = InputLine.substr(3);
			if (CurrentDirectory == "/") {
				TheTree.Root().RemoveChild(toRemove);
			} else {
				auto entry = TheTree.Find(CurrentDirectory);
				if (entry != TheTree.NotFound()) {
					entry->RemoveChild(toRemove);
				}
			}
		}
	}
	return EXIT_SUCCESS;
}

void DFS(const std::string &path, CFileSystemTree::CEntry &entry){
    std::vector< std::tuple< std::string, bool> > Entries;
    std::string PathWithSlash = path;
    if(PathWithSlash.back() != '/'){
        PathWithSlash += "/";
    }
    DirectoryListing::GetListing(path, Entries);
    for(auto &Entry : Entries){
        if(std::get<1>(Entry)){
            std::string DirName = std::get<0>(Entry);
            if((DirName != ".")and(DirName != "..")){
                entry.AddChild(DirName);
                DFS(PathWithSlash + DirName, *entry.Find(DirName));
            }
        }
        else{
            entry.AddChild(std::get<0>(Entry));
        }
    }
}
