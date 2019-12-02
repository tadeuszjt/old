#include "../Jackstd/Rect.hpp"

Rectf::Rectf() {
	w = 0;
	h = 0;
	x = 0;
	y = 0;
}

Rectf::Rectf(float width, float height) {
	w = width;
	h = height;
	x = 0;
	y = 0;
}

Rectf::Rectf(float width, float height, float xpos, float ypos) {
	w = width;
	h = height;
	x = xpos;
	y = ypos;
}

float
Rectf::xopp() const {
	return x + w;
}

float
Rectf::yopp() const {
	return y + h;
}



Recti::Recti() {
	w = 0;
	h = 0;
	x = 0;
	y = 0;
}

Recti::Recti(int width, int height) {
	w = width;
	h = height;
	x = 0;
	y = 0;
}

Recti::Recti(int width, int height, int xpos, int ypos) {
	w = width;
	h = height;
	x = xpos;
	y = ypos;
}

int
Recti::xopp() const {
	return x + w;
}

int
Recti::yopp() const {
	return y + h;
}

bool
Recti::operator != (Recti b) {
	return (
		w != b.w ||
		h != b.h ||
		x != b.x ||
		y != b.y
	);
}
