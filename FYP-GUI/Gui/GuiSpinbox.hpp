#ifndef GUISPINBOX_HPP
#define GUISPINBOX_HPP

#include <climits>

#include "../Jackstd.hpp"
#include "../Resources.hpp"

#include "GuiPanel.hpp"
#include "GuiText.hpp"
#include "GuiButton.hpp"

class GuiSpinbox {
	Recti rec;
	int   count;
	int   min, max;
	float mult;
public:
	GuiButton b1, b2;
	GuiPanel panel;
	GuiText  text;
	
	GuiSpinbox();
	void setCount(int c);
	void setCountRange(int _min, int _max);
	void setCountMultiplier(float m);
	float value() const;
	
	void mouse(Vec2i pos, bool pressed, bool released);
	void drawOn(sf::RenderTarget &target, sf::RenderStates states = sf::RenderStates::Default) const;
	void setRect(Recti r);
	Recti rect() const;
};

#endif
