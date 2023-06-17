#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <set>
#include "Particle.h"
#include "Grid.h"

class Renderer;

class World {
private:
	std::vector<Particle> particles;
	int borderX;
	int borderY;
	sf::Vector2f gravity;

	Grid* grid;

public:
	World(int x, int y, sf::Vector2f gravity);
	void initWorld();
	void createParticle(sf::Vector2i pos);
	size_t getParticleCount();
	void update();

	sf::Vector2f getElectricForce(int i, sf::Vector2f pos);
	void checkAndPerformBoundaryCollision(Particle& particle);
	void performClassicalCollisions();

	void performFastBoundaryCollisions();
	void performFastClassicalCollisions();

	float getEnergy();

	void render(sf::RenderWindow &window);

	friend class Renderer;
};