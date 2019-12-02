#include "../Gui/GuiText.hpp"


GuiText::GuiText() {
	alignment = 0;
	text.setFillColor(sf::Color::Black);
	text.setFont(Resources_Sans()); 
	setRect(Recti(100, 24));
}

void
GuiText::printf(const char *fmt, ...) {
	const int len = 100;
	char buf[len];
	va_list args;
	va_start(args, fmt);
	vsnprintf(buf, len, fmt, args);
	va_end(args);	
	text.setString(buf);
}

void
GuiText::alignLeft() {
	alignment = 0;
	setRect(rec);
}

void
GuiText::alignCentre() {
	alignment = 1;
	setRect(rec);
}

void
GuiText::alignRight() {
	alignment = 2;
	setRect(rec);
}

void
GuiText::fontSans() {
	text.setFont(Resources_Sans());
}

void
GuiText::fontSerif() {
	text.setFont(Resources_Serif());
}

void
GuiText::fontMono() {
	text.setFont(Resources_Mono());
}


void
GuiText::setColour(Colour c) {
	text.setFillColor(sf::Color(c.r, c.g, c.b, c.a));
}


void
GuiText::drawOn(sf::RenderTarget &target, sf::RenderStates states) const {
	target.draw(text, states);
}


void
GuiText::setRect(Recti r) {
	rec = r;
	
	/* set character size */
	int csize = 1.4 * float(rec.h);
	if (csize < 0) csize = 0;
	text.setCharacterSize(csize);
	
	/* set alignment */
	int vTrim = float(csize) * -0.29; // vertical trim
	int hTrim = float(csize) * -0.02; // horizontal trim
	if (alignment == 0) {
		text.setPosition(rec.x+hTrim, rec.y+vTrim);
	}
	else if (alignment == 1) {
		int halfText = text.getLocalBounds().width / 2;
		int halfRec = rec.w / 2;
		text.setPosition(rec.x+halfRec-halfText+hTrim, rec.y+vTrim);
	}
	else {
		int textWidth = text.getLocalBounds().width;
		text.setPosition(r.x+rec.w-textWidth+hTrim, rec.y+vTrim);
	}
}


Recti
GuiText::rect() const {
	return rec;
}
