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
#ifndef GAME
#define GAME

#include <SFML/Graphics.hpp>

const float MAX_EXPLOSION_RADIUS{ 40.0f }; //constant for the radius of the explosion
const int ASTEROID_COUNTER_DECREMENT = 1; //constant for the asteroid counter decrement

//enum class for the different states of the laser
enum class LaserStates
{
	Ready,
	Fired,
	Explosion
};

//enum class for the different states of the asteroid 
enum class AsteroidStates
{
	waiting,
	settingPositions,
	incoming
};

class Game
{
public:
	Game();
	~Game();
	void run();

private:

	void processEvents();
	void update(sf::Time t_deltaTime);
	void render();

	LaserStates m_currentLaserState{ LaserStates::Ready }; //sets the first laser state
	AsteroidStates m_currentAsteroidState{ AsteroidStates::settingPositions }; //sets the first asteroid state

	void mouseEvents(sf::Event t_mouseEvent); //function for mouse click 

	void setupFontAndText(); //function for setting up fonts and text
	void setupSprite(); //function for setting up textures
	void setupGame(); //function for setting up all the things in game 
	void updateLaser(); //function for updating the laser
	void updateAsteroid(); //function for updating the asteroid
	void updateExplosion(); //function for updating the explosion
	void asteroidCounter(); //function for the asteroid counter
	bool collision(); //function for checking collision
	void calMaxAltitude(); //function for calculating the altitude

	sf::RenderWindow m_window; // main SFML window
	sf::Font m_ArialBlackfont; // font used by message
	sf::Text m_welcomeMessage; // text used for message on screen
	sf::Texture m_logoTexture; // texture used for sfml logo
	sf::Sprite m_logoSprite; // sprite used for sfml logo
	bool m_exitGame; // control exiting game
	sf::RectangleShape m_base; //setting up the rectangle shape for base
	sf::RectangleShape m_ground; //setting up a rectangle for the ground
	sf::Text m_scoreText; // text used to show the score
	sf::Text m_gameoverText; //text used to say game over
	sf::Text m_altitudeText; //text to display altitude
	int score = 0; //variable for the score
	sf::RectangleShape m_powerBar; //setting up the shape for the power bar
	float m_currentAltitude{ 0.0f }; //variable for the current altitude
	float m_maxAltitude{ 0.0f }; //variable for the max altitude

	sf::CircleShape m_explosion; //setting up shape for the explosion
	float m_radius{ 1.0f }; //variable for the radius

	bool m_collisionDetected{ false }; //variable for the collision 


	sf::VertexArray m_laser{ sf::Lines }; //setting up the vertex array of lines for the laser
	sf::Vector2f m_laserFinish; //setting the laser's begining position
	sf::Vector2f m_laserPosition; //setting up the laser's current position

	void launchAsteroid(); //function for deciding the start and end point of the asteroid
	sf::Vector2f m_asteroidStart; //setting up the asteroid start position
	sf::VertexArray m_asteroid{ sf::Lines }; //setting up a vertex array of lines for the asteroid
	sf::Vector2f m_asteroidFinish; //setting the asteroids end position
	sf::Vector2f m_asteroidPosition; //setting the asteroid current position
	int m_asteroidCounter = 0; //variable for the asteroid counter

	bool m_extra = false; //bool to control the extras 
	sf::Texture m_backgroungTexture; //Texture used for the backgound
	sf::Texture m_groundTexture; //Texture used for the ground
	sf::Texture m_baseTexture; //Texture used for the base
	sf::Sprite m_background; //sprite for the background
	sf::Sprite m_groundSprite; //sprite for the ground
	sf::Sprite m_baseSprite; //sprite for the base

};

#endif // !GAME

