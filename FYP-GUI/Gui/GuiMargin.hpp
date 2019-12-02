#ifndef GUIMARGIN_HPP
#define GUIMARGIN_HPP

#include "../SoupSFML.hpp"
#include "../Jackstd.hpp"

class GuiMargin {
	Recti rec;
	int l, t, r, b;
	int x[4], y[4];
public:

	GuiMargin();
	void setMargins(int left, int top, int right, int bottom);
	
	void setRect(Recti rc);
	Recti rect(int row, int col) const;
	Recti rect() const;
};

#endif 
