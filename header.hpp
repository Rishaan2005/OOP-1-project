#pragma once

// de benodigde include statements; networking library weggelaten omdat deze game geen network zal accessen
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <iostream> // iostream voor eventuele errors door het niet vinden van bestanden
#include <vector>
#include <random> // voor random getallen, gebruikt voor het spawnen van asteroids
#include <cmath>

// gebruik van namespace om de code leesbaarder te maken en zodat er minder getypt hoeft te worden
using namespace sf;
using namespace std;

/*
De game is een spaceship shooter game, inspired by classic games like Asteroids and Space Invaders. Het verschil ligt in de game experience. Nieuwe sound fx, nieuwe graphics,
en een meer challenging gameplay. Als je een keer door een asteroid wordt geraakt, dan is het game over. Er zijn twee manieren om te surviven, door de asteroids te ontwijken of door ze te vernietigen
met de bullets die automatisch worden afgevuurd. Voor elke asteroid die wordt vernietigd, krijg je 1 punt. De game heeft geen einde, dus men kan proberen een high score te krijgen of iemand anders
high score te beaten. 

Wij hebben 3 classes gemaakt: Asteroid, Bullet, en Spaceship. Deze classes zijn verantwoordelijk voor de asteroids, bullets, en het ruimteschip. Voor elke class zijn er data members voor de
vorm, snelheid, en status van het object. Daarnaast zijn er member functies voor het updaten van de objecten, het tekenen van de objecten, en het controleren van de status van de objecten (voor o.a collision checking).
Ze zijn essentieel voor de goede werking van de game en de game experience.
*/

class Asteroid
{
private:
	RectangleShape asteroidShape;
	static Texture asteroidTexture;
	static bool textureLoaded; // om te voorkomen dat de texture meerdere keren wordt geladen
	Vector2f asteroidVelocity; // snelheid van de asteroïde
	bool destroyed;

public:
	Asteroid(float x, float y);
	void updateAsteroid(float deltaTime);
	FloatRect getBounds() const;
	bool isActive() const;
	void setasDestroyed();
	void drawAsteroid(RenderWindow& gameWindow);
};

class Bullet
{
private:
	RectangleShape bulletShape;
	Vector2f bulletVelocity; // snelheid van de kogel
	bool bulletActive;

public:
	Bullet(float x, float y);
	void updateBullet(float deltaTime);
	FloatRect getBounds() const;
	bool isBulletActive() const;
	void setBulletInactive();
	void drawBullet(RenderWindow& gameWindow);
};

class Spaceship
{
private:
	RectangleShape spaceshipShape;
	static Texture spaceshipTexture; // zorgt ervoor dat de spaceship een texture heeft, anders is het een witte rechthoek
	Vector2f spaceshipVelocity; // snelheid van het ruimteschip
	float speed; // snelheid van de ship

public:
	Spaceship();
	void handleInput();
	void updateShip(float deltaTime);
	void drawShip(RenderWindow& gameWindow);
	Vector2f getPosition() const;
	FloatRect getBounds() const;
	Vector2f getSize() const;
};


