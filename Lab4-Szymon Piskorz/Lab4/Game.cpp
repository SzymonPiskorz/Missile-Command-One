//@author Szymon Piskorz
//C00242808
//@date 29/11/2018
//Start time: 10:05
//Finished Time: 10:50
//Start Time: 16:05
//Finished Time: 19:00
//Start time: 10:05
//Finished Time: 10:50
//Start time: 17:10
//Finished time 18:00
//Estimated time: 300min
//Actual Time: 315min
//----------------------------------
//
//Known bugs:
//No known bugs
//
//Michael Rainsford Ryan explained how the vertex array and lines work

#include "Game.h"
#include "MyVector2.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

/// <summary>
/// default construcor
/// pass parameters fpr sfml window, setup m_exitGame
/// </summary>
Game::Game() :
	m_window{ sf::VideoMode{ 800, 600, 32 }, "SFML Game" },
	m_exitGame{ false } //when true game will exit
{
	//sets the rand to a random seed
	srand(time(nullptr));

	setupGame(); //sets up all the basics for the game
	setupFontAndText(); // load font 
	setupSprite(); // load texture
}

/// <summary>
/// default destructor for game, nothing to release
/// </summary>
Game::~Game()
{
}


/// <summary>
/// game loop running at 60fps
/// </summary>
void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	sf::Time timePerFrame = sf::seconds(1.f / 60.f); // 60 fps
	while (m_window.isOpen())
	{
		processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // at least 60 fps
			update(timePerFrame); //60 fps
		}
		render(); // as many as possible
	}
}

/// <summary>
/// handle user and system events/ input
/// get key presses/ mouse moves etc. from OS
/// and user :: Don't do game update here
/// </summary>
void Game::processEvents()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (sf::Event::Closed == event.type) // window message
		{
			m_window.close();
		}
		if (sf::Event::KeyPressed == event.type) //user key press
		{
			//checks if escape is clicked
			if (sf::Keyboard::Escape == event.key.code)
			{
				//if it was sets the exit game to true
				m_exitGame = true;
			}
			//Checks if T is pressed
			if (sf::Keyboard::T == event.key.code)
			{
				if (!m_extra) //if extras are false then set them to true
				{
					m_extra = true;
				}
				else //if extras are true then set them to false
				{
					m_extra = false;
				}
			}
		}
		//checks if mouse button is pressed
		if (sf::Event::MouseButtonPressed==event.type)
		{
			//if it is goes to the mouse event function
			mouseEvents(event);
		}
	}
}

/// <summary>
/// Update the game world
/// </summary>
/// <param name="t_deltaTime">time interval per frame</param>
void Game::update(sf::Time t_deltaTime)
{
	//checks if the game is still going or if it is over
	if (m_asteroidPosition.y <= 575)
	{
		//checks if the current laser state is fired
		if (m_currentLaserState == LaserStates::Fired)
		{
			m_currentAltitude = 0; //resets the max altitude
			m_powerBar.setSize(sf::Vector2f(m_currentAltitude, 15.0f)); //draws the power bar after reset
			updateLaser(); //runs the function to update the laser
		}
		//checks if the current laser state is explosion
		else if (m_currentLaserState == LaserStates::Explosion)
		{
			updateExplosion(); //runs the function to update the explosion
			m_collisionDetected = collision(); //runs the function that checks collision and returns an answer
		}
		//checks if the current laser state is ready or explosion
		if (m_currentLaserState == LaserStates::Ready || m_currentLaserState == LaserStates::Explosion)
		{
			calMaxAltitude(); //runs the function to calculate the max altitude
			//checks if the power bar is less then 200
			if (m_powerBar.getSize().x <= 200)
			{
				//draws the power bar every frame
				m_powerBar.setSize(sf::Vector2f(m_currentAltitude, 15.0f));
				m_currentAltitude += 0.5f; //adds the altitude every frame
			}
		}
		//checks if the current asteroid state is settingPosition
		if (m_currentAsteroidState == AsteroidStates::settingPositions)
		{
			launchAsteroid(); //runs the function to set up the path for the asteroid
		}
		//checks if the current asteroid state is incoming
		else if (m_currentAsteroidState == AsteroidStates::incoming)
		{
			updateAsteroid(); //runs the function to update the asteroid
		}
		//checks if the current asteroid state is waiting
		else if (m_currentAsteroidState == AsteroidStates::waiting)
		{
			asteroidCounter(); //runs the function to get the counter
		}
	}
	//checks if game is true
	if (m_exitGame)
	{
		//if it is then it closes the window
		m_window.close();
	}
}

/// <summary>
/// draw the frame and then switch bufers
/// </summary>
void Game::render()
{
	//checks if the asteroid is touching the gound if not it draws everything
	if(m_asteroidPosition.y <= 575)
	{
			m_window.clear();

			if (!m_extra) //if T is not pressed
			{
				m_window.draw(m_ground); //draws the gound
				m_window.draw(m_base); //draws the base
				m_window.draw(m_altitudeText); //draws the altitude text
			}
			else
			{
				m_window.draw(m_background); //draws the background
				m_window.draw(m_baseSprite); //draws a base with a texture
				m_window.draw(m_groundSprite); //draws the ground with a texture
			}

			//checks if the laser has been fired
			if (m_currentLaserState == LaserStates::Fired)
			{
				m_window.draw(m_laser); //draws the laser
			}
			m_window.draw(m_powerBar); //draws the power bar

			//checks if there is no collision and if the timer is finished
			if (m_currentAsteroidState != AsteroidStates::waiting && m_collisionDetected == false)
			{
				m_window.draw(m_asteroid); //draws the asteroid
			}

			//checks if there is an explosion
			if (m_currentLaserState == LaserStates::Explosion)
			{
				m_window.draw(m_explosion); //draws the draws the explosion
			}
			//displays everything on the screen
	}
	else //otherwise it only draws the score and game over text
	{
		m_window.draw(m_scoreText);
		m_window.draw(m_gameoverText);
	}
	m_window.display();
}

/// <summary>
/// load the font and setup the text message for screen
/// </summary>
void Game::setupFontAndText()
{
	//loads the font
	if (!m_ArialBlackfont.loadFromFile("ASSETS\\FONTS\\ariblk.ttf"))
	{
		std::cout << "problem loading arial black font" << std::endl;
	}

	//sets up the score text
	m_scoreText.setFont(m_ArialBlackfont);
	m_scoreText.setString("Score: " + std::to_string(score));
	m_scoreText.setPosition(350.0f, 400.0f);
	m_scoreText.setCharacterSize(28);
	m_scoreText.setFillColor(sf::Color::Red);

	//sets up the game over text
	m_gameoverText.setFont(m_ArialBlackfont);
	m_gameoverText.setString("Game Over");
	m_gameoverText.setPosition(200.0f, 250.0f);
	m_gameoverText.setCharacterSize(80);
	m_gameoverText.setFillColor(sf::Color::Red);

	//sets up the altitude text
	m_altitudeText.setFont(m_ArialBlackfont);
	m_altitudeText.setString("Altitude");
	m_altitudeText.setPosition(0.0f, 580.0f);
	m_altitudeText.setCharacterSize(16);
	m_altitudeText.setFillColor(sf::Color::White);

}

/// <summary>
/// load textures
/// </summary>
void Game::setupSprite()
{
	//sets up the sprte and textures for the base
	m_baseTexture.loadFromFile("ASSETS\\IMAGES\\launcher.png");
	m_baseSprite.setTexture(m_baseTexture);
	m_baseSprite.setPosition(345.0f, 515.0f);
	m_baseSprite.setScale(0.25f, 0.25f);
	m_baseSprite.setRotation(-18.0f);

	//sets up the sprte and textures for the backgound
	m_backgroungTexture.loadFromFile("ASSETS\\IMAGES\\backgroundImage.jpg");
	m_background.setTexture(m_backgroungTexture);
	m_background.setPosition(0.0f, 0.0f);

	//sets up the sprte and textures for the ground
	m_groundTexture.loadFromFile("ASSETS\\IMAGES\\snow-texture.jpg");
	m_groundSprite.setTexture(m_groundTexture);
	m_groundSprite.setPosition(0.0f, 575.0f);
}

void Game::setupGame()
{
	//sets up the colour, size and position of the gound
	m_ground.setFillColor(sf::Color::Green);
	m_ground.setPosition(0.0f, 575.0f);
	m_ground.setSize(sf::Vector2f(800.0f, 25.0f));

	//sets up the colour, size and position of the base
	m_base.setFillColor(sf::Color::Yellow);
	m_base.setPosition(375.0f, 525.0f);
	m_base.setSize(sf::Vector2f(50.0f, 50.0f));

	//sets up the colour, size and position of the power bar
	m_powerBar.setFillColor(sf::Color::Red);
	m_powerBar.setPosition(75.0f, 580.0f);
	m_powerBar.setSize(sf::Vector2f(m_currentAltitude, 15.0f));

	//sets up the colour and position of the laser
	m_laser.append(sf::Vertex{ sf::Vector2f{ 400.0f , 575.0f }, sf::Color::Red });
	m_laser.append(sf::Vertex{ sf::Vector2f{ 400.0f , 575.0f }, sf::Color::Red });

	//sets up the colour and position of asteroid
	m_asteroid.append(sf::Vertex{ sf::Vector2f{ static_cast<float>(rand() % 800)  , 0.0f }, sf::Color::White });
	m_asteroid.append(sf::Vertex{ sf::Vector2f{ static_cast<float>(rand() % 800)  , 575.0f }, sf::Color::White });

	////sets up the colour and size of the explosion
	m_explosion.setFillColor(sf::Color::Red);
	m_explosion.setRadius(1);
}

void Game::mouseEvents(sf::Event t_mouseEvent)
{
	//checks if left mouse button was pressed 
	if (sf::Mouse::Left == t_mouseEvent.mouseButton.button)
	{
		//checks if the laser is ready to fire
		if (m_currentLaserState == LaserStates::Ready)
		{
			//sets the laser finish to the mouse click
			m_laserFinish = sf::Vector2f{ static_cast<float>(t_mouseEvent.mouseButton.x), static_cast<float>(t_mouseEvent.mouseButton.y) };
			//sets the 2 cell of the laser array to the laser finish
			m_laser[1] = sf::Vertex{ m_laserFinish, sf::Color::Red };

			//sets the current laser position to the bottom middle of the base
			m_laserPosition = sf::Vector2f{ 400.0f , 575.0f };

			//sets the laser state to fired
			m_currentLaserState = LaserStates::Fired;
		}
	}

}


void Game::updateLaser()
{
	//checks if the current laser's y position is less more than max altitude and laser's y finish point
	if(m_laserPosition.y >= m_laserFinish.y && m_laserPosition.y >= m_maxAltitude)
	{
		float speed{ 3.0f }; //sets the speed of the laser to 3
		//gets the distance vector between the laser finish and the staring position
		sf::Vector2f distanceVector = m_laserFinish - sf::Vector2f{ 400.0f , 575.0f };
		sf::Vector2f velocity; //sets up a velocity vector

		//velocity is the unit vector of the distance vector mulitplied by the speed 
		velocity = vectorUnitVector(distanceVector)*speed;
		//laser's current position is itself plus the velocity
		m_laserPosition += velocity;

		//sets the 2 cell of the laser array to the laser's current position
		m_laser[1] = sf::Vertex{ m_laserPosition, sf::Color::Red };
	}
	else
	{
		m_radius = 1; //resets the radius of the explosion to 1
		m_currentLaserState = LaserStates::Explosion; //sets the laser state to explosion
		m_explosion.setPosition(m_laserPosition); //sets the explosions position to the laser's current position
		m_explosion.setRadius(m_radius); //sets the explosions radius
		m_explosion.setOrigin(m_radius, m_radius); //sets the origin of the explosion
	}
}

void Game::updateExplosion()
{
	//checks if the explosions current radius is less than max radius
	if (m_radius < MAX_EXPLOSION_RADIUS)
	{
		m_explosion.setRadius(m_radius++); //increses the radius by 1 each frame
		m_explosion.setOrigin(m_radius, m_radius); //sets the origin of the explosion each frame
	}
	else
	{
		//sets the current laser state to ready 
		m_currentLaserState = LaserStates::Ready;
	}
}

void Game::launchAsteroid()
{
	//sets the asteroid's starting position to a random x position
	m_asteroidStart = sf::Vector2f{ static_cast<float>(rand() % 800) , 0 };
	//sets the asteroid's finish position to a random x position
	m_asteroidFinish = sf::Vector2f{ static_cast<float>(rand() % 800) , 575 };
	m_asteroidPosition = m_asteroidStart; //sets the asteroid's current position to the starting position
	m_asteroid[0] = sf::Vertex{ m_asteroidPosition, sf::Color::White };
	//sets the 2 cell of the asteroid array to the asteroid's current position 
	m_asteroid[1] = sf::Vertex{ m_asteroidPosition, sf::Color::White };
	//sets the current asteroid state to incoming
	m_currentAsteroidState = AsteroidStates::incoming;
}

void Game::updateAsteroid()
{
	//checks if there is not collision and if it the asteroids current y position is less then the ground's y position
	if (m_collisionDetected != true && m_asteroidPosition.y <= 575)
	{
		float speed{ 1.0f }; //sets the speed of the asteroid to 1
		//gets the distance vector between the asteroid's finish and the staring position
		sf::Vector2f distanceVector = m_asteroidFinish - m_asteroidStart;
		sf::Vector2f velocity; // sets up a velocity vector

		//velocity is the unit vector of the distance vector mulitplied by the speed 
		velocity = vectorUnitVector(distanceVector)*speed;
		//asteroids current position is itself plus the velocity
		m_asteroidPosition += velocity;

		//sets the 2 cell of the asteroids array to the asteroids's current position
		m_asteroid[1] = sf::Vertex{ m_asteroidPosition, sf::Color::White };
	}
	else
	{
		//sets the asteroid counter to a random value 
		m_asteroidCounter= rand() % 400+1;
		//sets the asteroids current state to waiting
		m_currentAsteroidState = AsteroidStates::waiting;
	}
}

void Game::asteroidCounter()
{
	//checks if the asteroid counter is more than or equal to 0
	if (m_asteroidCounter >= 0)
	{
		//asteroids counter is decremented by the counter decrement
		m_asteroidCounter -= ASTEROID_COUNTER_DECREMENT;
	}
	else
	{
		if (m_collisionDetected == true)
		{
			score += 5; //adds 5 to the score
			m_scoreText.setString("Score: " + std::to_string(score)); //sets the score text
		}
		m_collisionDetected = false;
		//sets the asteroids state to settingPositions 
		m_currentAsteroidState = AsteroidStates::settingPositions;
	}
}

bool Game::collision()
{
	bool collisionDetect{ false }; //bool for the collision 

	//gets the distance vector from asteroids position minus explosion's position 
	sf::Vector2f distanceVector = m_asteroidPosition - m_explosion.getPosition();
	//gets the vector lenght of the distance vector which is the distance between 
	float distanceBetween = vectorLength(distanceVector);

	//checks if the distance between is less than or equal to the radius
	if (distanceBetween <= m_radius)
	{
		collisionDetect= true ; //sets the collision to true
	}
	return collisionDetect;
}

void Game::calMaxAltitude()
{
	//finds the max altitude (y position) for the laser position
	m_maxAltitude = 575.0f - (m_currentAltitude * 2.5); //its multiplied by 2.5 as the max value for current altitude is 200 
}