#include "header.hpp"

// hier komt de implementatie van de diverse classes en member functies


// van de Asteroid class
Asteroid :: Asteroid(float x, float y) : destroyed(false)
{
	asteroidShape.setSize(Vector2f(55, 50)); // grootte van de asteroïde
	asteroidShape.setPosition(x, y);

	// Texture laden voor de asteroid
	if (!textureLoaded)
	{
		if (asteroidTexture.loadFromFile("asteroid.png"))
		{
			textureLoaded = true;
		}
		else
		{
			cout << "Asteroid texture foto niet gevonden" << endl;
			asteroidShape.setFillColor(Color::Red);
			exit(-1);
		}
	}
	asteroidShape.setTexture(&asteroidTexture); // texture geplaatst op de asteroid vorm	

	asteroidVelocity = Vector2f(0, 250); // beweegt naar beneden met een vrij grote snelheid
	// dit zorgt voor een uitdagende game experience
}

void Asteroid :: updateAsteroid(float deltaTime)
{
	if (!destroyed)
	{
		asteroidShape.move(asteroidVelocity * deltaTime); // beweegt de asteroïde
		if (asteroidShape.getPosition().y > 720)
		{
			destroyed = true; // als de asteroid onder het scherm gaat, wordt het gemarkeerd als destroyed
		}
	}
}

FloatRect Asteroid :: getBounds() const
{
	return asteroidShape.getGlobalBounds(); // geeft de grenzen van de asteroid shape terug
}

bool Asteroid :: isActive() const
{
	return !destroyed; // geeft true terug als de asteroïde nog actief is
}

void Asteroid :: setasDestroyed()
{
	destroyed = true; // markeert de asteroïde als destroyed
}

void Asteroid::drawAsteroid(RenderWindow& gameWindow)
{
	if(!destroyed)
	{
		gameWindow.draw(asteroidShape); // tekent de asteroïde op het scherm als het nog actief is
	}
}


Texture Asteroid::asteroidTexture;
bool Asteroid::textureLoaded = false; // initialisatie van de static member

// van de Bullet class
Bullet :: Bullet(float x, float y) : bulletActive(true)
{
	bulletShape.setSize(Vector2f(7, 20)); // grootte van de kogel
	bulletShape.setPosition(x, y);
	bulletShape.setFillColor(Color::Yellow); // kleur van de kogel
	bulletVelocity = Vector2f(0, -450); // beweegt naar boven met een hoge snelheid
}

void Bullet :: updateBullet(float deltaTime)
{
	if (bulletActive)
	{
		bulletShape.move(bulletVelocity * deltaTime); // beweegt de kogel
		if (bulletShape.getPosition().y < 0)
		{
			bulletActive = false; // als de kogel boven het scherm gaat, wordt het gemarkeerd als inactive
		}
	}
}

FloatRect Bullet :: getBounds() const
{
	return bulletShape.getGlobalBounds(); // geeft de grenzen van de kogel terug
}

bool Bullet :: isBulletActive() const
{
	return bulletActive; // geeft true terug als de kogel nog actief is
}

void Bullet :: setBulletInactive()
{
	bulletActive = false; // markeert de kogel als inactive
}

void Bullet :: drawBullet(RenderWindow& gameWindow)
{
	if (bulletActive)
	{
		gameWindow.draw(bulletShape); // tekent de kogel op het scherm als het actief is
	}
}

// van de Spaceship class
Spaceship::Spaceship() : speed(300.0f)
{
	spaceshipShape.setSize(Vector2f(60, 40)); // grootte van het ruimteschip
	spaceshipShape.setPosition(640 - 20, 720 - 100); // zodat de ship in het midden van de onderkant van het scherm staat
	
	// Texture laden voor de ship, ook nodig voor de game experience
	if (!spaceshipTexture.loadFromFile("spaceship.png"))
	{
		cout << "Ruimteschip texture niet gevonden." << endl;
		exit(-1);
	}
	spaceshipShape.setTexture(&spaceshipTexture);
	spaceshipVelocity = Vector2f(0, 0); // begint met een snelheid van 0
}

void Spaceship::handleInput()
{
	spaceshipVelocity.x = 0; // reset de x snelheid

	// input handling voor de ship, met de pijltjestoetsen of A en D toetsen
    if (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A))
    {
        spaceshipVelocity.x = -speed; // beweegt naar links
    }

    if (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D))
    {
        spaceshipVelocity.x = speed; // beweegt naar rechts
    }
}

void Spaceship::updateShip(float deltaTime)
{
	spaceshipShape.move(spaceshipVelocity * deltaTime); // beweegt het ruimteschip

	// de ship moet binnen een bepaald gebied blijven, dit zorgt voor een challenging game experience >:)
	float leftBound = 640 - 320;
	float rightBound = 640 + 320 - spaceshipShape.getSize().x;\
	// deze waarden zijn gebaseerd op de schermgrootte en de grootte van de ship
	
		if(spaceshipShape.getPosition().x < leftBound)
		{
			spaceshipShape.setPosition(leftBound, spaceshipShape.getPosition().y); // houdt de ship binnen de linkerkant van het scherm
		}
		if (spaceshipShape.getPosition().x > rightBound)
		{
			spaceshipShape.setPosition(rightBound, spaceshipShape.getPosition().y); // houdt de ship binnen de rechterkant van het scherm
		}
}

void Spaceship::drawShip(RenderWindow& gameWindow)
{
	gameWindow.draw(spaceshipShape); // tekent de ship op het scherm
}

Vector2f Spaceship::getPosition() const
{
	return spaceshipShape.getPosition(); // geeft de huidige positie van de ship terug
}

FloatRect Spaceship::getBounds() const
{
	return spaceshipShape.getGlobalBounds(); // geeft de grenzen van de ship terug
}

Vector2f Spaceship:: getSize() const
{
	return spaceshipShape.getSize(); // geeft de grootte van de ship terug
}

Texture Spaceship::spaceshipTexture; // initialisatie van de static member

