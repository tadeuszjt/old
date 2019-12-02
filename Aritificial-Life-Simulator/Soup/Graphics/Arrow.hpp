#ifndef ARROW_HPP
#define ARROW_HPP

#include "../SoupSFML.hpp"
#include "../Jackstd.hpp"
#include "../Resources.hpp"

class Arrow {
	sf::Texture *texture;
	sf::Vertex verts[3][4]; // tail, body, head
	float tlen, hlen;
public:

	Arrow();
	Arrow(Vec2f pos, Vec2f mag);
	
	void setArrow(Vec2f pos, Vec2f mag);
	void setTexture(sf::Texture &tex);
	void setTailTexRec(Rectf r);
	void setBodyTexRec(Rectf r);
	void setHeadTexRec(Rectf r);
	void draw(sf::RenderTarget &target, sf::RenderStates states=sf::RenderStates::Default) const;
};

#endif
