#include "../Gui/GuiMargin.hpp"

GuiMargin::GuiMargin() {
	l = t = r = b = 0;
}

void
GuiMargin::setMargins(int left, int top, int right, int bottom) {
	l = left;
	t = top;
	r = right;
	b = bottom;
	
	x[0] = rec.x;
	x[1] = rec.x + left;
	x[2] = rec.xopp() - right; 
	x[3] = rec.xopp();
	
	y[0] = rec.y;
	y[1] = rec.y + top;
	y[2] = rec.yopp() - bottom;
	y[3] = rec.yopp();
}


void
GuiMargin::setRect(Recti rc) {
	rec = rc;
	setMargins(l, t, r, b);
}

Recti
GuiMargin::rect() const {
	return rec;
}

Recti
GuiMargin::rect(int row, int col) const {
	return Recti(x[col+1]-x[col], y[row+1]-y[row], x[col], y[row]);
}
