#include "../SoupSFML.hpp"

void sfQuadPositions(sf::Vertex quad[], Vec2f p0, Vec2f p1, Vec2f p2, Vec2f p3) {
	quad[0].position.x = p0.x;
	quad[0].position.y = p0.y;
	quad[1].position.x = p1.x;
	quad[1].position.y = p1.y;
	quad[2].position.x = p2.x;
	quad[2].position.y = p2.y;
	quad[3].position.x = p3.x;
	quad[3].position.y = p3.y;
}

void sfQuadPositions(sf::Vertex quad[], Rectf rec) {
	const float xopp = rec.x + rec.w;
	const float yopp = rec.y + rec.h;
	quad[0].position.x = rec.x;
	quad[0].position.y = rec.y;
	quad[1].position.x = xopp;
	quad[1].position.y = rec.y;
	quad[2].position.x = xopp;
	quad[2].position.y = yopp;
	quad[3].position.x = rec.x;
	quad[3].position.y = yopp;
}

void sfQuadTexcoords(sf::Vertex quad[], Rectf rec) {
	const float xopp = rec.x + rec.w;
	const float yopp = rec.y + rec.h;
	quad[0].texCoords.x = rec.x;
	quad[0].texCoords.y = rec.y;
	quad[1].texCoords.x = xopp;
	quad[1].texCoords.y = rec.y;
	quad[2].texCoords.x = xopp;
	quad[2].texCoords.y = yopp;
	quad[3].texCoords.x = rec.x;
	quad[3].texCoords.y = yopp;
}

void sfQuadColour(sf::Vertex quad[], Colour c) {
	const sf::Color sfcol(c.r, c.b, c.g, c.a);
	quad[0].color = sfcol;
	quad[1].color = sfcol;
	quad[2].color = sfcol;
	quad[3].color = sfcol;
}
