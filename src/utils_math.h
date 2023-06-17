#pragma once

inline float scalar_multiply(const sf::Vector2f& left, const sf::Vector2f& right) {
    return left.x * right.x + left.y * right.y;
}
