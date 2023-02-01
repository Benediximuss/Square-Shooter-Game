#pragma once

#ifndef GAME_H
#define GAME_H

struct Target
{
	sf::RectangleShape shape;
	int type;
	float speed;
};

class Game
{
public:
	// Constructor & Destructor
	Game();
	virtual ~Game();

	// Accessors
	const bool isGameRunning() const;
	
	// Public Engine Functions
	void update();
	void render();

private:
	// Engine Variables
	sf::VideoMode videoMode;
	sf::RenderWindow* window;
	sf::Event ev;

	// Mouse Positions
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;
	bool mouseHeld;

	// Resources
	sf::Font mainFont;

	// HUD Elements
	sf::RectangleShape HUDtopBar;

	// Skills HUD
	std::vector<sf::CircleShape> blastHUD;
	std::vector<sf::CircleShape> freezeHUD;

	// UI Text
	sf::Text uiText;
	sf::Text skillsText;
	sf::Text blastText;
	sf::Text freezeText;
	sf::Text gameOverText;

	// Initializers
	void initVariables();
	void initWindow();
	void initHUD();
	void initTexts();

	// Private Engine Functions
	void pollEvents();
	void updateMousePos();
	void updateHUD();

	// Render
	void renderEnemies();
	void renderHUD();

	// Game Logic
	unsigned int points;
	float enemySpawnTimer;
	float enemySpawnTimerMax;
	int maxEnemies;
	int health;

	// Skills
	unsigned int blastPoints;
	unsigned int freezePoints;

	void blastSkill();
	void freezeSkill();

	const int freezeTime;
	bool isFrozen;
	int freezeTimer;

	// Game Objects
	std::vector<Target> enemies;

	// Game Functions
	void spawnEnemy();
	void updateEnemies();

};

#endif

