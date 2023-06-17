#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Particle.h"

class GridCell {
private:
	std::vector<Particle*> particles;
	std::vector<int> indices;
	sf::Vector2i origin;

public:
	GridCell(sf::Vector2i origin);

	void clear();
	void add(Particle& p, int i);
	size_t getCount();
	Particle* getParticle(size_t i);
	int getParticleIndex(size_t i);
};