#include "../Jackstd/Colour.hpp"

Colour::Colour() {
	r = 255;
	g = 255;
	b = 255;
	a = 255;
}

Colour::Colour(uint8_t R, uint8_t G, uint8_t B, uint8_t A) {
	r = R;
	g = G;
	b = B;
	a = A;
}

Colour Colour_rand() {
	uint8_t r = rand() % 255;
	uint8_t g = rand() % 255;
	uint8_t b = rand() % 255;	
	return Colour(r, g, b);
}
