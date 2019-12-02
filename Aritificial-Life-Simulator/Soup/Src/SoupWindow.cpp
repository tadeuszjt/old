#include "../Window/SoupWindow.hpp"

SoupWindow::SoupWindow() {
	sfWindow.create(sf::VideoMode(800, 600), "Soup", sf::Style::Default);
	sfWindow.setVerticalSyncEnabled(true);
	soupView.setZoom(1.0, sf::Vector2f(0,0));
	
	mouseCB = nullptr;
	keyboardCB = nullptr;
	drawCB = nullptr;
	drawGuiCB = nullptr;
}

void
SoupWindow::run() {
	while (sfWindow.isOpen()) {
		sf::Event event;
	
		// poll events
		while (sfWindow.pollEvent(event)) {
			switch (event.type) {
				case sf::Event::Closed :
					sfWindow.close();
					break;
					
					
				case sf::Event::Resized : {
						soupView.resize(sfWindow.getSize());
						sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
						guiView = sf::View(visibleArea);
					} break;
					
				case sf::Event::MouseWheelScrolled : {
						sf::Vector2i pos;
						pos.x = event.mouseWheelScroll.x;
						pos.y = event.mouseWheelScroll.y;
						soupView.zoom(event.mouseWheelScroll.delta, pos);
					} break;
				
				case sf::Event::MouseMoved :
					if (mouseCB) {
						Vec2i pos;
						pos.x = event.mouseMove.x;
						pos.y = event.mouseMove.y;
						(*mouseCB)(pos, false, false);
					} break;
				
				case sf::Event::MouseButtonPressed :
					if (mouseCB) {
						Vec2i pos;
						pos.x = event.mouseButton.x;
						pos.y = event.mouseButton.y;
						(*mouseCB)(pos, true, false);
					} break;
				
				case sf::Event::MouseButtonReleased :
					if (mouseCB) {
						Vec2i pos;
						pos.x = event.mouseButton.x;
						pos.y = event.mouseButton.y;
						(*mouseCB)(pos, false, true);
					} break;
					
				case sf::Event::KeyPressed : 
					if (keyboardCB) {
						(*keyboardCB)(event.key.code, true, false);
					} break;
					
				case sf::Event::KeyReleased :
					if (keyboardCB) {
						(*keyboardCB)(event.key.code, false, true);
					} break;
			}
		}
		
		// render
		soupView.update();
		sfWindow.clear(sf::Color(245, 245, 245));
		if (drawCB) {
			sfWindow.setView(soupView);
			(*drawCB)(sfWindow);
		}
		
		if (drawGuiCB) {
			sfWindow.setView(guiView);
			(*drawGuiCB)(sfWindow);
		}
		
		sfWindow.display();
	}
}

void
SoupWindow::setMouseCB(void (*mouse)(Vec2i, bool, bool)) {
	mouseCB = mouse;
}

void
SoupWindow::setKeyboardCB(void (*keyboard)(sf::Keyboard::Key, bool, bool)) {
	keyboardCB = keyboard;
}

void
SoupWindow::setDrawCB(void (*draw)(sf::RenderTarget&)) {
	drawCB = draw;
}

void
SoupWindow::setDrawGuiCB(void (*draw)(sf::RenderTarget&)) {
	drawGuiCB = draw;
}

Vec2f
SoupWindow::mouseWindow() const {
	sf::Vector2i m = sf::Mouse::getPosition(sfWindow);
	return Vec2f(m.x, m.y);	
}

Vec2f
SoupWindow::mouseWorld() const {
	sf::Vector2i m  = sf::Mouse::getPosition(sfWindow);
	sf::Vector2f mw = sfWindow.mapPixelToCoords(m, soupView);
	return Vec2f(mw.x, mw.y);
}
