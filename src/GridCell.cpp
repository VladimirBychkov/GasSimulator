#include "GridCell.h"


GridCell::GridCell(sf::Vector2i origin): origin(origin) {
	particles.reserve(20);
	indices.reserve(20);
}

void GridCell::clear() {
	particles.clear();
	indices.clear();
}

void GridCell::add(Particle& p, int i) {
	particles.push_back(&p);
	indices.push_back(i);
}

size_t GridCell::getCount()
{
	return particles.size();
}

Particle* GridCell::getParticle(size_t i) {
	return particles[i];
}

int GridCell::getParticleIndex(size_t i) {
	return indices[i];
}
