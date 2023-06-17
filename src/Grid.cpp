#include "Grid.h"

Grid::Grid(int width, int height, int cell_width, int cell_height) : width(width), height(height), cell_width(cell_width), cell_height(cell_height), 
	neighbor_origins{ { -1, -1 }, { -1, 0 }, { -1, 1 }, { 0, -1 }, { 0, 1 }, { 1, -1 }, { 1, 0 }, { 1, 1 } } {

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			cells.push_back(GridCell(sf::Vector2i(i, j)));
		}		
	}

	for (int i = 0; i < width; i++) {
		boundary_cells.push_back(&cells[i]);
	}
	for (int i = 0; i < width; i++) {
		boundary_cells.push_back(&cells[i + width * (height - 1)]);
	}
	for (int j = 1; j < height - 1; j++) {
		boundary_cells.push_back(&cells[width * j]);
	}
	for (int j = 1; j < height - 1; j++) {
		boundary_cells.push_back(&cells[width -1 + width * j]);
	}
	neighbor_cells.reserve(8);
}

bool Grid::valid_cell_idx(sf::Vector2i cell_idx) {
	if (cell_idx.x < 0 || cell_idx.x >= width) {
		return false;
	}
	if (cell_idx.y < 0 || cell_idx.y >= height) {
		return false;
	}
	return true;
}

std::vector<GridCell*>& Grid::getBoundaryCells() {
	return boundary_cells;
}

std::vector<GridCell*>& Grid::getNeighborCells(sf::Vector2i origin) {
	neighbor_cells.clear();

	for (auto& neighbor_origin : neighbor_origins) {
		if (valid_cell_idx(origin + neighbor_origin)) {
			int cell_idx = (origin + neighbor_origin).x + width * (origin + neighbor_origin).y;
			neighbor_cells.push_back(&cells.at(cell_idx));
		}
	}

	return neighbor_cells;
}

GridCell& Grid::getGridCell(sf::Vector2i origin) {
	return cells.at(origin.x + origin.y * width);
}

sf::Vector2i Grid::getParticleCellOrigin(Particle& particle) {
	int origin_x{ static_cast<int>(floor(particle.getPosition().x / cell_width))};
	int origin_y{ static_cast<int>(floor(particle.getPosition().y / cell_height))};
	sf::Vector2i origin{ origin_x, origin_y };
	return origin;
}

std::vector<std::pair<Particle*, size_t>> Grid::getNeighborParticles(Particle& particle, size_t idx) {
	std::vector<std::pair<Particle*, size_t>> neigbor_particles;

	sf::Vector2i origin = getParticleCellOrigin(particle);
	std::vector<GridCell*> search_cells = getNeighborCells(origin);
	search_cells.push_back(& getGridCell(origin));

	for (auto& cell : search_cells) {
		for (int j = 0; j < cell->getCount(); j++) {
			if (idx == cell->getParticleIndex(j)) {
				continue;
			}

			neigbor_particles.push_back(std::make_pair(cell->getParticle(j), cell->getParticleIndex(j)));
		}
	}
	return neigbor_particles;
}

int Grid::getWidth() {
	return width;
}

int Grid::getHeight() {
	return height;
}

int Grid::getCellWidth() {
	return cell_width;
}

int Grid::getCellHeight() {
	return cell_height;
}

void Grid::clearCells() {
	for (auto& cell : cells) {
		cell.clear();
	}
}

void Grid::updateGrid(std::vector<Particle>& particles) {
	clearCells();

	for (size_t i = 0; i < particles.size(); i++) {
		float x{ particles[i].getPosition().x };
		float y{ particles[i].getPosition().y };
		int origin_x{ static_cast<int>(floor(x / cell_width)) };
		int origin_y{ static_cast<int>(floor(y / cell_height)) };
		sf::Vector2i origin{ origin_x, origin_y };
		getGridCell(origin).add(particles[i], i);
	}
}