#ifndef RECT_HPP
#define RECT_HPP

struct Rectf {
	float w, h, x, y;
	Rectf();
	Rectf(float width, float height);
	Rectf(float width, float height, float xpos, float ypos);
	float xopp() const;
	float yopp() const;
};


struct Recti {
	int w, h, x, y;
	Recti();
	Recti(int width, int height);
	Recti(int width, int height, int xpos, int ypos);
	int xopp() const;
	int yopp() const;
	
	bool operator != (Recti b);
};

#endif
