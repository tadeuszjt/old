#include <iostream> 

#include "Soup/Window.hpp"
#include "Boats.hpp"
using namespace std;

Boats boats;
SoupWindow window;



void mouse(Vec2i pos, bool press, bool release) {
	boats.mouse(window.mouseWorld(), press, release);
}

int step = 1;
void keyboard(sf::Keyboard::Key key, bool press, bool release) {
	if (press) {
		switch (key) {
			case sf::Keyboard::Space : {
				boats.pause = !boats.pause;
			} break;
			
			case sf::Keyboard::Up : {
				step *= 2;
			} break;
			
			case sf::Keyboard::Down : {
				step /= 2;
				if (step < 1) step = 1;
			} break;
		}
	}
}

int ucount = 0;
void draw(sf::RenderTarget &target) {
	for (int i=0; i<step; i++) {
		boats.update();
		ucount++;
		if (ucount > 60*60) { // print pop count every minute
			ucount = 0;
			cout << boats.list.count << endl;
		}
		
	}
		
	boats.draw(target);
}

void drawGui(sf::RenderTarget &target) {
	for (int i=0; i<boats.selected.count; i++) {
		boats.selected[i]->net.draw(target);
	}
}



int
main() {
	time_t t;
	srand((unsigned) time(&t));
	
	for (int i=0; i<100; i++)
		boats.newBoater();
	
	window.setDrawCB(draw);
	window.setDrawGuiCB(drawGui);
	window.setMouseCB(mouse);
	window.setKeyboardCB(keyboard);
	window.run();
	return 0;
}
