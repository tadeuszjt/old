#include "../Resources.hpp"

sf::Texture&
Resources_Ship() {
	static sf::Texture tex;
	static bool loaded = false;
	if (!loaded) {
		tex.loadFromFile("Soup/Resources/Ship.png");
		tex.setSmooth(true);
		tex.generateMipmap();
		loaded = true;
	}
	return tex;
}

sf::Texture&
Resources_Boat() {
	static sf::Texture tex;
	static bool loaded;
	if (!loaded) {
		tex.loadFromFile("Soup/Resources/Boat.png");
		tex.setSmooth(true);
		tex.generateMipmap();
		loaded = true;
	}
	return tex;
}

sf::Texture&
Resources_Arrow() {
	static sf::Texture tex;
	static bool loaded = false;
	if (!loaded) {
		tex.loadFromFile("Soup/Resources/Arrow.png");
		tex.setSmooth(true);
		tex.generateMipmap();
		loaded = true;
	}
	return tex;
}

sf::Texture&
Resources_Circle() {
	static sf::Texture tex;
	static bool loaded = false;
	if (!loaded) {
		tex.loadFromFile("Soup/Resources/Circle.png");
		tex.setSmooth(true);
		tex.generateMipmap();
		loaded = true;
	}
	return tex;
}

sf::Texture&
Resources_Ring() {
	static sf::Texture tex;
	static bool loaded = false;
	if (!loaded) {
		tex.loadFromFile("Soup/Resources/Ring.png");
		tex.setSmooth(true);
		tex.generateMipmap();
		loaded = true;
	}
	return tex;
}
