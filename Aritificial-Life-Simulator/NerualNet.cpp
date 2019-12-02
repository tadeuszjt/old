#include "NeuralNet.hpp"

NeuralNet::NeuralNet(int numInputs, int hiddenRows, int hiddenCols, int numOutputs) {
	ni = numInputs;
	no = numOutputs;
	rows = hiddenRows;
	cols = hiddenCols;
	
	nh = rows*cols;
	nw = (ni+no+2)*rows + (rows+2)*rows*(cols-1) + rows*no;
	nf = ni + nh + no + nw;
	
	fltmem = new float [nf]();
	input = fltmem;
	hidden = input + ni;
	output = hidden + nh;
	weight = output + no;
}


NeuralNet::~NeuralNet() { 
	delete [] fltmem;
}

void
NeuralNet::randomise() {
	for (int i=0; i<nw; i++)
		weight[i] = randflt(-1.0, 1.0);
}
	
	
float
NeuralNet::sigmoid(float a) const {
	const float a4 = a*4;
	return (0.5*a4) / (1 + fabs(a4)) + 0.5;
}
	

void
NeuralNet::process() {
	float *w = weight;
	
	// 1st hidden layer
	for (int h=0; h<rows; h++) {
		float a = 0.0;
		for (int i = 0; i<ni; i++) // read inputs
			a += *w++ * input[i];
			
		for (int i = 0; i<no; i++) // read outputs
			a += *w++ * output[i];
		
		a += *w++ * hidden[h]; // recurrent
		a += *w++;             // bias
		hidden[h] = sigmoid(a);
	}
	
	// rest of hiddens
	for (int c=1; c<cols; c++) {
		float *hid = hidden + c*rows;
		float *prv = hidden + (c-1)*rows;
		
		for (int h=0; h<rows; h++) {
			float a = 0.0;
			for (int i=0; i<rows; i++)
				a += *w++ * prv[i];
			
			a += *w++ * hid[h]; // recurrent
			a += *w++;          // bias
			hid[h] = sigmoid(a);
		}
	}
	
	// outputs
	for (int o=0; o<no; o++) {
		float *last = hidden + (cols-1)*(rows);
		
		float a = 0.0;
		for (int i=0; i<rows; i++)
			a += *w++ * last[i];
			
		CLAMP(a, 0.0, 1.0);
		output[o] = a;
	}
}


void
NeuralNet::fprint(FILE *fp) {
	fprintf(fp, "%d floats\n", nf);
	fprintf(fp, "%d inputs\n", ni);
	fprintf(fp, "%d hiddens\n", nh);
	fprintf(fp, "%d outputs\n", no);
	fprintf(fp, "%d weights\n", nw);
}


void
NeuralNet::draw(sf::RenderTarget &target) {
	sf::Sprite circle, ring;
	circle.setScale(16./640., 16./640.);
	circle.setTexture(Resources_Circle());
	ring.setScale(16./640., 16./640.);
	ring.setTexture(Resources_Ring());
	ring.setColor(sf::Color(68, 68, 68));
	
	// draw inputs 
	for (int i=0; i<ni; i++) {
		uint8_t val = input[i] * 255;
		circle.setColor(sf::Color(255, 0, 0, val));
		circle.setPosition(0, 18*i);
		ring.setPosition(0, 18*i);
		target.draw(circle);
		target.draw(ring);
	}
	
	// draw hiddens
	for (int c=0; c<cols; c++) {
		for (int r=0; r<rows; r++) {
			uint8_t val = hidden[c*rows + r] * 255;
			circle.setColor(sf::Color(255, 0, 0, val));
			circle.setPosition(18*(1+c), 18*r);
			ring.setPosition(18*(1+c), 18*r);
			target.draw(circle);
			target.draw(ring);
		}
	}
	
	// draw outputs
	for (int i=0; i<no; i++) {
		uint8_t val = output[i] * 255;
		circle.setColor(sf::Color(255, 0, 0, val));
		circle.setPosition(18*(1+cols), 18*i);
		ring.setPosition(18*(1+cols), 18*i);
		target.draw(circle);
		target.draw(ring);
	}
}

	
