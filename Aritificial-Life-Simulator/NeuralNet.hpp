#ifndef NEURALNET_HPP
#define NEURALNET_HPP

#include <cstdio>
#include "Soup/Jackstd.hpp" 
#include "Soup/SoupSFML.hpp"
#include "Soup/Resources.hpp"

class NeuralNet {
public:
	int rows, cols;
	int ni, nh, no, nw, nf; // num inputs, hiddens, outputs, weights, floats
	float *input, *hidden, *output, *weight, *fltmem;

	NeuralNet(int numInputs, int hiddenRows, int hiddenCols, int numOutputs);
	~NeuralNet();
	void  randomise();
	float sigmoid(float a) const;
	void  process();
	
	void fprint(FILE *fp);
	void draw(sf::RenderTarget &target);
};

#endif 
