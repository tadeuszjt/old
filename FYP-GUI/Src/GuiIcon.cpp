#include "../Gui/GuiIcon.hpp"

GuiIcon::GuiIcon() {
	texture = nullptr;
}

void
GuiIcon::setTexture(sf::Texture &tex) {
	texture = &tex;
	tw = tex.getSize().x;
	th = tex.getSize().y;
	sfQuadTexcoords(verts, Rectf(tw, th));
	setRect(rec);
}

void
GuiIcon::setColour(Colour c) {
	sfQuadColour(verts, c);
}

void
GuiIcon::drawOn(sf::RenderTarget &target, sf::RenderStates states) const {
	states.texture = texture;
	target.draw(verts, 4, sf::Quads, states);
}

void
GuiIcon::setRect(Recti r) {
	rec = r;
	sfQuadPositions(
		verts,
		Rectf(
			tw, th,
			rec.x + rec.w/2 - tw/2, // xpos
			rec.y + rec.h/2 - th/2  // ypos
		)
	);
}

Recti
GuiIcon::rect() const {
	return rec;
}
