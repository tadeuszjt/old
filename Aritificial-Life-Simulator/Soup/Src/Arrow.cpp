#include "../Graphics/Arrow.hpp"

Arrow::Arrow() {
	texture = &Resources_Arrow();
	setHeadTexRec(Rectf(540, 640, 100, 0));
	setBodyTexRec(Rectf(10,  640, 90,  0));
	setTailTexRec(Rectf(90,  640 , 0,   0));
}

Arrow::Arrow(Vec2f pos, Vec2f mag) {
	texture = &Resources_Arrow();
	setHeadTexRec(Rectf(540, 640, 100, 0));
	setBodyTexRec(Rectf(10,  640, 90,  0));
	setTailTexRec(Rectf(90,  640 , 0,   0));
	setArrow(pos, mag);
}

void
Arrow::setArrow(Vec2f p, Vec2f m) {
	const Vec2f n = m.normal();
	const Vec2f s(n.y/2, -n.x/2); // side
	const Vec2f t = n * tlen;   // tail
	const Vec2f h = n * hlen;   // head
	
	/* spine */
	const Vec2f v[4] = {
		{ p - t     },
		{ p         },
		{ p + m - h },
		{ p + m     }
	};
	
	/* points */
	const Vec2f pt[8] = {
		{ v[0] - s },
		{ v[1] - s },
		{ v[2] - s },
		{ v[3] - s },
		{ v[0] + s },
		{ v[1] + s },
		{ v[2] + s },
		{ v[3] + s }
	};
	
	/* verts */
	sfQuadPositions(verts[0], pt[0], pt[1], pt[5], pt[4]); 
	sfQuadPositions(verts[1], pt[1], pt[2], pt[6], pt[5]); 
	sfQuadPositions(verts[2], pt[2], pt[3], pt[7], pt[6]); 
}

void
Arrow::setTexture(sf::Texture &tex) {
	texture = &tex;
}

void
Arrow::setTailTexRec(Rectf r) {
	tlen = r.w / r.h;
	sfQuadTexcoords(verts[0], r);
}

void
Arrow::setBodyTexRec(Rectf r) {
	sfQuadTexcoords(verts[1], r);
}

void
Arrow::setHeadTexRec(Rectf r) {
	hlen = r.w / r.h;
	sfQuadTexcoords(verts[2], r);
}

void
Arrow::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	states.texture = texture;
	target.draw((sf::Vertex*)verts, 3*4, sf::Quads, states);
}
