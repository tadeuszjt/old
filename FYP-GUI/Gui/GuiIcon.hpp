#ifndef GUIICON_HPP
#define GUIICON_HPP

#include "../SoupSFML.hpp"
#include "../Jackstd.hpp"

class GuiIcon {
	Recti rec;
	int tw, th;
	sf::Vertex verts[4];
	sf::Texture *texture;
public:

	GuiIcon();
	void setTexture(sf::Texture &tex);
	void setColour(Colour c);
	
	void drawOn(sf::RenderTarget &target, sf::RenderStates states=sf::RenderStates::Default) const;
	void setRect(Recti r);
	Recti rect() const;
};

#endif
