#ifndef GUITEXT_HPP
#define GUITEXT_HPP

#include <cstdarg>
#include "../SoupSFML.hpp"
#include "../Jackstd.hpp"
#include "../Resources.hpp"

class GuiText {
	Recti rec;
	int alignment;
public:
	sf::Text text;
	
	GuiText();
	void printf(const char *fmt, ...);
	void alignLeft();
	void alignCentre();
	void alignRight();
	void fontSans();
	void fontSerif();
	void fontMono();
	void setColour(Colour c);
	
	void drawOn(sf::RenderTarget &target, sf::RenderStates states=sf::RenderStates::Default) const;
	void setRect(Recti r);
	Recti rect() const;
};

#endif
