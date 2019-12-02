#include "../Gui/GuiButton.hpp"


GuiButton::GuiButton() {
	held = false;
	hovered = false;
	clicked = false;
	hoveredPanel.setTexture(Resources_Panel4());
	pressedPanel.setTexture(Resources_Panel4());
	releasedPanel.setTexture(Resources_Panel3());
	pressedPanel.setColour(Colour(240, 240, 240));
}


void
GuiButton::mouse(Vec2i pos, bool pressed, bool released) {
	if (pressed) {
		if (pos.inRect(rec))
			held = true;
	}
	else if (released) {
		if (held && pos.inRect(rec))
			clicked = true;
			
		held = false;
	}
	hovered = pos.inRect(rec);
}


void
GuiButton::drawOn(sf::RenderTarget &target, sf::RenderStates states) const {
	if      (held)    pressedPanel.drawOn(target, states);
	else if (hovered) hoveredPanel.drawOn(target, states);
	else              releasedPanel.drawOn(target, states);
}


void
GuiButton::setRect(Recti r) {
	rec = r;
	hoveredPanel.setRect(rec);
	pressedPanel.setRect(rec);
	releasedPanel.setRect(rec);
	
}


Recti
GuiButton::rect() const {
	return rec;
}
