#include <SFML/Graphics.hpp>

int main() {
	sf::RenderWindow window(sf::VideoMode(1500, 1200), "Test Batch Drawing", sf::Style::Titlebar | sf::Style::Close);

	while (window.isOpen()) {
		window.clear(sf::Color(0, 0, 255));

		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		sf::CircleShape circle(20.0f);
		circle.setPosition(sf::Vector2f(0, 0));
		circle.setFillColor(sf::Color::Green);
		window.draw(circle);


		sf::VertexArray quad(sf::Quads, 8);
		sf::Texture texture;
		texture.loadFromFile("../../assets/circle.png");
		//texture.loadFromFile("D:\\Work\\C++\\GasSimulator_CMake\\src\\circle.png");

		quad[0].position = sf::Vector2f(500.f, 500.f);
		quad[1].position = sf::Vector2f(700.f, 500.f);
		quad[2].position = sf::Vector2f(700.f, 700.f);
		quad[3].position = sf::Vector2f(500.f, 700.f);

		quad[0].color = sf::Color::Blue;
		quad[1].color = sf::Color::Green;
		quad[2].color = sf::Color::Red;
		
		quad[0].texCoords = sf::Vector2f(0.f, 0.f);
		quad[1].texCoords = sf::Vector2f(30.f, 0.f);
		quad[2].texCoords = sf::Vector2f(30.f, 30.f);
		quad[3].texCoords = sf::Vector2f(0.f, 30.f);

		quad[4].position = sf::Vector2f(450.f, 450.f);
		quad[5].position = sf::Vector2f(550.f, 450.f);
		quad[6].position = sf::Vector2f(550.f, 550.f);
		quad[7].position = sf::Vector2f(450.f, 550.f);

		quad[4].texCoords = sf::Vector2f(0.f, 0.f);
		quad[5].texCoords = sf::Vector2f(30.f, 0.f);
		quad[6].texCoords = sf::Vector2f(30.f, 30.f);
		quad[7].texCoords = sf::Vector2f(0.f, 30.f);

		window.draw(quad, &texture);

		window.display();
	}

	return 0;
}