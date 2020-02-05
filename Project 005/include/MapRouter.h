#ifndef MAPROUTER_H
#define MAPROUTER_H

#include <vector>
#include <istream>
#include <map>

class CMapRouter{
    public:
        using TNodeID = unsigned long;
        using TStopID = unsigned long;
        using TLocation = std::pair<double, double>;
        using TPathStep = std::pair<std::string, TNodeID>;
        
        static const TNodeID InvalidNodeID;
		static const double WalkingSpeed;
    private:
		using TNodeIndex = size_t;
		using WayID = int;
			
		struct Edge {
			double Distance;
			double Time;
			double SpeedLimit;
			bool IsBusEdge;
			TNodeIndex Destination;
		};
		struct Node {
			TNodeID NodeID;
			TLocation Location;
			std::vector< Edge > Edges;
		};
		std::map<TStopID, TNodeID> StopTranslation;
		std::map<std::string, std::vector<TStopID>> RouteTranslation;
		std::vector< Node > Nodes;
		std::map<TNodeID, TNodeIndex> NodeTranslation;
		std::map<WayID, std::vector<TNodeID>> Ways;
		std::vector< TNodeID > SortedNodeIDs;

		double Dijkstras(TNodeIndex src, TNodeIndex dest, std::vector<TNodeIndex> &path, int searchtype);
		double Dijkstras2(TNodeIndex src, TNodeIndex dest, std::vector<TPathStep> &path, int searchtype);

    public:
        CMapRouter();
        ~CMapRouter();
        
        static double HaversineDistance(double lat1, double lon1, double lat2, double lon2);
		static double HaversineDistance(TLocation Start, TLocation End);
        static double CalculateBearing(double lat1, double lon1,double lat2, double lon2);
		static double CalculateBearing(TLocation Location1, TLocation Location2);
        
        bool LoadMapAndRoutes(std::istream &osm, std::istream &stops, std::istream &routes);
        size_t NodeCount() const;
        TNodeID GetSortedNodeIDByIndex(size_t index) const;
        TLocation GetSortedNodeLocationByIndex(size_t index) const;
        TLocation GetNodeLocationByID(TNodeID nodeid) const;
        TNodeID GetNodeIDByStopID(TStopID stopid) const;
        size_t RouteCount() const;
        std::string GetSortedRouteNameByIndex(size_t index) const;
        bool GetRouteStopsByRouteName(const std::string &route, std::vector< TStopID > &stops);
        
        double FindShortestPath(TNodeID src, TNodeID dest, std::vector< TNodeID > &path);
        double FindFastestPath(TNodeID src, TNodeID dest, std::vector< TPathStep > &path);
        bool GetPathDescription(const std::vector< TPathStep > &path, std::vector< std::string > &desc) const;
};

#endif
