#include "header.hpp"

// resetfunctie wanneer men weer wil spelen, het verwijderd alle asteroids en bullets, en reset de spaceship en clocks
void resetGame(Spaceship& spaceship, vector<Bullet>& bullets, vector<Asteroid>& asteroids, Clock& asteroidClock, Clock& bulletClock, Clock& gameClock)
{
	spaceship = Spaceship();
	bullets.clear();
	asteroids.clear();
	asteroidClock.restart();
	bulletClock.restart();
	gameClock.restart();

}

int main()
{
	// declaratie van de schermgrootte, meeste displays zijn minstens 1080p
	// door 720p te nemen, voorkomen we een situatie waarbij de game window niet past op het scherm
	const int WINDOW_LENGTE = 1280;
	const int WINDOW_BREEDTE = 720;

	// game clock, nodig voor tijdsgebonden zaken zoals beweging van de asteroids, bullets, en spaceship
	Clock gameClock;
	Clock bulletClock;
	Clock asteroidClock;


	// declaratie van de game window met de gekozen dimensies
	RenderWindow gameWindow(VideoMode({ WINDOW_LENGTE, WINDOW_BREEDTE }), "Blasteroids", Style::Default);

	// achtergrond texture declaratie, indien de foto gevonden wordt dan runt de game niet
	// zonder de foto wordt de game experience geruined
	Texture backgroundTexture;
	if (!backgroundTexture.loadFromFile("background.png"))
	{
		cout << "Achtergrond texture foto niet gevonden." << endl;
		return -1;
	}

	// achtergrond texture geplaatst
	Sprite backgroundSprite(backgroundTexture);
	backgroundSprite.setTexture(backgroundTexture);

	// declaratie van de start screen button en start screen state 
	bool onStartScreen = true;
	RectangleShape startButton(Vector2f(300, 100));
	startButton.setFillColor(Color::Transparent);
	startButton.setPosition(WINDOW_LENGTE / 2 - 150, WINDOW_BREEDTE / 2 - 50); // button in het midden van het scherm

	// font voor de game, essentieel voor de game experience
	Font gameFont;
	if (!gameFont.loadFromFile("PressStart2P-Regular.ttf"))
	{
		cout << "Font niet gevonden." << endl;
		return -1;
	}

	// initialisatie van de start button text
	Text startText("START", gameFont, 48);
	startText.setFillColor(Color::White);
	FloatRect textRect = startText.getLocalBounds();
	startText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	startText.setPosition(WINDOW_LENGTE / 2, WINDOW_BREEDTE / 2);

	// declaratie van de game over screen state en game over text
	bool gameOver = false;
	RectangleShape gameOverButton(Vector2f(300, 100));
	gameOverButton.setFillColor(Color::Transparent);
	gameOverButton.setPosition(WINDOW_LENGTE / 2 - 150, WINDOW_BREEDTE / 2 - 50); // button in het midden van het scherm

	Text gameOverText("GAME OVER", gameFont, 48);
	gameOverText.setFillColor(Color::White);
	FloatRect gameOverTextRect = gameOverText.getLocalBounds();
	gameOverText.setOrigin(gameOverTextRect.left + gameOverTextRect.width / 2.0f, gameOverTextRect.top + gameOverTextRect.height / 2.0f);
	gameOverText.setPosition(WINDOW_LENGTE / 2, WINDOW_BREEDTE / 2);

	Text scoreText("Score:0", gameFont, 24);
	scoreText.setFillColor(Color::White);
	scoreText.setPosition(20, 20);

	Text scoreNumber;
	int score = 0; // score voor de game, wordt verhoogd bij het vernietigen van een asteroid
	int lastJingleScore = 0; // om te voorkomen dat de jingle te vaak wordt afgespeeld

	// muziek declaratie en initialisatie, want het voegt veel toe aan de game experience
	Music backgroundMusic;
	if (!backgroundMusic.openFromFile("backgroundmusic.ogg"))
	{
		cout << "Achtergrond muziek niet gevonden." << endl;
		return -1;
	}
	backgroundMusic.setLoop(true); // muziek moet in een loop spelen
	backgroundMusic.setVolume(20); // volume van de muziek, kan aangepast worden naar wens
	backgroundMusic.play();

	// sound fx declaratie en initialisatie, ook voor de game experience
	SoundBuffer jingle;
	if (!jingle.loadFromFile("jingle.wav"))
	{
		cout << "Jingle sound effect niet gevonden." << endl;
		return -1;
	}
	Sound jingleSound;
	jingleSound.setBuffer(jingle);

	// tweede sound fx voor de projectile (bullets)
	SoundBuffer laserProjectile;
	if (!laserProjectile.loadFromFile("laser_shot.wav"))
	{
		cout << "Bullet sound effect niet gevonden." << endl;
		return -1;
	}
	Sound bulletSound;
	bulletSound.setBuffer(laserProjectile);
	bulletSound.setVolume(60); // volume van het projectiel sound fx

	// derde sound fx voor de asteroid destruction
	SoundBuffer explosion;
	if (!explosion.loadFromFile("explosion.wav"))
	{
		cout << "Asteroid destruction sound effect niet gevonden." << endl;
		return -1;
	}
	Sound asteroidDestroyed;
	asteroidDestroyed.setBuffer(explosion);
	asteroidDestroyed.setVolume(20); // volume van de sound effect

	// vierde sound fx voor de score jingle
	SoundBuffer scoreJingle;
	if (!scoreJingle.loadFromFile("score_jingle.wav"))
	{
		cout << "Score jingle sound effect niet gevonden." << endl;
		return -1;
	}
	Sound scoreJingleSound;
	scoreJingleSound.setBuffer(scoreJingle);
	scoreJingleSound.setVolume(40); // volume van de score jingle sound effect

	// declaratie van de spaceship, bullets en asteroid objecten
	Spaceship spaceship;
	vector<Bullet> bullets;
	vector<Asteroid> asteroids;

	// creatie van de game loop
	while (gameWindow.isOpen())
	{
		float deltaTime = gameClock.restart().asSeconds(); // tijd sinds de laatste frame, gebruikt voor de asteroids, bullets en spaceship beweging

		// Event handling voor de game window
		Event gameRunning;
		while (gameWindow.pollEvent(gameRunning))
		{
			if (gameRunning.type == Event::Closed)
			{
				gameWindow.close();
			}

			if (onStartScreen)
			{
				// hier wordt gekeken of de start button is geklikt of of de spacebar is ingedrukt
				if ((gameRunning.type == Event::MouseButtonPressed && gameRunning.mouseButton.button == Mouse::Left &&
					startButton.getGlobalBounds().contains(Vector2f(gameRunning.mouseButton.x, gameRunning.mouseButton.y)))
					|| (gameRunning.type == Event::KeyPressed && gameRunning.key.code == Keyboard::Space))
				{
					onStartScreen = false;
					jingleSound.play();
					resetGame(spaceship, bullets, asteroids, asteroidClock, bulletClock, gameClock);
					score = 0; // Ensure score is reset when starting a new game
					lastJingleScore = 0;
					scoreText.setString("Score:0");
				}
			}
			else if (gameOver) // indien game over is, dan wordt de game over screen getoond
			{
				// hier wordt gekeken of de game over button is geklikt of of de spacebar is ingedrukt
				if ((gameRunning.type == Event::MouseButtonPressed && gameRunning.mouseButton.button == Mouse::Left &&
					gameOverButton.getGlobalBounds().contains(Vector2f(gameRunning.mouseButton.x, gameRunning.mouseButton.y)))
					|| (gameRunning.type == Event::KeyPressed && gameRunning.key.code == Keyboard::Space)
					)
				{
					gameOver = false;
					onStartScreen = true; // terug naar het start scherm
					score = 0;
					lastJingleScore = 0; // reset de score en last jingle score
					scoreText.setString("Score:0");
					resetGame(spaceship, bullets, asteroids, asteroidClock, bulletClock, gameClock);
				}
			}
		}

		gameWindow.clear(Color::Black);

		if (onStartScreen)
		{
			// game logica voor het start scherm
			gameWindow.draw(backgroundSprite);
			gameWindow.draw(startButton);
			gameWindow.draw(startText);
		}
		else if (gameOver) // indien game over is, dan wordt de game over screen getoond
		{
			gameWindow.draw(backgroundSprite);
			gameWindow.draw(gameOverText);
			gameWindow.draw(gameOverButton);
		}
		else
		{
			// vanaf hier begint de game logica voor het spel zelf

			// teken de achtergrond
			gameWindow.draw(backgroundSprite);

			// spaceship
			spaceship.handleInput();
			spaceship.updateShip(deltaTime);
			spaceship.drawShip(gameWindow);

			// bullets
			if (bulletClock.getElapsedTime().asSeconds() >= 0.8f) // de bullets worden per 0.8 seconden ageschoten en vanaf het bovenkant van de spaceship worden ze getekend en geupdate
			{
				Vector2f shipPos = spaceship.getPosition();
				Vector2f shipSize = spaceship.getSize();
				bullets.emplace_back(shipPos.x + shipSize.x / 2 - 2.5f, shipPos.y);
				bulletSound.play(); // speel de bullet sound effect af bij het schieten
				bulletClock.restart();
			}

			for (auto& bullet : bullets)
			{
				bullet.updateBullet(deltaTime);
				bullet.drawBullet(gameWindow);
			}
			

			// asteroids
			if (asteroidClock.getElapsedTime().asSeconds() >= 1.0f)
			{
				// gebruik van <random> zodat de asteroids willekeurig worden gespawned
				random_device rd;
				mt19937 gen(rd());
				uniform_int_distribution<> asteroidCountDistribution(3, 10);
				uniform_real_distribution<> asteroidPosDistribution(-340.0f, 340.0f); // willekeurige positie van de asteroids tussen -340 en 340

				int count = asteroidCountDistribution(gen);
				vector<float> positions;

				// ze moeten niet overlappen, anders kan het lastig worden voor de player
				for (int i = 0; i < count; ++i) {
					float pos;
					bool validPosition;
					int attempts = 0;

					do {
						validPosition = true;
						pos = 640.0f + asteroidPosDistribution(gen);

						// hier wordt gekeken of de positie van de asteroid niet te dicht bij een andere asteroid is
						for (float existingPos : positions) {
							if (abs(pos - existingPos) < 55.0f)
							{
								validPosition = false;
								break;
							}
						}
						attempts++;
					} while (!validPosition && attempts < 50);

					if (validPosition) {
						positions.push_back(pos);
						asteroids.emplace_back(pos, 0.0f);
					}
				}

				// reset de clock zodat de asteroids niet te snel worden gespawned
				asteroidClock.restart();
			}

			// update en teken de asteroids
			for (auto& asteroid : asteroids)
			{
				asteroid.updateAsteroid(deltaTime);
				asteroid.drawAsteroid(gameWindow);
			}

			// draw Score
			gameWindow.draw(scoreText);
			 

			// Collision checking

			// tussen asteroids en bullets
			for (auto& bullet : bullets)
			{
				if (!bullet.isBulletActive()) continue;

				for (auto& asteroid : asteroids)
				{
					if (!asteroid.isActive()) continue;

					if (bullet.getBounds().intersects(asteroid.getBounds()))
					{
						bullet.setBulletInactive();
						asteroid.setasDestroyed();
						asteroidDestroyed.play(); // speel de sound effect af bij het vernietigen van een asteroid

						// +1 toevoegen aan de score
						score++;
						scoreText.setString("Score:" + to_string(score));
						break; // break de loop om te voorkomen dat we dezelfde bullet meerdere keren verwerken
					}
				}
			}

			// tussen asteroids en spaceship
			for (auto& asteroid : asteroids)
			{
				if (!asteroid.isActive()) continue;

				if (spaceship.getBounds().intersects(asteroid.getBounds()))
				{
					// Game Over
					gameOver = true;
					break; // break de loop om te voorkomen dat we dezelfde asteroid meerdere keren verwerken

				}
			}

			// inactieve game objecten verwijderen
			bullets.erase(remove_if(bullets.begin(), bullets.end(),
				[](const Bullet& bullet) { return !bullet.isBulletActive(); }), bullets.end());

			asteroids.erase(remove_if(asteroids.begin(), asteroids.end(),
				[](const Asteroid& asteroid) { return !asteroid.isActive(); }), asteroids.end());

		}

		if (score != 0 && score % 10 == 0 && score != lastJingleScore)
		{
			scoreJingleSound.play(); // speel de score jingle af bij het behalen van een score die een veelvoud is van 10 (dit is voor de game experience)
			lastJingleScore = score; // update de last jingle score zodat de jingle niet te vaak wordt afgespeeld
		}

		gameWindow.display();

	}

	return 0;
}