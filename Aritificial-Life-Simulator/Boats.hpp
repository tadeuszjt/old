#ifndef DRIVERS_HPP
#define DRIVERS_HPP

#include <functional>

#include "Soup/SoupSFML.hpp"
#include "Soup/Jackstd.hpp"
#include "Soup/Graphics.hpp"
#include "Soup/Grid.hpp"

#include "NeuralNet.hpp"


class Boater {
public:
	Grouper<Boater*> mode;

	NeuralNet net;
	
	bool selected = false;
	bool held = false;
	bool die = false;

	Iconer  iconOut, iconBack;
	Gridder gridder;
	Vec2f   pos, dir;
	Vec2f   vel;
	Vec2f   steer;
	Colour  col;
	
	Boater();
	void setPos(Vec2f p, Vec2f d);
	void setCol(Colour c);
	void update();
};


class Boats {
	void threadUpdate(int i0, int i1);
public:
	bool pause = false;
	Icon boatOut, boatBack;
	Dtab<Boater*> list;
	Grid grid;
	
	int spawnOdds = 300;
	int birthOdds = 600;

	Vec2f mPos;
	bool held;
	Group<Boater*> selected;
	
	
	
	Boats();
	~Boats();
	void mouse(Vec2f pos, bool press, bool release);
	void draw(sf::RenderTarget &target);
	
	Boater *newBoater();
	void update();
};

#endif 
