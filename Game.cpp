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
    sf::Sprite sprite;
    sf::Texture texture;

    Asteroid(float x, float y) : GameObject(x, y) {
        texture.loadFromFile("asteroid.png"); 
        sprite.setTexture(texture);

        
        sprite.setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);
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
