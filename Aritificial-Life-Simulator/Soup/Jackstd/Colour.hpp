#ifndef COLOUR_HPP
#define COLOUR_HPP

#include <cstdlib>
#include <cstdint> 

struct Colour {
	uint8_t r, g, b, a;
	Colour();
	Colour(uint8_t R, uint8_t G, uint8_t B, uint8_t A=255);
};

Colour Colour_rand();

#endif
