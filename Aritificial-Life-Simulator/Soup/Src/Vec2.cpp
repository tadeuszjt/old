#include "../Jackstd/Vec2.hpp"

Vec2f::Vec2f() {
	x = 0;
	y = 0;
}

Vec2f::Vec2f(float X, float Y) {
	x = X;
	y = Y;
}

void
Vec2f::normalise() {
	if (x == 0.0 && y == 0.0) return;
	const float inv = 1.0 / len();
	if (isnanf(inv) || isinff(inv)) return;
	x *= inv;
	y *= inv;
}

void
Vec2f::clamp(Rectf rec) {
	if      (x < rec.x     ) x = rec.x;
	else if (x > rec.xopp()) x = rec.xopp();
	if      (y < rec.y     ) y = rec.y;
	else if (y > rec.yopp()) y = rec.yopp();
}

void
Vec2f::clamp(Recti rec) {
	if      (x < rec.x     ) x = rec.x;
	else if (x > rec.xopp()) x = rec.xopp();
	if      (y < rec.y     ) y = rec.y;
	else if (y > rec.yopp()) y = rec.yopp();
}

void
Vec2f::rotate(float rad) {
	const float s = sinf(rad);
	const float c = cosf(rad);
	const float X = x;
	const float Y = y;
	x = c*X - s*Y;
	y = s*X + c*Y;
}

float
Vec2f::dot(Vec2f b) const {
	return x*b.x + y*b.y;
}


bool
Vec2f::inRect(Rectf rec) const {
	return (
		x > rec.x &&
		x < rec.xopp() &&
		y > rec.y &&
		y < rec.yopp()
	);
}

bool
Vec2f::inRect(Recti rec) const {
	return (
		x > rec.x &&
		x < rec.xopp() &&
		y > rec.y &&
		y < rec.yopp()
	);
}

float
Vec2f::len() const {
	return sqrtf(x*x + y*y);
}

float
Vec2f::len2() const {
	return x*x + y*y;
}

float
Vec2f::theta() const {
	return atan2f(y, x);
}


Vec2f
Vec2f::normal() const {
	Vec2f v(x, y);
	v.normalise();
	return v;
}

Vec2f
Vec2f::rotated(float rad) const {
	Vec2f v(x, y);
	v.rotate(rad);
	return v;
}

float
Vec2f::thetaTo(Vec2f b) const {
	return atan2f(x*b.y-y*b.x, x*b.x+y*b.y);
}


void
Vec2f::operator += (Vec2f v) {
	x += v.x;
	y += v.y;
}

void
Vec2f::operator -= (Vec2f v) {
	x -= v.x;
	y -= v.y;
}

void
Vec2f::operator *= (float f) {
	x *= f;
	y *= f;
}

void 
Vec2f::operator /= (float f) {
	const float inv = 1.0 / f;
	x *= inv;
	y *= inv;
}

Vec2f
Vec2f::operator + (Vec2f v) const {
	return Vec2f(x+v.x, y+v.y);
}

Vec2f
Vec2f::operator - (Vec2f v) const {
	return Vec2f(x-v.x, y-v.y);
}

Vec2f
Vec2f::operator * (float f) const {
	return Vec2f(x*f, y*f);
}

Vec2f
Vec2f::operator / (float f) const {
	const float inv = 1.0 / f;
	return Vec2f(x*inv, y*inv);
}



Vec2i::Vec2i() {
	x = 0;
	y = 0;
}

Vec2i::Vec2i(int X, int Y) {
	x = X;
	y = Y;
}

bool
Vec2i::inRect(Rectf rec) const {
	return (
		x > rec.x &&
		x < rec.xopp() &&
		y > rec.y &&
		y < rec.yopp()
	);
}

bool
Vec2i::inRect(Recti rec) const {
	return (
		x > rec.x &&
		x < rec.xopp() &&
		y > rec.y &&
		y < rec.yopp()
	);
}



Vec2f operator - (Vec2f v) {
	return Vec2f(-v.x, -v.y);
}


Vec2f Vec2f_rand(Rectf rec) {
	const float X = randflt(rec.x, rec.xopp());
	const float Y = randflt(rec.y, rec.yopp());
	return Vec2f(X, Y);
}

Vec2f Vec2f_randNorm() {
	const float r = randflt(0, 2*M_PI);
	return Vec2f(sinf(r), cosf(r));
}
