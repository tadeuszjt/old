#ifndef VEC2_HPP
#define VEC2_HPP

#include <cmath>
#include "Random.hpp"
#include "Rect.hpp"

struct Vec2f {
	float x, y;
	Vec2f();
	Vec2f(float X, float Y);
	
	void normalise();
	void clamp(Rectf rec);
	void clamp(Recti rec);
	void rotate(float rad);
	
	float dot(Vec2f b) const;
	bool  inRect(Rectf rec) const;
	bool  inRect(Recti rec) const;
	Vec2f rotated(float rad) const;
	float thetaTo(Vec2f b) const;
	
	float len() const;
	float len2() const;
	float theta() const;
	Vec2f normal() const;
	
	void operator += (Vec2f v);
	void operator -= (Vec2f v);
	void operator *= (float f);
	void operator /= (float f);
	
	Vec2f operator + (Vec2f v) const;
	Vec2f operator - (Vec2f v) const;
	Vec2f operator * (float f) const;
	Vec2f operator / (float f) const;
};


struct Vec2i {
	int x, y;
	Vec2i();
	Vec2i(int X, int Y);
	
	bool inRect(Rectf rec) const;
	bool inRect(Recti rec) const;
};

Vec2f operator - (Vec2f v);
Vec2f Vec2f_rand(Rectf rec);
Vec2f Vec2f_randNorm();

#endif
