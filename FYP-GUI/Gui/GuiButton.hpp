#ifndef GUIBUTTON_HPP
#define GUIBUTTON_HPP

#include "../SoupSFML.hpp"
#include "../Jackstd.hpp"
#include "../Resources.hpp"
#include "GuiPanel.hpp"

class GuiButton {
	Recti rec;
public:
	bool held;
	bool hovered;
	bool clicked;
	GuiPanel hoveredPanel;
	GuiPanel pressedPanel;
	GuiPanel releasedPanel;
	
	GuiButton();

	void mouse(Vec2i pos, bool pressed, bool released);
	void drawOn(sf::RenderTarget &target, sf::RenderStates states = sf::RenderStates::Default) const;
	void setRect(Recti r);
	Recti rect() const;
};

#endif
