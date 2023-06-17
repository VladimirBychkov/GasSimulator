#include <SFML/Graphics.hpp>
#include "../Grid.h"

int main() {
	Grid grid{ 5, 5, 10, 10 };

	auto boundary_cells = grid.getBoundaryCells();
	auto n_c_1 = grid.getNeighborCells({ 0, 0 });
	auto n_c_2 = grid.getNeighborCells({ 3, 0 });
	auto n_c_3 = grid.getNeighborCells({ 0, 3 });
	auto n_c_4 = grid.getNeighborCells({ 2, 2 });

	int x{ 0 };
	return 0;
}