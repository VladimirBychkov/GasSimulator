#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "World.h"
#include "Particle.h"


class Renderer {
private:
	sf::Texture particle_texture;

public:
	Renderer();
	void render(sf::RenderWindow& window, World& world);
};