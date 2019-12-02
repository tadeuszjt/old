#ifndef ICON_HPP
#define ICON_HPP

#include <cmath>
#include "../SoupSFML.hpp"
#include "../Jackstd.hpp"

class Iconer;
class Icon {
public:
	sf::Texture texture;
	Dtab<Iconer*> icons;
	Dtab<sf::Vertex> verts;
	
	Icon();
	Icon(const char filename[]);
	~Icon();
	void loadFile(const char filename[]);
	void draw(sf::RenderTarget &target, sf::RenderStates states=sf::RenderStates::Default) const;
};


class Iconer {
	int    index;
public:
	Icon  *icon;
	Rectf  rec;
	Rectf  iconRec;
	Vec2f  pos;
	Vec2f  mag;
	Colour col;

	Iconer();
	~Iconer();
	
	void setIcon(Icon &ic);
	void unSetIcon();
	
	void setIconRect(Rectf r);
	void setRect(Rectf r);
	void setCol(Colour c);
	
	void setPos(Vec2f p);
	void setPos(Vec2f p, Vec2f mag);
};


#endif
