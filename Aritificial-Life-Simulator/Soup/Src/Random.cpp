#include "../Jackstd/Random.hpp"

float randflt(float min, float max) {
	return (float(rand())*(max - min))/RAND_MAX + min;
}

int randint(int min, int max) {
	return rand() % (max+1 - min) + min;
}

bool diceroll(int odds) {
	return rand() % odds == 0;
}
