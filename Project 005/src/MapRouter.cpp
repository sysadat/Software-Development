#include "MapRouter.h"
#include "XMLReader.h"
#include "CSVReader.h"
#include <algorithm>
#include <cmath>
#include <queue>
#include <stack>
#include <iostream>
#include <sstream>

using namespace std;
// Helper functions for debugging
template<typename T>
void printVector(std::vector<T> &v) {
	for (auto &s : v) {
		cout << s << endl;
	}
}
// Helper functions for debugging
template<typename T, typename Y>
void printMap(std::map<T, Y> &m) {
	for (auto it = m.begin(); it != m.end(); ++it) {
		for (auto &s : it->second) {
			cout << it->first << " " << s << endl;
		}
	}
}
const CMapRouter::TNodeID CMapRouter::InvalidNodeID = -1;
const double CMapRouter::WalkingSpeed = 3.0; 

CMapRouter::CMapRouter(){
}

CMapRouter::~CMapRouter(){

}

double CMapRouter::Dijkstras2(TNodeIndex src, TNodeIndex dest, std::vector< TPathStep > &path, int searchtype) {
	std::vector< TNodeIndex > Previous(Nodes.size());
	std::vector< double > Distance(Nodes.size(), std::numeric_limits<double>::max());
	std::vector< TNodeIndex > Heap;
	std::vector< bool > Bus(Nodes.size(), false);
	bool onABus = false;
	auto Compare = [&Distance](TNodeIndex idx1, TNodeIndex idx2){ return Distance[idx1] < Distance[idx2]; };
	/* cout << "Src index: " << src << endl; */
	Previous[src] = src;
	Distance[src] = 0.0;
	Heap.push_back(src);
	while (!Heap.empty()) {
		std::make_heap(Heap.begin(), Heap.end(), Compare);
		std::pop_heap(Heap.begin(), Heap.end(), Compare);
		auto Current = Heap.back();
		/* cout << "Current : "  << Current << endl; */
		/* cout << "Currrent Node ID : " << NodeTranslation[Current] << endl; */
		Heap.pop_back();
		/* cout << "Size of edges: " << Nodes[Current].Edges.size() << endl; */
		for (auto &Edge : Nodes[Current].Edges) {
			// Depends on the searchtype
			double EdgeDistance;
			switch(searchtype) {
				case 0:
					EdgeDistance = Edge.Distance;
					break;
				case 1:
					EdgeDistance = Edge.Time;
					break;
				default:
					EdgeDistance = Edge.Distance / Edge.SpeedLimit;
					break;
			}
			auto AltDistance = Distance[Current] + EdgeDistance;
			if (AltDistance < Distance[Edge.Destination]) {
				/* cout << "Destination : " << Edge.Destination << endl; */
				/* cout << "Destination ID : " << NodeTranslation[Edge.Destination] << endl; */
				if (Distance[Edge.Destination] == std::numeric_limits<double>::max()){
					Heap.push_back(Edge.Destination);
				}
				Distance[Edge.Destination] = AltDistance;
				Previous[Edge.Destination] = Current;
				Bus[Edge.Destination] = Edge.IsBusEdge;
			}
		}
	}

	if (Distance[dest] == std::numeric_limits<double>::max()){

	} else {
		auto CurrentPrevious = dest;
		bool firstBus = false;
		while (CurrentPrevious != src) {
			string Way = "Walk";
			if (Bus[CurrentPrevious]) {
				Distance[dest] += firstBus ?  double(30.0 / 3600.0) : 0;
				firstBus = true;
				path.push_back(make_pair("Bus A", SortedNodeIDs[CurrentPrevious]));
			} else {
				path.push_back(make_pair("Walk",SortedNodeIDs[CurrentPrevious]));
			}
			CurrentPrevious = Previous[CurrentPrevious];
		}
		if (Bus[CurrentPrevious]) {
			Distance[dest] += firstBus ?  double(30.0 / 3600.0) : 0;
			path.push_back(make_pair("Bus A", SortedNodeIDs[CurrentPrevious]));
		} else {
			path.push_back(make_pair("Walk",SortedNodeIDs[CurrentPrevious]));
		}
	}
	/* cout << "Dest : " << dest << endl; */
	/* cout << "Distance Vector: " << endl; */
	/* printVector(Distance); */
	/* cout << "Previous: " <<  endl; */
	/* printVector(Previous); */
	return Distance[dest];
}

double CMapRouter::Dijkstras(TNodeIndex src, TNodeIndex dest, std::vector< TNodeIndex > &path, int searchtype) {
	std::vector< TNodeIndex > Previous(Nodes.size());
	std::vector< double > Distance(Nodes.size(), std::numeric_limits<double>::max());
	std::vector< TNodeIndex > Heap;
	auto Compare = [&Distance](TNodeIndex idx1, TNodeIndex idx2){ return Distance[idx1] < Distance[idx2]; };
	/* cout << "Src index: " << src << endl; */
	Previous[src] = src;
	Distance[src] = 0.0;
	Heap.push_back(src);
	while (!Heap.empty()) {
		std::make_heap(Heap.begin(), Heap.end(), Compare);
		std::pop_heap(Heap.begin(), Heap.end(), Compare);
		auto Current = Heap.back();
		/* cout << "Current : "  << Current << endl; */
		/* cout << "Currrent Node ID : " << NodeTranslation[Current] << endl; */
		Heap.pop_back();

		/* cout << "Size of edges: " << Nodes[Current].Edges.size() << endl; */
		for (auto &Edge : Nodes[Current].Edges) {
			// Depends on the searchtype
			double EdgeDistance;
			switch(searchtype) {
				case 0:
					EdgeDistance = Edge.Distance;
					break;
				case 1:
					EdgeDistance = Edge.Time;
					break;
				default:
					EdgeDistance = Edge.Distance / Edge.SpeedLimit;
					break;
			}
			auto AltDistance = Distance[Current] + EdgeDistance;
			if (AltDistance < Distance[Edge.Destination]) {
				/* cout << "Destination : " << Edge.Destination << endl; */
				/* cout << "Destination ID : " << NodeTranslation[Edge.Destination] << endl; */
				if (Distance[Edge.Destination] == std::numeric_limits<double>::max()){
					Heap.push_back(Edge.Destination);
				}
				Distance[Edge.Destination] = AltDistance;
				Previous[Edge.Destination] = Current;
			}
		}
	}
	if (Distance[dest] == std::numeric_limits<double>::max()){

	} else {
		auto CurrentPrevious = dest;
		while (CurrentPrevious != src) {
			path.push_back(SortedNodeIDs[CurrentPrevious]);
			CurrentPrevious = Previous[CurrentPrevious];
		}
		path.push_back(SortedNodeIDs[CurrentPrevious]);
		//Complete later
	}
	/* cout << "Dest : " << dest << endl; */
	/* cout << "Distance Vector: " << endl; */
	/* cout << "Previous: " <<  endl; */
	return Distance[dest];
}

double CMapRouter::HaversineDistance(TLocation start ,TLocation end) {
	return HaversineDistance(get<0>( start ), get<1>(start), get<0>(end), get<1>(end));
}

double CMapRouter::HaversineDistance(double lat1, double lon1, double lat2, double lon2){
	auto DegreesToRadians = [](double deg){return M_PI * (deg) / 180.0;};
	double LatRad1 = DegreesToRadians(lat1);
	double LatRad2 = DegreesToRadians(lat2);
	double LonRad1 = DegreesToRadians(lon1);
	double LonRad2 = DegreesToRadians(lon2);
	double DeltaLat = LatRad2 - LatRad1;
	double DeltaLon = LonRad2 - LonRad1;
	double DeltaLatSin = sin(DeltaLat/2);
	double DeltaLonSin = sin(DeltaLon/2);
	double Computation = asin(sqrt(DeltaLatSin * DeltaLatSin + cos(LatRad1) * cos(LatRad2) * DeltaLonSin * DeltaLonSin));
	const double EarthRadiusMiles = 3959.88;

	return 2 * EarthRadiusMiles * Computation;
}

double CMapRouter::CalculateBearing(TLocation Location1, TLocation Location2) {
	return CalculateBearing(std::get<0>(Location1), std::get<1>(Location1), std::get<0>(Location2), std::get<1>(Location2));
}

double CMapRouter::CalculateBearing(double lat1, double lon1,double lat2, double lon2){
	auto DegreesToRadians = [](double deg){return M_PI * (deg) / 180.0;};
	auto RadiansToDegrees = [](double rad){return 180.0 * (rad) / M_PI;};
	double LatRad1 = DegreesToRadians(lat1);
	double LatRad2 = DegreesToRadians(lat2);
	double LonRad1 = DegreesToRadians(lon1);
	double LonRad2 = DegreesToRadians(lon2);
	double X = cos(LatRad2)*sin(LonRad2-LonRad1);
	double Y = cos(LatRad1)*sin(LatRad2)-sin(LatRad1)*cos(LatRad2)*cos(LonRad2-LonRad1);
	return RadiansToDegrees(atan2(X,Y));
}



bool CMapRouter::LoadMapAndRoutes(std::istream &osm, std::istream &stops, std::istream &routes){
	CXMLReader OSMReader(osm);
	cout << "Getting OSM" << endl;
	CCSVReader StopsReader(stops);
	cout << "Getting Stops" << endl;
	CCSVReader RoutesReader(routes);
	cout << "Getting Routes" << endl;
	std::vector<std::string> TempRow;

	SXMLEntity TempEntity;

	/* Read the initial OSM tag */
	OSMReader.ReadEntity(TempEntity);

	/* long WayId; */
	long long LastRef = -1;
	while (!OSMReader.End()) {
		OSMReader.ReadEntity(TempEntity);
		if (true) {
			if (TempEntity.DNameData == "node") {
				TNodeID TempNodeID = std::stoul(TempEntity.AttributeValue("id"));
				double TempLat = std::stod(TempEntity.AttributeValue("lat"));
				double TempLong = std::stod(TempEntity.AttributeValue("lon"));
				Node TempNode;
				TempNode.NodeID = TempNodeID;
				TempNode.Location = std::make_pair(TempLat, TempLong);
				NodeTranslation[TempNodeID] = Nodes.size();
				Nodes.push_back(TempNode);
				SortedNodeIDs.push_back(TempNodeID);
			}
			else if (TempEntity.DNameData == "way") {
				double SpeedLimit = 25;
				bool OneWay = false;
				LastRef = -1;
				/* TempEdge.Time = TempEdge.Distance / 3.0 */
				/* WayId = stoi(TempEntity.AttributeValue("id")); */
			} else if (TempEntity.DNameData == "nd") {
				long long currentRef = stoll(TempEntity.AttributeValue("ref"));
				/* stringstream t(TempEntity.AttributeValue("ref")); */
				/* int currentRef; */
				/* t >> currentRef; */
				// Useless, just for testing
				/* Ways[WayId].push_back(currentRef); */
				if (LastRef > 0) {
					// Keeps track of the last reference
					TLocation LastRefLocation = Nodes[NodeTranslation[LastRef]].Location;
					TLocation CurrentRefLocation = Nodes[NodeTranslation[currentRef]].Location;
					double HDistance = HaversineDistance(LastRefLocation, CurrentRefLocation);
					Edge NewEdge;
					NewEdge.Distance = HDistance;
					NewEdge.Time = HDistance / WalkingSpeed;
					NewEdge.SpeedLimit = 25;
					NewEdge.IsBusEdge = false;
					// CurrentRef is the ID!
					// Stores Index as destination
					NewEdge.Destination = NodeTranslation[currentRef];
					/* cout << LastRef << " Connects to " << currentRef << endl; */
					Nodes[NodeTranslation[LastRef]].Edges.push_back(NewEdge);
					LastRef = currentRef;
				} else {
					LastRef = currentRef;
				}
			}
		}
	}
	// Route TempRoute;
	// StopIndex = 1
	// size_t RouteIndex = 0;
	// while ....
	// Row;
	// RouteReader.ReaderRow(Row)
	// if (Row.empty())
	// auto RouteName = Row[RouteIndex];
	// TStopID TempStopID = std::stoul(Row[StopIndex]);
	// TNodeID TempNodeID = StopToNodeTranslation(TempNodeID)->second;
	// TNodeIndex TempNodeIndex = StopToNodeTranslation(TempNodeID)->second;
	// if (TempRoute.Name != RouteName){
	// if (TempRoute.Name.empty()){
	// RouteTranslation[TempRoute.Name] = TempRoute;
	// SortedRouteNames.push_back(TempRoute);
	// }
	// TempRoute Name = RouteName;
	// TempRoute.StopIndices.clear();
	// TempRoute.StopIndices.push_back(TempNodeIndex);
	// }
	/* printMap(Ways); */
	while (!StopsReader.End()) {
		StopsReader.ReadRow(TempRow);
	}

	LastRef = -1;
	for (size_t i = 2 ; i < TempRow.size(); i += 2) {
		TStopID StopId = std::stoul(TempRow[i]);
		TNodeID NodeId = std::stoul(TempRow[i + 1]);
		StopTranslation[StopId] = NodeId;
		if (LastRef > 0) {
			// Keeps track of the last reference
			TLocation LastRefLocation = Nodes[NodeTranslation[LastRef]].Location;
			TLocation CurrentRefLocation = Nodes[NodeTranslation[NodeId]].Location;
			double HDistance = HaversineDistance(LastRefLocation, CurrentRefLocation);
			Edge NewEdge;
			NewEdge.Distance = HDistance;
			NewEdge.Time = HDistance / 25.0;
			NewEdge.SpeedLimit = 25;
			NewEdge.IsBusEdge = true;
			// CurrentRef is the ID!
			// Stores Index as destination
			NewEdge.Destination = NodeTranslation[NodeId];
			/* cout << LastRef << " Connects to " << currentRef << endl; */
			Nodes[NodeTranslation[LastRef]].Edges.push_back(NewEdge);
			LastRef = NodeId;
		} else {
			LastRef = NodeId;
		}
		/* Nodes[NodeTranslation[NodeId]].Edge.IsBusEdge = true; */
		/* Nodes[NodeTranslation[NodeId]].Edge.Time = Nodes[NodeTranslation[NodeId]].Edge.Distance / 25; */
	}

	TempRow.clear();

	while (!RoutesReader.End()) {
		RoutesReader.ReadRow(TempRow);
	}

	for (size_t i = 2 ; i < TempRow.size(); i += 2) {
		string Route = TempRow[i];
		TStopID StopId = std::stoul(TempRow[i + 1]);
		RouteTranslation[Route].push_back(StopId);
	}
	TempRow.clear();

	std::sort(SortedNodeIDs.begin(), SortedNodeIDs.end());
	// Sort the route names

	return true;
}

size_t CMapRouter::NodeCount() const{
	return SortedNodeIDs.size();
}

CMapRouter::TNodeID CMapRouter::GetSortedNodeIDByIndex(size_t index) const{
	return SortedNodeIDs[index];
}

CMapRouter::TLocation CMapRouter::GetSortedNodeLocationByIndex(size_t index) const{
	if (index >= NodeCount()) {
		auto Invalid = std::make_pair(180.0, 360.0);
		return std::make_pair(180.0, 360.0);
	}
	auto Search = NodeTranslation.find(SortedNodeIDs[index]);
	return Nodes[Search -> second].Location;
}

CMapRouter::TLocation CMapRouter::GetNodeLocationByID(TNodeID nodeid) const{
	/* return Nodes[NodeTranslation[nodeid]].Location; */
	return Nodes[nodeid].Location;
}

CMapRouter::TNodeID CMapRouter::GetNodeIDByStopID(TStopID stopid) const{
	auto Search = StopTranslation.find(stopid);
	return Search->second;
}

size_t CMapRouter::RouteCount() const{
	return RouteTranslation.size();
}

std::string CMapRouter::GetSortedRouteNameByIndex(size_t index) const{
	size_t i = 0;
	auto it = RouteTranslation.begin();
	if (RouteCount() == 1) {
		return it->first;
	}
	for (it ; i < index || it != RouteTranslation.end(); ++it, ++i) {

	}
	auto result = it->first;
	return result;
}

bool CMapRouter::GetRouteStopsByRouteName(const std::string &route, std::vector< TStopID > &stops){
	stops = RouteTranslation[route];
	return true;
}

// Use Dijstras multiple times

double CMapRouter::FindShortestPath(TNodeID src, TNodeID dest, std::vector< TNodeID > &path){
	// Your code HERE
	double Result = Dijkstras(NodeTranslation[src], NodeTranslation[dest], path, 0);
	std::reverse(path.begin(), path.end());
	return Result;
	/* return std::numeric_limits<double>::max(); */
}

double CMapRouter::FindFastestPath(TNodeID src, TNodeID dest, std::vector< TPathStep > &path){
	// Your code HERE
	double Result = Dijkstras2(NodeTranslation[src], NodeTranslation[dest], path, 1);
	std::vector< TPathStep > fin;
	while (!path.empty()) {
		fin.push_back(path.back());
		path.pop_back();
	}
	path = fin;
	return Result;
	/* return std::numeric_limits<double>::max(); */
}

std::pair<string, string> DMS(CMapRouter::TLocation Location) {
	double X = get<0>(Location);
	double Y = get<1>(Location);
	int dX = int(X);
	int dY = int(Y);
	int mX = int((X - dX) * 60);
	int mY = int((Y - dY) * 60);
	int sX = int(X - dX - (mX / 60) * 3600);
	int sY = int(Y - dY - (mY / 60) * 3600);
	string retString = "";
	retString += to_string(dX) + "d " + to_string(mX) + "' " + to_string(sX) + "\"";
	string retString2;
	retString2 += to_string(dY) + "d " + to_string(mY) + "' " + to_string(sY) + "\"";
	/* cout << retString2 << endl; */
	return make_pair(retString, retString2);
}

string GetBearingString(double CurrentBearing) {
	string BearingString;
	if (CurrentBearing >= -22.5 && CurrentBearing <= 22.5) {
		BearingString = "N";
	} else if (CurrentBearing <= 67.5) {
		BearingString = "NE";
	} else if (CurrentBearing <= 112.5) {
		BearingString = "E";
	} else if (CurrentBearing <= 157.5) {
		BearingString = "SE";
	}
	else if (CurrentBearing < -157.5) {
		BearingString = "S";
	}
	else if(CurrentBearing > -157.5 && CurrentBearing < -112.5) {
		BearingString = "SW";
	}
	else if (CurrentBearing > -112.5 && CurrentBearing < -67.5) {
		BearingString = "W";
	}
	else if ( CurrentBearing > -67.5 && CurrentBearing < -22.5) {
		BearingString = "NW";
	} else {
		BearingString = "W";
	}
	return BearingString;
}

void PrintLocation(CMapRouter::TLocation Location) {
	cout << get<0>(Location) << '\t' << get<1>(Location) << endl;
}

bool CMapRouter::GetPathDescription(const std::vector< TPathStep > &path, std::vector< std::string > &desc) const{
	if (!path.empty()) {
		TLocation StartLocation = GetNodeLocationByID(std::get<1>(path[0]));
		/* PrintLocation(StartLocation); */
		desc.push_back("Start at " + get<0>(DMS(StartLocation)) + " N, " + get<0>(DMS(StartLocation)) + " E");
		TLocation PreviousLocation = GetNodeLocationByID(std::get<1>(path[0]));
		stack<string> busStack;
		double CurrentBearing = 0;
		size_t isBus; 
		std::pair<string, string> DMSPair;
		for (size_t i = 1; i < path.size(); i++) {
			isBus = std::get<0>(path[i]).find("Bus");
			TLocation CurrentLocation = GetNodeLocationByID(std::get<1>(path[i]));
			/* PrintLocation(CurrentLocation); */
			double CurrentBearing = CalculateBearing(PreviousLocation, CurrentLocation);
			TLocation AltLocation = make_pair(abs(get<0>(PreviousLocation) - get<0>(CurrentLocation)), abs(get<1>(PreviousLocation) - get<1>(CurrentLocation)));
			bool onBus = false;
			DMSPair = DMS(AltLocation);
			string CombinedString;
			if (isBus == string::npos) {
				// Walk 
				if (busStack.empty()) {
					CombinedString = get<0>(path[i]) + string(" ") + GetBearingString(CurrentBearing) + " to " + get<0>(DMSPair)+ " N, " + get<1>(DMSPair) + " E";
					desc.push_back(CombinedString);
				} else {
					onBus = false;
					desc.push_back(busStack.top() + " and get off at stop 23" );
					CombinedString = get<0>(path[i]) + string(" ") + GetBearingString(CurrentBearing) + " to " + get<0>(DMSPair) + " N, "+ get<1>(DMSPair) + " E";
					desc.push_back(CombinedString);
					while (!busStack.empty())
						busStack.pop();
				}
			} else {
				busStack.push("Take " + std::get<0>(path[i]));
				onBus = true;
			}

			PreviousLocation = CurrentLocation;
		}
		desc.push_back("End at " + get<0>(DMSPair) + " N, "+ get<1>(DMSPair) + " E");
		cout << "Path size : " << desc.size() << endl;
		return true;
	}
	return false;
}
