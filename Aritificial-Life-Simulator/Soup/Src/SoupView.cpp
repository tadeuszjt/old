#include "../Window/SoupView.hpp"

void
SoupView::update() {
	const float error = currentzoom - targetzoom;
	
	if (fabs(error) > 0.0001) {
		currentzoom -= error * 0.3;
	
		const float scale = powf(2, currentzoom);
		const float diffscale = scale - currentscale;
		currentscale = scale;
		
		setSize(viewsize.x*scale, viewsize.y*scale);
		move(-mouse.x*diffscale, -mouse.y*diffscale);
	}
}

void 
SoupView::resize(sf::Vector2u size) {
	viewsize.x = size.x;
	viewsize.y = size.y;
	halfsize.x = size.x / 2.0;
	halfsize.y = size.y / 2.0;
	setSize(viewsize.x*currentscale, viewsize.y*currentscale);
}

void 
SoupView::zoom(float delta, sf::Vector2i mousepos) {
	targetzoom -= delta * 0.2;
	mouse.x = mousepos.x - halfsize.x;
	mouse.y = mousepos.y - halfsize.y;
}

void
SoupView::setZoom(float scale, sf::Vector2f centre) {
	const float zoom = log2f(scale);
	currentscale     = scale;
	targetzoom       = zoom;
	currentzoom      = zoom;
	
	setSize(viewsize.x*currentscale, viewsize.y*currentscale);
	setCenter(centre);
}
