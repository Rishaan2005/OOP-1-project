#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <cmath>
#include <vector>
#include <random>

//ScreenResulution//
const int W = 1920;
const int H = 1080;

//Controls//
const auto UP = Keyboard::UP;
const auto LEFT = Keyboard::LEFT;
const auto RIGHT = Keyboard::RIGHT;
const auto FIRE = Keyboard::Z;

class GameObject {
public:
    sf::Vector2f position;
    sf::Vector2f velocity;

    GameObject(float x, float y) : position(x, y), velocity(0, 0) {}

    virtual void update(float dt) {
        position += velocity * dt;
    }

    virtual void draw(sf::RenderWindow& window) = 0;
};

class Asteroid : public GameObject {
public:
    sf::CircleShape shape;

    Asteroid(float x, float y) : GameObject(x, y) {
        shape.setRadius(30);
        shape.setFillColor(sf::Color::Transparent);
        shape.setOutlineColor(sf::Color::White);
        shape.setOutlineThickness(2);
        shape.setOrigin(30, 30);
        shape.setPosition(position);

        velocity = sf::Vector2f(50, 50); // floating diagonally
    }

    void update(float dt) override {
        GameObject::update(dt);
        shape.setPosition(position);
    }

    void draw(sf::RenderWindow& window) override {
        window.draw(shape);
    }
};
