#include "Particle.h"
#include "utils_math.h"

Particle::Particle(float x, float y, float vx, float vy, float m, float q, float radius, sf::Color color): 
	pos(x, y), velocity(vx, vy), m(m), q(q), radius(radius), color(color) {
}

sf::Vector2f Particle::getPosition() {
	return pos;
}

float Particle::getAbsVelocity() {
	return sqrt(scalar_multiply(velocity, velocity));
}

sf::Vector2f Particle::getVelocity() {
	return velocity;
}

float Particle::getMass() {
	return m;
}

float Particle::getCharge() {
	return q;
}

float Particle::getRadius() {
	return radius;
}

sf::Color Particle::getColor() {
	return color;
}

void Particle::setPosition(sf::Vector2f r) {
	pos = r;
}

void Particle::setVelocity(float vx, float vy) {
	velocity.x = vx;
	velocity.y = vy;
}

void Particle::setVelocity(sf::Vector2f v) {
	velocity = v;
}

void Particle::setColor(sf::Color c) {
	color = c;
}

void Particle::move(sf::Vector2f offset) {
	pos += offset;
}

void swapVelocities(Particle& lhs, Particle& rhs) {
	std::swap(lhs.velocity.x, rhs.velocity.x);
	std::swap(lhs.velocity.y, rhs.velocity.y);
}

bool performCollision(Particle& p_1, Particle& p_2) {
    sf::Vector2f d = p_2.getPosition() - p_1.getPosition();
	if (abs(d.x) < 0.0001f && abs(d.y) < 0.0001f) {
		return true;
	}

    float r_1 = p_1.getRadius();
    float r_2 = p_2.getRadius();

    float m_1 = p_1.getMass();
    float m_2 = p_2.getMass();

    float dist_squared = scalar_multiply(d, d);
    float constraint = (r_1 + r_2) * (r_1 + r_2);
	if (dist_squared < constraint) {
		sf::Vector2f u_relative = p_2.getVelocity() - p_1.getVelocity();
		p_1.setVelocity(p_1.getVelocity() + (2 * m_2 / (m_1 + m_2)) * d * (scalar_multiply(u_relative, d) / (dist_squared)));
		p_2.setVelocity(p_2.getVelocity() - (2 * m_1 / (m_1 + m_2)) * d * (scalar_multiply(u_relative, d) / (dist_squared)));

		sf::Vector2f true_distance = d / sqrt(dist_squared) * (r_1 + r_2);
		p_1.setPosition(p_1.getPosition() - 0.5f * (true_distance - d));
		p_2.setPosition(p_2.getPosition() + 0.5f * (true_distance - d));

		return true;
	}

	return false;
}