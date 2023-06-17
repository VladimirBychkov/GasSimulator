#pragma once
#include <vector>
#include <utility>
#include <SFML/Graphics.hpp>
#include "GridCell.h"
#include "Particle.h"

class Grid {
private:
	int width;
	int height;
	int cell_width;
	int cell_height;
	std::vector<GridCell> cells;
	std::vector<GridCell*> boundary_cells;
	std::vector<GridCell*> neighbor_cells;
	std::vector<sf::Vector2i> neighbor_origins;

	// utils functions
	bool valid_cell_idx(sf::Vector2i cell_idx);

public:
	Grid(int width, int height, int cell_width, int cell_height);

	// getters
	int getWidth();
	int getHeight();
	int getCellWidth();
	int getCellHeight();
	std::vector<GridCell*>& getBoundaryCells();
	std::vector<GridCell*>& getNeighborCells(sf::Vector2i origin);
	std::vector<std::pair<Particle*, size_t>> getNeighborParticles(Particle& particle, size_t idx);
	GridCell& getGridCell(sf::Vector2i origin);
	sf::Vector2i getParticleCellOrigin(Particle& particle);

	void clearCells();
	void updateGrid(std::vector<Particle>& particles);
};