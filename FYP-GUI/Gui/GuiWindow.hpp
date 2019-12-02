#ifndef GUIWINDOW_HPP
#define GUIWINDOW_HPP

#include "../SoupSFML.hpp"
#include "../Jackstd.hpp"
#include "GuiButton.hpp"
#include "GuiMargin.hpp"
#include "GuiPanel.hpp"
#include "GuiText.hpp"

class GuiWindow {
	sf::Vertex barVerts[4];
	Vec2i mpos;
	Recti rec;
public:
	GuiMargin margin;
	GuiText   title;
	GuiButton bar;
	GuiPanel  panel;
	GuiButton resize;
	
	GuiWindow();

	void mouse(Vec2i pos, bool pressed, bool released);
	void drawOn(sf::RenderTarget &target, sf::RenderStates states=sf::RenderStates::Default) const;
	void setRect(Recti r);
	Recti rect() const;
};


#endif
