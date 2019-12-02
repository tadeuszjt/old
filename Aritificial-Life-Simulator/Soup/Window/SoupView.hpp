#ifndef SOUPVIEW_HPP
#define SOUPVIEW_HPP

#include <cmath>
#include "../SoupSFML.hpp"

class SoupView : public sf::View {
	sf::Vector2f viewsize;
	sf::Vector2f halfsize;
	sf::Vector2f mouse;
	float targetzoom = 0.0;
	float currentzoom = 0.0;
	float currentscale = 1.0;
	
public:
	void update();	
	void resize(sf::Vector2u size);
	void zoom(float delta, sf::Vector2i mousepos);
	void setZoom(float scale, sf::Vector2f centre);
};

#endif
