#include "CSVReader.h"
#include "CSVWriter.h"
#include "XMLReader.h"
#include "XMLWriter.h"
#include "StringUtils.h"
#include "MapRouter.h"
#include <iostream>
#include <istream>
#include <sstream>
#include <vector>
#include <map>
#include <iomanip>
#include <fstream>

using namespace std;
// This function is only for testing
string timeFormat(double hours) {
	double seconds = hours * 60 * 60;
	int m = 0;
	int h = 0;
	while (seconds >= 60) {
		++m;
		seconds -= 60;
		while (m>= 60) {
			++h;
			m -= 60;
		}
	}
	string retString;
	if (h) {
		retString += to_string(h) + "hr ";
	}
	if (m) {
		retString += to_string(m) + "min ";
	}
	retString += to_string(seconds) + "sec";
	return retString;
}
int main(int argc, char *argv[]) {
	CMapRouter MapRouter;
	ifstream OSMFile ("data/davis.osm");
	string line;
	if (OSMFile.is_open()) {
	/* 	while ( getline(OSMFile, line) ) { */
	/* 		cout << line << endl; */
	/* 	} */
		cout << "Opened OSM File" << endl;
	}
	ifstream RoutesFile("data/routes.csv");
	if (RoutesFile.is_open()) {
		cout << "Opened Routes File" << endl;
	}
	ifstream StopsFile("data/stops.csv");
	if (StopsFile.is_open()) {
		cout << "Opened Stops File" << endl;
	}
	MapRouter.LoadMapAndRoutes(OSMFile, StopsFile, RoutesFile);
    std::vector< CMapRouter::TPathStep > Path;
	vector< string > Description;
	bool Done = false;
	string Command;
	string HelpString = 
	"findroute [--data=path | --resutls=path]\n"
	"------------------------------------------------------------------------\n"
	"help\t\tDisplay this help menu\n"
	"exit\t\tExit the program\n"
	"count\t\tOutput the number of nodes in the map\n"
	"node\t\tSyntax \"node [0, count)\"\n" 
	"\t\tWill output node ID and Lat/Lon for node\n"
	"fastest\t\tSyntax \"fastest start end\"\n" 
	"\t\tCalculates the time for fastest path from start to end\n"
	"shortest\tSyntax \"shortest start end\"\n" 
	"\t\tCalculates the distance for the shortest path from start to end\n"
	"save\t\tSaves the last calculated path to file\n"
	"print\t\tPrints the steps for the last calculated path\n";
	vector< CMapRouter::TPathStep > StoredPath;
	vector< CMapRouter::TNodeID > StoredPathId;
	unsigned long NodeID1;
	unsigned long NodeID2;
	double Result;
	while (!Done) {
		cout << "> ";
		getline(std::cin, Command);
		if (Command == "exit") {
			Done = true;
		} else if (Command == "help") {
			cout << HelpString << endl;
		} else if (Command.substr(0,4) == "node") {
			std::vector<string> V = StringUtils::Split(Command, " ");
			unsigned long idx = stoul(V[1]);
			CMapRouter::TNodeID CurrentNodeID = MapRouter.GetSortedNodeIDByIndex(idx) ;
			CMapRouter::TLocation CurrentNodeLocation = MapRouter.GetSortedNodeLocationByIndex(idx);
			double Lon = std::get<0>(CurrentNodeLocation);
			double Lat = std::get<1>(CurrentNodeLocation);
			cout << "Node " << idx << ": id = " << CurrentNodeID << " is at " << Lon << ", " << Lat << endl; 
			/* CMapRouter::TNodeID CurrentNodeId= stoul(Command.substr(4)); */
			/* cout << CurrentNodeId << endl; */
		} else if (Command.substr(0,7) == "fastest") {
			vector<string> V = StringUtils::Split(Command, " ");
			NodeID1 = stoul(V[1]);
			NodeID2 = stoul(V[2]);
			Result = MapRouter.FindFastestPath(NodeID1, NodeID2, StoredPath);
			cout << "Fastest Path takes " << timeFormat(Result) << endl;
		} else if (Command.substr(0,8) == "shortest") {
			vector<string> V = StringUtils::Split(Command, " ");
			NodeID1 = stoul(V[1]);
			NodeID2 = stoul(V[2]);
			Result = MapRouter.FindShortestPath(NodeID1, NodeID2, StoredPathId);
			cout << "Shortest path is " << setprecision(2) << Result << "mi" << endl;
		} else if (Command == "save") {
			string OutputFileName = (to_string(NodeID1)+ "_" + to_string(NodeID2) + "_" + to_string(Result) + "mi.csv");
			ofstream OutputFile (OutputFileName);
			CCSVWriter CSVWriter(OutputFile);
			vector<string> CVector;
			CVector.push_back("Write CSV Here");
			CSVWriter.WriteRow(CVector);
			cout << "saved to " << OutputFileName << endl;
		} else if (Command == "print") {
			cout << "Size :" << StoredPath.size() << endl;
			MapRouter.GetPathDescription(StoredPath, Description);
			for (auto &d : Description) {
				cout << d << endl;
			}
		} else {
			cout << "Invalid Command" << endl;
		}
	}
	return 0;
}
