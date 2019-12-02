#include "../Grid.hpp"

Grid::Tile*
Grid::tileAt(Vec2f p) {
	int r = (p.y-region.y) * rowsDivH;
	int c = (p.x-region.x) * colsDivW;
	if (r<0) r=0; else if (r>=rows) r=rows-1;
	if (c<0) c=0; else if (c>=cols) c=cols-1;
	return &tiles[r*cols + c];
}


Grid::Grid(Rectf Region, float Range) {
	// calculate dimensions
	region = Region;
	range  = Range;
	rows   = region.h / range; if (rows<1) rows=1;
	cols   = region.w / range; if (cols<1) cols=1;
	numTiles = rows * cols;
	rowsDivH = rows / region.h;
	colsDivW = cols / region.w;
	
	// build data structure
	tiles = new Tile [numTiles];
	for (int r=0; r<rows; r++) {
		for (int c=0; c<cols; c++) {
			Tile *tp = &tiles[r*cols + c];
			
			// calculate rec 
			tp->rec.w = region.w / cols;
			tp->rec.h = region.h / rows;
			tp->rec.x = (region.w*c) / cols;
			tp->rec.y = (region.h*r) / rows;
	
			// calculate area tiles
			const int loc[9][2] = {
				{r  , c  }, //centre
				{r-1, c  }, //top
				{r+1, c  }, //bottom
				{r  , c-1}, //left
				{r  , c+1}, //right
				{r-1, c-1}, //top left
				{r-1, c+1}, //top right
				{r+1, c-1}, //bottom left
				{r+1, c+1}  //bottom right
			};

			
			int i=0;
			for (int a=0; a<9; a++) {
				const int ar = loc[a][0];
				const int ac = loc[a][1];
				if (
					ar <  0    ||
					ar >= rows ||
					ac <  0    || 
					ac >= cols
				) continue; 
								
				tp->area[i++] = &tiles[ar*cols + ac];
			}
			
			while (i < 10)
				tp->area[i++] = nullptr; // null terminated
		}
	}
}


bool
Grid::inRange(Vec2f pos, float dist, void **ptrloc) {
	const float dist2 = dist*dist;
	Tile *tp, **app   = tileAt(pos)->area;
	
	while ((tp = *app++) != nullptr) {
		for (int i=0; i<tp->group.count; i++) {
			Gridder *gridder = (Gridder*)tp->group.gpers[i]->owner;
			
			Vec2f vec  = gridder->pos - pos;
			float len2 = vec.len2();
			
			if (len2 < dist2) {
				if (ptrloc)
					*ptrloc = gridder->grouper.itemcpy;
				return true;;
			}
		}
	}
	
	return false;
}

bool
Grid::nearest(Vec2f pos, void **ptrloc, float *distloc) {
	float dist2 = FLT_MAX;
	bool  found = false;
	void *ptr;
	Tile *tp, **app = tileAt(pos)->area;
	
	while ((tp = *app++) != nullptr) {
		for (int i=0; i<tp->group.count; i++) {
			Gridder *gridder = (Gridder*)tp->group.gpers[i]->owner;
			
			Vec2f vec  = gridder->pos - pos;
			float len2 = vec.len2();
			
			if (len2 < dist2) {
				dist2 = len2;
				found = true;
				ptr = gridder->grouper.itemcpy;
			}
		}
	}
	if (found) {
		if (ptrloc)  *ptrloc = ptr;
		if (distloc) *distloc = sqrtf(dist2);
		return true;
	}
	return false;
}

Rectf
Grid::rect() const {
	return region;
}






Gridder::Gridder(void *Ptr) :
grouper(Ptr) {
	grouper.owner = this;
	grid = nullptr;
	tile = nullptr;
	read = nullptr;
}

void
Gridder::setGrid(Grid &_grid) {
	grid = &_grid;
	tile = grid->tileAt(pos);
	grouper.setGroup(tile->group);
	read = nullptr;
}

void
Gridder::setPos(Vec2f p) {
	pos = p;
	if (grid) {
		tile = grid->tileAt(p);
		grouper.setGroup(tile->group);
	}
}

void
Gridder::resetLocals() {
	areai = 0;
	readi = 0;
	if (tile)
		read = tile->area[areai++];
}

bool
Gridder::pollLocals(void **ptrloc) {
	if (read) {
		for (;;) {
			if (readi < read->group.count) {
				*ptrloc = read->group.items[readi++];
				return true;
			}
			
			if ((read = tile->area[areai++]) == nullptr) break;
			else readi = 0;
		}
	}
	read = nullptr;
	return false;
}
