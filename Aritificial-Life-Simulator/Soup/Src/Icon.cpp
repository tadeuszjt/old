#include "../Graphics/Icon.hpp"

Icon::Icon() {
	texture.setSmooth(true);
	texture.generateMipmap();
}

Icon::Icon(const char filename[]) {
	texture.setSmooth(true);
	loadFile(filename);
}

Icon::~Icon() {
	for (int i=0; i<icons.count; i++)
		icons[i]->icon = nullptr;
}

void
Icon::loadFile(const char filename[]) {
	texture.loadFromFile(filename);
	texture.generateMipmap();
}

void
Icon::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	states.texture = &texture;
	target.draw(verts.array, verts.count, sf::Quads, states);
}




Iconer::Iconer() {
	icon = nullptr;
	rec  = Rectf(1, 1, -0.5, -0.5);
	col  = Colour(255, 255, 255, 255);
	mag  = Vec2f(1.0, 0.0);
}

Iconer::~Iconer() {
	unSetIcon();
}

void
Iconer::setIcon(Icon &ic) {
	unSetIcon();
	icon  = &ic;
	index = icon->icons.count;
	icon->icons.add(this);
	icon->verts.extend(4);
	
	sf::Vector2u tsize = icon->texture.getSize();
	setIconRect(Rectf(tsize.x, tsize.y));
	setCol(col);
	setPos(pos);
}

void
Iconer::unSetIcon() {
	if (icon) {
		icon->icons.remove(index);
		const int vloc = index*4;
		icon->verts.remove(vloc+3);
		icon->verts.remove(vloc+2);
		icon->verts.remove(vloc+1);
		icon->verts.remove(vloc+0);
		
		if (icon->icons.count > index) // switcheroo
			icon->icons.array[index]->index = index;
	}
	icon = nullptr;
}

void
Iconer::setIconRect(Rectf r) {
	iconRec = r;
	if (icon) {
		sf::Vertex *quad = &icon->verts[index*4];
		sfQuadTexcoords(quad, r);
	}
}

void
Iconer::setRect(Rectf r) {
	rec = r;
	setPos(pos);
}

void
Iconer::setCol(Colour c) {
	col = c;
	if (icon) {
		sf::Vertex *quad = &icon->verts[index*4];
		sfQuadColour(quad, c);
	}
}

void
Iconer::setPos(Vec2f p) {
	pos = p;
	if (icon) {
		sf::Vertex *quad = &icon->verts[index*4];
		
		if (mag.x == 1.0 && mag.y == 0.0)
			sfQuadPositions(quad, Rectf(rec.w, rec.h, rec.x+p.x, rec.y+p.y));
		
		else {
			const float dxx0 = mag.x*rec.x;
			const float dyx0 = mag.y*rec.x;
			const float dxx1 = mag.x*rec.xopp();
			const float dyx1 = mag.y*rec.xopp();
			
			const float dxy0 = mag.x*rec.y;
			const float dyy0 = mag.y*rec.y;
			const float dxy1 = mag.x*rec.yopp();
			const float dyy1 = mag.y*rec.yopp();
			
			quad[0].position = sf::Vector2f(dxx0-dyy0 + p.x, dyx0+dxy0 + p.y);
			quad[1].position = sf::Vector2f(dxx1-dyy0 + p.x, dyx1+dxy0 + p.y);
			quad[2].position = sf::Vector2f(dxx1-dyy1 + p.x, dyx1+dxy1 + p.y);
			quad[3].position = sf::Vector2f(dxx0-dyy1 + p.x, dyx0+dxy1 + p.y);
		}	
	}
}

void
Iconer::setPos(Vec2f p, Vec2f m) {
	pos = p;
	mag = m;
	setPos(pos);
}

