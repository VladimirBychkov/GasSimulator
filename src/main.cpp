#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>
#include <iostream>

#include "World.h"
#include "Renderer.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(1800, 1500), "Gas Simulator", sf::Style::Titlebar | sf::Style::Close);

	int x{ static_cast<int>(window.getSize().x) };
	int y{ static_cast<int>(window.getSize().y) };
	World world{ x, y, sf::Vector2f{0, 0} };
    world.initWorld();
	Renderer renderer;
    
    int frameCounter{ 0 };
	sf::Clock clock;

    while (window.isOpen()) {
        frameCounter++;

		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		world.update();
		sf::Time update_elapsed = clock.restart();
		if (frameCounter % 1 == 0) {
			//world.render(window);
			renderer.render(window, world);
		}		
		sf::Time render_elapsed = clock.restart();

		if (frameCounter % 200 == 0) {
			std::cout << "Frame: " << frameCounter << std::endl;
			std::cout << "Energy: " << world.getEnergy() << std::endl;
			std::cout << "Simulation and rendering time per frame: " << update_elapsed.asMilliseconds() << " " << 
				render_elapsed.asMilliseconds() << std::endl;
		}

		//sf::sleep(sf::milliseconds(1));
	}

	return 0;
}