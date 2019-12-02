#ifndef GUIPANEL_HPP
#define GUIPANEL_HPP

#include "../SoupSFML.hpp"
#include "../Jackstd.hpp"
#include "../Resources.hpp"

class GuiPanel {
	Recti rec;
	int tileSize;
	sf::Vertex verts[3][3][4];
	sf::Texture *texture;
public:

	GuiPanel();
	void setTexture(sf::Texture &tex);
	void setColour(Colour c);
	
	void drawOn(sf::RenderTarget &target, sf::RenderStates states=sf::RenderStates::Default) const;
	void setRect(Recti r);
	Recti rect() const;
};

#endif
