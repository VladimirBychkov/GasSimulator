#include "Renderer.h"


Renderer::Renderer() {
	particle_texture.loadFromFile("../assets/circle.png");
}

void Renderer::render(sf::RenderWindow& window, World& world) {
	window.clear(sf::Color(0, 0, 255));

	sf::VertexArray particle_quads(sf::Quads, 4 * world.particles.size());
	for (int i = 0; i < world.particles.size(); i++) {
		float r = world.particles[i].getRadius();
		sf::Vector2f position = world.particles[i].getPosition();

		particle_quads[4 * i + 0].position = sf::Vector2f(position.x - r, position.y - r);
		particle_quads[4 * i + 1].position = sf::Vector2f(position.x + r, position.y - r);
		particle_quads[4 * i + 2].position = sf::Vector2f(position.x + r, position.y + r);
		particle_quads[4 * i + 3].position = sf::Vector2f(position.x - r, position.y + r);

		sf::Vector2u texture_size = particle_texture.getSize();

		particle_quads[4 * i + 0].texCoords = sf::Vector2f(0.f, 0.f);
		particle_quads[4 * i + 1].texCoords = sf::Vector2f(static_cast<float>(texture_size.x), 0.f);
		particle_quads[4 * i + 2].texCoords = sf::Vector2f(static_cast<float>(texture_size.x), static_cast<float>(texture_size.y));
		particle_quads[4 * i + 3].texCoords = sf::Vector2f(0.f, static_cast<float>(texture_size.y));

		sf::Color color = world.particles[i].getColor();

		particle_quads[4 * i + 0].color = color;
		particle_quads[4 * i + 1].color = color;
		particle_quads[4 * i + 2].color = color;
		particle_quads[4 * i + 3].color = color;
	}

	window.draw(particle_quads, &particle_texture);

	window.display();
}