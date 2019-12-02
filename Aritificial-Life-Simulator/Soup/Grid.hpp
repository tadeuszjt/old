#ifndef GRID_HPP
#define GRID_HPP

#include <cfloat>
#include "Jackstd.hpp"

class Gridder;
class Grid {
	/* dimensions */
	Rectf region;
	float range;
	int   rows;
	int   cols;
	int   numTiles;
	float rowsDivH;
	float colsDivW;
public:
	
	/* data structure */
	struct Tile {
		Group<void*> group;
		Tile  *area[10];
		Rectf rec;
	} *tiles;

	Tile *tileAt(Vec2f p);
	Grid(Rectf Region, float Range);
	bool inRange(Vec2f pos, float dist, void **ptrloc = nullptr);
	bool nearest(Vec2f pos, void **ptrloc = nullptr, float *distloc = nullptr);
	Rectf rect() const;
	
};


class Gridder {
public:
	Vec2f pos;
	Grid *grid;
	Grid::Tile *tile;
	Grouper<void*> grouper;

	/* grid reader */
	Grid::Tile *read;
	int areai, readi;

	Gridder(void *Ptr);
	void setGrid(Grid &_grid);
	void setPos(Vec2f p);
	void resetLocals();
	bool pollLocals(void **ptrloc);
};

#endif
