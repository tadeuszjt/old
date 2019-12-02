#include "../Gui/GuiPanel.hpp"

GuiPanel::GuiPanel() {
	setTexture(Resources_Panel1());
}

void
GuiPanel::setTexture(sf::Texture &tex) {
	texture = &tex;
	tileSize = tex.getSize().x / 3;
	for (int r=0; r<3; r++)
		for (int c=0; c<3; c++) 
			sfQuadTexcoords(
				verts[r][c],
				Rectf(tileSize, tileSize, c*tileSize, r*tileSize)
			);
	
	setRect(rec);
}

void
GuiPanel::setColour(Colour c) {
	sf::Color col(c.r, c.g, c.b, c.a);
	sf::Vertex *vertarr = (sf::Vertex*)verts;
	for (int i=0; i<3*3*4; i++)
		vertarr[i].color = col;
}


void 
GuiPanel::drawOn(sf::RenderTarget &target, sf::RenderStates states) const {
	states.texture = texture;
	target.draw((sf::Vertex*)verts, 3*3*4, sf::Quads, states);
}

void 
GuiPanel::setRect(Recti r) {
	rec = r;
	
	// set verts
	const int p = tileSize / 2;
	const int x[4] = {r.x-p, r.x+p, r.xopp()-p, r.xopp()+p};
	const int y[4] = {r.y-p, r.y+p, r.yopp()-p, r.yopp()+p};
	for (int r=0; r<3; r++) {
		for (int c=0; c<3; c++) {
			verts[r][c][0].position = sf::Vector2f(x[c],   y[r]);
			verts[r][c][1].position = sf::Vector2f(x[c+1], y[r]);
			verts[r][c][2].position = sf::Vector2f(x[c+1], y[r+1]);
			verts[r][c][3].position = sf::Vector2f(x[c],   y[r+1]);
		}
	}
}

Recti 
GuiPanel::rect() const {
	return rec;
}
