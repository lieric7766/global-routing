#include <iostream>
#include <vector>
#include <algorithm>
#include "global_routing.h"
#include <stack>
#include <fstream>

#define BOUND 9999

global_routing :: global_routing()
{}

void global_routing :: setAllGrid(int mrow, int mcolumn)
{
	row = mrow;
	column = mcolumn;

	grid.resize(row);
	for (int i = 0; i < row; ++i) {
		grid[i].resize(column);
	}

	for (int i = 0; i < row; ++i) {
		for (int j = 0; j < column; ++j) {
			grid[i].push_back(tile());
		}
	}

	for (int i = 0; i < row; ++i) {
		for (int j = 0; j < column; ++j) {
			grid[i][j].tile_row = i;
			grid[i][j].tile_column = j;
			grid[i][j].loop_num = 0;
		}
	}

	edge_vertical.resize(row-1);
	for (int i = 0; i < row-1; ++i) {
		edge_vertical[i].resize(column);
	}

	edge_horiz.resize(row);
	for (int i = 0; i < row; ++i) {
		edge_horiz[i].resize(column-1);
	}
}

void global_routing :: addline(int netID, int sx, int sy, int tx, int ty)
{
	vector<int> v;
	v.push_back(netID);
	v.push_back(sx);
	v.push_back(sy);
	v.push_back(tx);
	v.push_back(ty);
	lines.push_back(v);
}

void global_routing :: dijkstra_shortest_path(int id)
{
	edgeRefresh(id-1);
	initialize_source(id);
	while (!Q.empty()) {
		tile* u = extract_min();

		//up
		if (u->tile_row - 1 >= 0) {
			relax(&grid[u->tile_row][u->tile_column], &grid[u->tile_row-1][u->tile_column], 1);
		}
		//down
		if (u->tile_row + 1 < row) {
			relax(&grid[u->tile_row][u->tile_column], &grid[u->tile_row+1][u->tile_column], 1);
		}
		//left
		if (u->tile_column - 1 >= 0) {
			relax(&grid[u->tile_row][u->tile_column], &grid[u->tile_row][u->tile_column-1], 0);
		}
		//right
		if (u->tile_column + 1 < column) {
			relax(&grid[u->tile_row][u->tile_column], &grid[u->tile_row][u->tile_column+1], 0);	
		}
	}
}

void global_routing :: initialize_source(int id)
{
	int sx, sy;

	sx = lines[id][1];
	sy = lines[id][2];
	grid[sx][sy].distance = 0;
	grid[sx][sy].loop_num++;
	Q.push_back(&grid[sx][sy]);
}

void global_routing :: initialize_edge()
{
	for (int i = 0; i < row-1; ++i) {
		for (int j = 0; j < column; ++j) {
			edge_vertical[i][j] = 0;
		}
	}

	for (int i = 0; i < row; ++i) {
		for (int j = 0; j < column-1; ++j) {
			edge_horiz[i][j] = 0;
		}
	}
	
}

tile* global_routing :: extract_min()
{
	tile* minu;
	std::sort(Q.begin(), Q.end(), compareDistance);
	minu = Q.front();
	Q.erase(Q.begin());

	return minu;
}

bool global_routing :: compareDistance(tile* element1, tile* element2)
{
	return element1->distance < element2->distance;
}

void global_routing :: relax(tile* u, tile* v, int flg)
{
	int weight = edge(u->tile_row, u->tile_column, v->tile_row, v->tile_column, flg);

	if (v->loop_num != loopNum) {
		v->distance = BOUND;
		v->loop_num++;
		Q.push_back(v);
	}

	if (v->distance > u->distance + weight) {
		v->distance = u->distance + weight;
		v->fromfirst = u->tile_row;
		v->fromsecond = u->tile_column;
	}
}

int global_routing :: edge(int urow, int ucol, int vrow, int vcol, int flg)
{
	int weight, mrow, mcol;

	if (flg == 1) {
		if (urow < vrow) {
			mrow = urow;
			mcol = ucol;
		}
		else {
			mrow = vrow;
			mcol = vcol;
		}
		return edge_vertical[mrow][mcol];
	}
	else {
		if (ucol < vcol) {
			mrow = urow;
			mcol = ucol;
		}
		else {
			mrow = vrow;
			mcol = vcol;
		}
		return edge_horiz[mrow][mcol];
	}
}

void global_routing :: traceEdge(int x1, int y1, int x2, int y2, int flg)
{
	int mrow, mcol;
	if (flg == 0) {
		if (y1 < y2) {
			edge_horiz[x1][y1]++;
		}
		else {
			edge_horiz[x2][y2]++;
		}
	}
	else {
		if (x1 < x2) {
			edge_vertical[x1][y1]++;
		}
		else {
			edge_vertical[x2][y2]++;
		}
	}

}

void global_routing :: edgeRefresh(int lastid)
{
	if (lastid >= 0) {
		int tx = lines[lastid][3];
		int ty = lines[lastid][4];
		int curx, cury, curtmpx1, curtmpy1, curtmpx2, curtmpy2;
		curx = tx;
		cury = ty;
		while (1) {
			curtmpx1 = grid[curx][cury].tile_row;
			curtmpy1 = grid[curx][cury].tile_column;
			curtmpx2 = grid[curx][cury].fromfirst;
			curtmpy2 = grid[curx][cury].fromsecond;
			if (curtmpx1 - curtmpx2 == 0) {
				traceEdge(curtmpx1, curtmpy1, curtmpx2, curtmpy2, 0);
			}
			else {
				traceEdge(curtmpx1, curtmpy1, curtmpx2, curtmpy2, 1);
			}
			
			curx = curtmpx2;
			cury = curtmpy2;
			if (curx == lines[lastid][1] && cury == lines[lastid][2])
				break;
			
		}
	}
}

void global_routing :: tracePath(int id)
{

	mpair mpr;
	int tx = lines[id][3];
	int ty = lines[id][4];
	int curx, cury, curtmpx, curtmpy;
	int mcount = 0;
	curx = tx;
	cury = ty;
	std::stack<mpair> mstack;

	while (1) {
		mpr.x1 = grid[curx][cury].fromfirst;
		mpr.y1 = grid[curx][cury].fromsecond;
		mpr.x2 = grid[curx][cury].tile_row;
		mpr.y2 = grid[curx][cury].tile_column;
		
		mcount++;
		mstack.push(mpr);
	
		curtmpx = grid[curx][cury].fromfirst;
		curtmpy = grid[curx][cury].fromsecond;
		curx = curtmpx;
		cury = curtmpy;
		if (curx == lines[id][1] && cury == lines[id][2])
			break;
	}

	fout << id << " " << mcount << std::endl;
	while(!mstack.empty()) {
		fout << mstack.top().x1 << " " << mstack.top().y1 << " ";
		fout << mstack.top().x2 << " " << mstack.top().y2 << std::endl;
		mstack.pop();
	}

}

void global_routing :: display()
{
	for (int i = 0; i < netNum; ++i) {
		for (int j = 0; j < 5; ++j) {
			std::cout << lines[i][j] << " ";
		}
		std::cout << std::endl;
	}

}

void global_routing :: setCapacity(int mcapacity)
{
	capacity = mcapacity;
}

void global_routing :: setNetNum(int netnum)
{
	netNum = netnum;
}

void global_routing :: setOutFile(const char* filename)
{
	// open the output file
	fout.open(filename, std::fstream::out);
	if(!fout.is_open())
	{
	std::cout << "Error: the output file is not opened!!" << std::endl;
	    exit(1);
	}
	
}

void global_routing :: start()
{
	loopNum = 1;
	initialize_edge();
	for (int i = 0; i < netNum; ++i) {
		dijkstra_shortest_path(i);
		tracePath(i);
		loopNum++;
	}

}
