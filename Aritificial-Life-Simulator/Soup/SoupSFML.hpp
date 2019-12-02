#ifndef SOUPSFML_HPP
#define SOUPSFML_HPP

#include <SFML/Graphics.hpp>
#include "Jackstd.hpp"

void sfQuadPositions(sf::Vertex quad[], Vec2f p0, Vec2f p1, Vec2f p2, Vec2f p3);
void sfQuadPositions(sf::Vertex quad[], Rectf rec);
void sfQuadTexcoords(sf::Vertex quad[], Rectf rec);
void sfQuadColour(sf::Vertex quad[], Colour col);

#endif
