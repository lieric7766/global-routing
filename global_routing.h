#include <vector>
#include <utility>
#include <fstream>
#include <string>

#ifndef GLOBAL_ROUTING_H
#define GLOBAL_ROUTING_H

using std::vector;

struct tile {
	int distance;
	int tile_row, tile_column;
	int loop_num;
	int fromfirst, fromsecond;
};

struct mpair {
	int x1, y1, x2, y2;
};

class global_routing
{
	public:
		void setAllGrid(int, int);
		void addline(int, int, int, int, int);
		void display();
		void setCapacity(int mcapacity);
		void setNetNum(int netnum);
		void setOutFile(const char*);
		void start();
		global_routing();
	private:
		vector<vector<tile> > grid;
		vector<vector<int> > lines;
		vector<vector<int> > edge_vertical;
		vector<vector<int> > edge_horiz;
		vector<tile*> Q;
		int capacity;
		int netNum, loopNum;
		int row, column;
		std::fstream fout;
		void dijkstra_shortest_path(int);
		void initialize_source(int);
		tile* extract_min();
		static bool compareDistance(tile*, tile*);
		void relax(tile* u, tile* v, int);
		void tracePath(int);
		int edge(int, int, int, int, int);
		void initialize_edge();
		void edgeRefresh(int);
		void traceEdge(int, int, int, int, int);
};

#endif 
