#ifndef SOUPWINDOW_HPP
#define SOUPWINDOW_HPP

#include "../SoupSFML.hpp"
#include "../Jackstd.hpp"
#include "SoupView.hpp"

class SoupWindow {
	sf::View guiView;
	
	void (*mouseCB)    (Vec2i, bool, bool);
	void (*keyboardCB) (sf::Keyboard::Key, bool press, bool release);
	void (*drawCB)     (sf::RenderTarget&);
	void (*drawGuiCB)  (sf::RenderTarget&);
public:
	sf::RenderWindow sfWindow;
	SoupView soupView;
	
	SoupWindow();
	void run();
	
	void setMouseCB(void (*mouse)(Vec2i pos, bool press, bool release));
	void setKeyboardCB(void (*keyboard)(sf::Keyboard::Key key, bool press, bool release));
	void setDrawCB(void (*draw)(sf::RenderTarget &target));
	void setDrawGuiCB(void (*draw)(sf::RenderTarget &target));
	
	Vec2f mouseWindow() const;
	Vec2f mouseWorld() const;
};


#endif
