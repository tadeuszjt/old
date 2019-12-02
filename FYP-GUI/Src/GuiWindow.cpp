#include "../Gui/GuiWindow.hpp"

	
GuiWindow::GuiWindow() {
	margin.setMargins(20, 20, 20, 20);
	bar.releasedPanel.setTexture(Resources_Panel1());
	bar.hoveredPanel.setTexture(Resources_Panel1());
	bar.pressedPanel.setTexture(Resources_Panel1());
	bar.releasedPanel.setColour(Colour(255, 255, 255, 160));
	bar.hoveredPanel.setColour(Colour(255, 255, 255, 160));
	bar.pressedPanel.setColour(Colour(255, 255, 255, 160));
	
	title.text.setFillColor(sf::Color::White);
	title.text.setOutlineColor(sf::Color::Black);
	title.text.setOutlineThickness(1.6);
	title.text.setStyle(sf::Text::Bold);
	title.alignCentre();
}

void
GuiWindow::mouse(Vec2i pos, bool press, bool release) {
	resize.mouse(pos, press, release);
	bar.mouse(pos, press, release);
	
	if (resize.held) { // resize
		int mx = pos.x - mpos.x;
		int my = pos.y - mpos.y;
		setRect(Recti(rec.w+mx, rec.h+my, rec.x, rec.y));
	}
	
	else if (bar.held) { // move 
		int mx = pos.x - mpos.x;
		int my = pos.y - mpos.y;
		setRect(Recti(rec.w, rec.h, rec.x+mx, rec.y+my));
	}
	mpos = pos;
}

void
GuiWindow::drawOn(sf::RenderTarget &target, sf::RenderStates states) const {
	panel.drawOn(target, states);
	resize.drawOn(target, states);
	bar.drawOn(target, states);
	
	states.texture = &Resources_TitleBar();
	target.draw(barVerts, 4, sf::Quads, states);
	
	title.drawOn(target, states);
}

void
GuiWindow::setRect(Recti r) {
	rec = r;
	margin.setRect(rec);
	panel.setRect(rec);
	resize.setRect(margin.rect(2, 2));
	
	Recti brec(r.w, margin.rect(0, 1).h, r.x, r.y);
	
	const int b = 0.16*brec.h;
	title.setRect(Recti(brec.w, brec.h-2*b, brec.x, brec.y+b));
	
	bar.setRect(brec);
	sfQuadPositions(barVerts, brec);
	sfQuadTexcoords(barVerts, Recti(brec.w, brec.h, r.x/8, r.y/8));
}

Recti
GuiWindow::rect() const {
	return rec;
}
