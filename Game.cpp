#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream> 
#include <cmath>
#include <vector>
#include <random>

// Screen Resolution
const int W = 1920;
const int H = 1080;

// Controls
const sf::Keyboard::Key UP = sf::Keyboard::Up;
const sf::Keyboard::Key LEFT = sf::Keyboard::Left;
const sf::Keyboard::Key RIGHT = sf::Keyboard::Right;
const sf::Keyboard::Key FIRE = sf::Keyboard::Z;

// Asteroid sprite constants
const int FRAME_WIDTH = 51;
const int FRAME_HEIGHT = 157;
const int FRAME_INDEX = 2;

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
    sf::Sprite sprite;
    sf::Texture texture;

    Asteroid(float x, float y) : GameObject(x, y) {
        if (!texture.loadFromFile("/mnt/data/PC Computer - Wild Dogs - Asteroid.png")) {
            std::cerr << "Error loading asteroid sprite sheet!" << std::endl;
        }

        sprite.setTexture(texture);
        sprite.setTextureRect(sf::IntRect(FRAME_WIDTH * FRAME_INDEX, 0, FRAME_WIDTH, FRAME_HEIGHT));
        sprite.setOrigin(FRAME_WIDTH / 2.0f, FRAME_HEIGHT / 2.0f);
        sprite.setPosition(position);

        velocity = sf::Vector2f(50, 50); 
    }

    void update(float dt) override {
        GameObject::update(dt);
        sprite.setPosition(position);
    }

    void draw(sf::RenderWindow& window) override {
        window.draw(sprite);
    }
};
