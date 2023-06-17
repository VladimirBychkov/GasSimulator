#pragma once
#include <SFML/Graphics.hpp>

class Particle {
private:
	sf::Vector2f pos;
	sf::Vector2f velocity;
	float m;
	float q;
	float radius;
	sf::Color color;
public:
	Particle(float x, float y, float vx, float vy, float m=1, float q=1, 
		float radius=1, sf::Color color=sf::Color::Red);
	
	sf::Vector2f getPosition();
	sf::Vector2f getVelocity();
	float getAbsVelocity();
	float getMass();
	float getCharge();
	float getRadius();
	sf::Color getColor();

	void setPosition(sf::Vector2f r);
	void setVelocity(float vx, float vy);
	void setVelocity(sf::Vector2f v);
	void setColor(sf::Color c);

	void move(sf::Vector2f offset);

	friend void swapVelocities(Particle& lhs, Particle& rhs);
};

bool performCollision(Particle& p_1, Particle& p_2);
