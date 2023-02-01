#include "stdafx.h"
#include "Game.h"

Game::Game()
	: freezeTime(60)
{
	this->initVariables();
	this->initWindow();
	this->initHUD();
}

Game::~Game()
{
	delete this->window;
}

void Game::initVariables()
{
	this->window = nullptr;

	this->videoMode.height = 600;
	this->videoMode.width = 800;

	mouseHeld = false;

	this->points = 0;
	this->enemySpawnTimer = 0.f;
	this->enemySpawnTimerMax = 5.f;
	this->maxEnemies = 10;
	this->health = 10;

	this->blastPoints = 0;
	this->freezePoints = 0;

	this->isFrozen = false;
	this->freezeTimer = 0;

	if (!this->mainFont.loadFromFile("Fonts/BRLNSR.TTF"))
		std::cout << "ERROR: Failed to load font!" << std::endl;

	srand(time(NULL));
}

void Game::initWindow()
{
	this->window = new sf::RenderWindow(this->videoMode, "My first game", sf::Style::Titlebar | sf::Style::Close);
	this->window->setFramerateLimit(60);
}

void Game::initHUD()
{
	// init hud top bar
	this->HUDtopBar.setPosition(0.f, 0.f);
	this->HUDtopBar.setSize(sf::Vector2f(this->videoMode.width, 95.f));
	this->HUDtopBar.setFillColor(sf::Color(150, 0, 0, 255));

	// init skills hud
	int k = 0;
	while (k < 3)
	{
		sf::CircleShape temp;
		temp.setPosition(500.f + k*45, 11.f);
		temp.setRadius(15.f);
		temp.setFillColor(sf::Color(100, 100, 100, 255));
		temp.setOutlineThickness(3.f);

		this->blastHUD.push_back(temp);
		k++;
	}

	k = 0;
	while (k < 3)
	{
		sf::CircleShape temp;
		temp.setPosition(500.f + k * 45, 56.f);
		temp.setRadius(15.f);
		temp.setFillColor(sf::Color(100, 100, 100, 255));
		temp.setOutlineThickness(3.f);

		this->freezeHUD.push_back(temp);
		k++;
	}

	this->initTexts();

}

void Game::initTexts()
{
	// init points and health text
	this->uiText.setFont(this->mainFont);
	this->uiText.setCharacterSize(40);
	this->uiText.setFillColor(sf::Color::White);
	this->uiText.setString("NONE");
	this->uiText.setPosition(0.f, 0.f);

	// init skills text
	this->skillsText.setFont(this->mainFont);
	this->skillsText.setCharacterSize(40);
	this->skillsText.setFillColor(sf::Color::White);
	this->skillsText.setString("  BLAST:\nFREEZE:");
	this->skillsText.setPosition(350.f, 0.f);

	// init press skill text
	this->blastText.setFont(this->mainFont);
	this->blastText.setCharacterSize(40);
	this->blastText.setFillColor(this->HUDtopBar.getFillColor());
	this->blastText.setString("PRESS Q!");
	this->blastText.setPosition(640.f, 0.f);

	this->freezeText.setFont(this->mainFont);
	this->freezeText.setCharacterSize(40);
	this->freezeText.setFillColor(this->HUDtopBar.getFillColor());
	this->freezeText.setString("PRESS W!");
	this->freezeText.setPosition(640.f, 45.f);

	this->gameOverText.setFont(this->mainFont);
	this->gameOverText.setCharacterSize(40);
	this->gameOverText.setFillColor(sf::Color::Red);
	this->gameOverText.setString("GAME OVER!");
	this->gameOverText.setPosition(300.f, 400.f);
	this->gameOverText.setOutlineThickness(1.f);
	this->gameOverText.setOutlineColor(sf::Color::White);

}

const bool Game::isGameRunning() const
{
	return this->window->isOpen();
}

void Game::pollEvents()
{
	// Event polling
	while (this->window->pollEvent(this->ev))
	{
		switch (this->ev.type)
		{
		case sf::Event::Closed:
			this->window->close();
			break;
		case sf::Event::KeyPressed:
			if (this->ev.key.code == sf::Keyboard::Escape)
				this->window->close();
			else if (this->health > 0)
			{
				if (this->ev.key.code == sf::Keyboard::E && this->blastPoints < 3)
				{ 
					this->blastHUD[this->blastPoints].setFillColor(sf::Color(247, 176, 44, 255));
					this->blastPoints += 1;
				}
				else if (this->ev.key.code == sf::Keyboard::R)
				{
					this->freezeHUD[this->freezePoints].setFillColor(sf::Color(36, 243, 236, 255));
					this->freezePoints += 1;
				}
				else if (this->ev.key.code == sf::Keyboard::Q && this->blastPoints == 3)
					this->blastSkill();
				else if (this->ev.key.code == sf::Keyboard::W && this->freezePoints == 3)
					this->freezeSkill();
				break;
			}
		case sf::Event::MouseButtonPressed:
			if (this->ev.key.code == sf::Mouse::Left)
			{
				std::cout << "Mouse pos: " << this->mousePosWindow.x << ","
				<< this->mousePosWindow.y << std::endl;
			}
		}
	}
}

void Game::updateMousePos()
{
	this->mousePosWindow = sf::Mouse::getPosition(*this->window);
	this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
}

void Game::updateHUD()
{
	std::stringstream ss;

	ss << "Points: " << this->points << std::endl << "Health: " << this->health;
	
	this->uiText.setString(ss.str());

	if (this->blastPoints == 3)
		this->blastText.setFillColor(sf::Color::Yellow);
	else
		this->blastText.setFillColor(this->HUDtopBar.getFillColor());

	if (this->freezePoints == 3)
		this->freezeText.setFillColor(sf::Color::Cyan);

}

void Game::blastSkill()
{
	std::cout << "BLAST used!\n";
	this->blastPoints = 0;
	this->points += this->enemies.size();
	this->enemies.clear();

	for (auto &i : this->blastHUD)
	{
		i.setFillColor(sf::Color(100, 100, 100, 255));
	}
}

void Game::freezeSkill()
{
	std::cout << "FREEZE used!\n";
	this->freezePoints = 0;
	this->isFrozen = true;
	this->freezeTimer = 0;
	this->freezeText.setString("FROZEN!");

	for (auto& i : this->freezeHUD)
	{
		i.setFillColor(sf::Color(100, 100, 100, 255));
	}
}

void Game::spawnEnemy()
{
	// Create New Enemy
	Target newEnemy;

	// Generating enemy type
	// 5/7 = Regular Enemy Chance, 1/7 = Blast Enemy Chance, 1/7 = Freeze Enemy Chance
	int newEnemyType = rand() % 20;

	if (newEnemyType == 15)
	{
		newEnemy.type = 3;
		newEnemy.shape.setFillColor(sf::Color(247, 176, 44, 255)); // blast point
		newEnemy.speed = 5;
		newEnemy.shape.setSize(sf::Vector2f(50, 50));
	}
	else if (newEnemyType == 5)
	{
		newEnemy.type = 2;
		newEnemy.shape.setFillColor(sf::Color(36, 243, 236, 255)); // freeze point
		newEnemy.speed = 5;
		newEnemy.shape.setSize(sf::Vector2f(50, 50));
	}
	else
	{
		newEnemy.type = 1;
		newEnemy.shape.setFillColor(sf::Color::Magenta);
		newEnemy.speed = rand() % 3 + 1;
	}

	// Generate random properties for new enemy
	// Size range: [50,100]
	// Position range: [0,WindowWidth - Size - Thickness] [0,WindowHeight - Size - Thickness]
	// Color range: [0,255] [0,255] [0,255]
	// Speed range: [1,5]

	bool isPosValid = false;

	// Keep generating random size and position until new shape does not overlaps with others
	while (!isPosValid && newEnemy.type == 1)
	{
		float randomSize = rand() % 51 + 50;
		newEnemy.shape.setSize(sf::Vector2f(randomSize, randomSize));

		newEnemy.shape.setPosition(rand() % (this->videoMode.width + 1 - (int)randomSize - 3),
			3 /* rand() % (this->videoMode.height + 1 - (int)randomSize -3)*/);
		isPosValid = true;

		for (auto& i : this->enemies)
		{
			if (newEnemy.shape.getGlobalBounds().intersects(i.shape.getGlobalBounds()))
			{
				isPosValid = false;
				break;
			}
		}
	}

	// Constant properties
	newEnemy.shape.setOutlineColor(sf::Color::White);
	newEnemy.shape.setOutlineThickness(3.f);

	// Add enemy to the enemies vector
	this->enemies.push_back(newEnemy);
}

void Game::updateEnemies()
{
	if (this->isFrozen == false)
	{
		// Enemy spawn timer
		if (this->enemies.size() < this->maxEnemies)
		{
			if (this->enemySpawnTimer >= this->enemySpawnTimerMax)
			{
				this->spawnEnemy();
				this->enemySpawnTimer = 0.f;
			}
			else
			{
				this->enemySpawnTimer += 1.f;
			}
		}

		// Updating enemy positions
		for (int i = 0; i < this->enemies.size(); i++)
		{
			// Move enemies downwards
			this->enemies[i].shape.move(0.f, this->enemies[i].speed);

			// If enemy dropped out of screen delete them from vector
			if (this->enemies[i].shape.getPosition().y >= this->videoMode.height + 3)
			{
				this->enemies.erase(this->enemies.begin() + i);
				i--;

				this->health -= 1;
			}
		}
	}
	else
	{
		if (this->freezeTimer < this->freezeTime)
		{
			this->freezeTimer++;
		}
		else
		{
			this->freezeTimer = 0;
			this->isFrozen = false;
			this->freezeText.setString("PRESS W!");
			this->freezeText.setFillColor(this->HUDtopBar.getFillColor());
		}
	}

	// If mouse clicked
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		// Check if left click held
		if (this->mouseHeld == false)
		{
			this->mouseHeld = true;

			// If not clicked on HUD
			if (this->mousePosView.y > 95)
			{
				// Check if the clicked position shoots an enemy
				for (int i = this->enemies.size() - 1; i > -1; i--)
				{
					if (this->enemies[i].shape.getGlobalBounds().contains(this->mousePosView))
					{

						if (this->enemies[i].type == 3 && this->blastPoints < 3) // shoot blast square
						{
							this->blastHUD[this->blastPoints].setFillColor(sf::Color(247, 176, 44, 255));
							this->blastPoints++;
						}

						else if (this->enemies[i].type == 2 && this->freezePoints < 3) // shoot freeze square
						{
							this->freezeHUD[this->freezePoints].setFillColor(sf::Color(36, 243, 236, 255));
							this->freezePoints++;
						}

						this->enemies.erase(this->enemies.begin() + i);
						i++;
						this->points += 1;
						break;
					}
				}
			}
		}
	}
	else
	{
		this->mouseHeld = false;
	}
}

void Game::renderEnemies()
{
	// Render all enemies in vector
	for (auto &i : this->enemies)
	{
		this->window->draw(i.shape);
	}
}

void Game::renderHUD()
{
	this->window->draw(this->HUDtopBar);
	this->window->draw(this->uiText);
	this->window->draw(this->skillsText);
	this->window->draw(this->blastText);
	this->window->draw(this->freezeText);

	if (this->health == 0)
	{
		this->window->draw(this->gameOverText);
	}


	for (auto &i : this->blastHUD)
	{
		this->window->draw(i);
	}

	for (auto& i : this->freezeHUD)
	{
		this->window->draw(i);
	}
}

void Game::update()
{
	this->pollEvents();

	if (this->health > 0)
	{
		this->updateMousePos();
		this->updateEnemies();
		this->updateHUD();
	}
}

void Game::render()
{
	// Clear old frame
	this->window->clear(sf::Color(0, 0, 0, 255));

	// Draw new frame
	this->renderEnemies();
	this->renderHUD();

	this->window->display();

}
