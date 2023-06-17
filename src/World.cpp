#include "World.h"
#include "utils_math.h"
#include <iostream>
#include <set>

World::World(int x, int y, sf::Vector2f gravity) : borderX(x), borderY(y), gravity(gravity) {
    //grid = new Grid(150, 120, static_cast<int>(x / 150), static_cast<int>(y / 120));
    //grid = new Grid(300, 240, static_cast<int>(x / 300), static_cast<int>(y / 240));
    grid = new Grid(360, 300, static_cast<int>(x / 150), static_cast<int>(y / 120));
}

void World::initWorld() {
    float r = 2;
    float m = 1;
    float q = 0;
    for (int i = 0; i < 80; i++) {
        for (int j = 0; j < 80; j++) {
            particles.push_back(Particle(r + 3 * r * i, 300 + r + 4 * r * j, 500, 0, m, q, r));
            particles.push_back(Particle(borderX - 2 * r - 3 * r * i, borderY - 4 * r - 4 * r * j, 0, 0, m, -q, r, sf::Color::Green));
            //particles.push_back(Particle(borderX - 2 * r - 3 * r * i, r + 4 * r * j, -100, 2, 10*m, q, r, sf::Color::Black));
        }
    }
    //particles.push_back(Particle(500, 900, 0, -2, 20, 0, 20));
    //particles.push_back(Particle(500, 300, 0, -5, 20, 0, 20));
    
    //particles.push_back(Particle(500, 900, 0, 0, m, 0, 20));
    //particles.push_back(Particle(500, 300, 0, 0, m, 0, 20, sf::Color::Green));

    //particles.push_back(Particle(400, 100,  11, 0, 1,  50, 5));
    //particles.push_back(Particle(400, 200, -11, 0, 1, -50, 5, sf::Color::Green));
    //particles.push_back(Particle(400, 500, 0, -300, 1, 100, 5));

    //particles.push_back(Particle(400, 100, 0, 300, 1, 50, 20));
    //particles.push_back(Particle(400, 200, 0, -300, 1, -50, 20, sf::Color::Green));
}

void World::createParticle(sf::Vector2i pos) {
}

size_t World::getParticleCount() {
    return particles.size();
}

void World::update() {
    float dt{ 0.005f };
    for (int i = 0; i < getParticleCount(); i++) {
        Particle& particle = particles[i];
        sf::Vector2f pos{ particle.getPosition() };
        sf::Vector2f v{ particle.getVelocity() };
        float m{ particle.getMass() };

        //sf::Vector2f acc_0 = getElectricForce(i, pos) / m;
        //sf::Vector2f acc_1 = getElectricForce(i, pos + v * (dt / 2)) / m;

        particle.move(v * dt + gravity * (dt * dt / 2));
        particle.setVelocity(v + gravity * dt);
        checkAndPerformBoundaryCollision(particle);
    }
    
    grid->updateGrid(particles);

    //performClassicalCollisions();
    performFastClassicalCollisions();
}

sf::Vector2f World::getElectricForce(int i, sf::Vector2f pos) {
    sf::Vector2f force{ 0, 0 };
    
    float q_i        = particles.at(i).getCharge();
    sf::Vector2f r_i = pos;

    for (int j = 0; j < getParticleCount(); j++) {
        if (j == i) {
            continue;
        }
        float q_j = particles.at(j).getCharge();
        sf::Vector2f r_j = particles.at(j).getPosition();
        sf::Vector2f d = r_i - r_j;
        float distance_3 = pow(sqrt(scalar_multiply(d, d) + 0.001), 3);
        force += 10 * q_i * q_j / distance_3 * d;
    }
    return force;
}

void World::checkAndPerformBoundaryCollision(Particle& particle) {
    sf::Vector2f position = particle.getPosition();
    sf::Vector2f velocity = particle.getVelocity();
    float r = particle.getRadius();

    if (position.x - r < 0 or position.x + r > borderX) {
        particle.setVelocity(-velocity.x, velocity.y);
        if (position.x - r < 0) {
            particle.setPosition({ r, position.y });
        }
        else {
            particle.setPosition({ borderX - r, position.y });
        }
    }
    if (position.y - r < 0 or position.y + r > borderY) {
        particle.setVelocity(velocity.x, -velocity.y);
        
        // Update position variable after processing x boundaries
        position = particle.getPosition();
        if (position.y - r < 0) {
            // Correction for additional speed gain above the boundary
            float v = particle.getAbsVelocity();
            particle.setVelocity(particle.getVelocity() + sf::Vector2f{ 0, gravity.y / v * (r - position.y) });
            particle.setPosition({ position.x, r });
        }
        else {
            // Correction for additional speed gain below the boundary
            float v = particle.getAbsVelocity();
            particle.setVelocity(particle.getVelocity() + sf::Vector2f{ 0, gravity.y / v * (position.y - borderY + r) });
            particle.setPosition({ position.x, borderY - r });
        }
    }
}

void World::performClassicalCollisions() {
    std::set<int> processed;
    size_t n = getParticleCount();

    for (size_t i = 0; i < n; i++) {
        for (size_t j = i + 1; j < n; j++) {
            if (processed.find(j) != processed.end()) {
                continue;
            }
            if (performCollision(particles[i], particles[j])){
                processed.insert(i);
                processed.insert(j);
                break;
            }            
        }
    }
}

void World::performFastBoundaryCollisions() {

}

void World::performFastClassicalCollisions() {
    using ParticleIndexPair = std::pair<Particle*, size_t>;
    std::set<int> processed;

    for (size_t i = 0; i < getParticleCount(); i++) {
        if (processed.find(i) != processed.end()) {
            continue;
        }

        std::vector<ParticleIndexPair> neighbor_particles = grid->getNeighborParticles(particles[i], i);

        for (auto& particle_idx : neighbor_particles) {
            if (performCollision(particles[i], *(particle_idx.first))) {
                processed.insert(i);
                processed.insert(particle_idx.second);
                break;
            }
        }
    }
}

float World::getEnergy() {
    float energy{ 0.0f };
    for (auto& particle : particles) {
        float m = particle.getMass();
        energy += 0.5 * m * scalar_multiply(particle.getVelocity(), particle.getVelocity());
        energy += particle.getMass() * gravity.y * (borderY - particle.getPosition().y);
    }
    //for (int i = 0; i < getParticleCount(); i++) {
    //    for (int j = i + 1; j < getParticleCount(); j++) {
    //        float q_i = particles.at(i).getCharge();
    //        float q_j = particles.at(j).getCharge();
    //        sf::Vector2f r_i = particles.at(i).getPosition();
    //        sf::Vector2f r_j = particles.at(j).getPosition();
    //        sf::Vector2f d = r_i - r_j;
    //        energy += 10 * q_i * q_j / sqrt(scalar_multiply(d, d));
    //    }
    //}
    return energy;
}

void World::render(sf::RenderWindow& window) {
    window.clear(sf::Color(0, 0, 255));
    
    for (auto& particle : particles) {
        float r = particle.getRadius();
        sf::CircleShape circle(r);
        circle.setPosition(particle.getPosition() - sf::Vector2f(r, r));
        circle.setFillColor(particle.getColor());

        window.draw(circle);
    }
    window.display();
}

