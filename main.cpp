#include "Ships.h"
#include <iostream>
#include <vector>

using namespace std;

int main()
{
	const int width = 1200;
	const int height = 800;

	sf::RenderWindow window(sf::VideoMode(width, height), "Title");
	window.setFramerateLimit(60);

	sf::Event event;

		
	BasicShip lvl1 = BasicShip(window.getSize().x, window.getSize().y);

	lvl2Ship lvl2 = lvl2Ship(window.getSize().x, window.getSize().y);

	lvl3Ship lvl3 = lvl3Ship(window.getSize().x, window.getSize().y);

	lvl4Ship lvl4 = lvl4Ship(window.getSize().x, window.getSize().y);

	vector<BasicShip*> ships = { &lvl1, &lvl2, &lvl3, &lvl4 };

	int currentVersion = 0;

	BasicShip* currentShip = ships[0];	

	vector<sf::Vector2f> pos = { ships[0]->getPosition() };

	
	Asteroid a = Asteroid(window.getSize().x, window.getSize().y, currentShip->getShipSize(), pos);
	vector<Asteroid*> asteroids = { &a };
	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
			{
				sf::Vector2f pos = currentShip->getPosition();
				float angle = currentShip->getAngle();
				currentVersion = upgrade(currentVersion);
				currentShip = ships[currentVersion];
				currentShip->setPosition(pos);
				currentShip->setAngle(angle);
			}
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
			{
				asteroids.push_back(new Asteroid(window.getSize().x, window.getSize().y, currentShip->getShipSize(), pos));
				pos.push_back(asteroids[asteroids.size() - 1]->getPosition());
			}
			currentShip->move();
		}
		
		window.clear();
		currentShip->update();
		currentShip->draw(window);
		for (int i = 0; i < asteroids.size(); i++)
		{
			asteroids[i]->move();
			asteroids[i]->draw(window);
		}
		window.display();
	}
	return 0;
}