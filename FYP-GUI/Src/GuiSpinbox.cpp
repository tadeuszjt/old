#include "../Gui/GuiSpinbox.hpp"

GuiSpinbox::GuiSpinbox() {
	panel.setTexture(Resources_Panel2());
	panel.setColour(Colour(180, 180, 180, 100));
	
	text.fontMono();
	
	min = INT_MIN;
	max = INT_MAX;
	mult = 0.1;
	setCount(0);
}

void
GuiSpinbox::setCount(int c) {
	if      (c < min) c = min;
	else if (c > max) c = max;
	count = c;
	text.printf("%g", count*mult);
}

void
GuiSpinbox::setCountMultiplier(float m) {
	mult = m;
}

void
GuiSpinbox::setCountRange(int _min, int _max) {
	min = _min;
	max = _max;
	setCount(count);
}

float
GuiSpinbox::value() const {
	return count * mult;
}


void
GuiSpinbox::mouse(Vec2i pos, bool pressed, bool released) {
	b1.mouse(pos, pressed, released);
	b2.mouse(pos, pressed, released);
	
	if (b1.clicked) {
		b1.clicked = false;
		setCount(count+1);
	}
	
	if (b2.clicked) {
		b2.clicked = false;
		setCount(count-1);
	}
}

void
GuiSpinbox::drawOn(sf::RenderTarget &target, sf::RenderStates states) const {
	panel.drawOn(target, states);
	text.drawOn(target, states);
	b1.drawOn(target, states);
	b2.drawOn(target, states);
}


void
GuiSpinbox::setRect(Recti r) {
	rec = r;
	panel.setRect(rec);
	
	// text
	const int b = 0.25*rec.h;
	text.setRect(Recti(rec.w-b, rec.h-2*b, rec.x+b, rec.y+b));
	
	// buttons
	const int bh = rec.h/2;
	const int bx = rec.xopp() - rec.h;
	b1.setRect(Recti(rec.h, bh, bx, rec.y));
	b2.setRect(Recti(rec.h, bh, bx, rec.y+bh));
}

Recti
GuiSpinbox::rect() const {
	return rec;
}
