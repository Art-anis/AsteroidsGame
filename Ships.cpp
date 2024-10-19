#include "Ships.h"
#include <iostream>
#include <string>
#include <random>s

using namespace std;

BasicShip::BasicShip(float x, float y)
{
	ship_texture.loadFromFile("ship1.png");

	sprite.setTexture(ship_texture);

	sprite.setOrigin(sprite.getTexture()->getSize().x / 2, sprite.getTexture()->getSize().y / 2);

	sprite.setScale(0.7f, 0.7f);

	sprite.setPosition(x / 2, y / 2);

	up = false;
	down = false;
	angle = 0;
	version = 1;

	maxLocation.x = x;
	maxLocation.y = y;
}

void BasicShip::draw(sf::RenderWindow &window)
{
	window.draw(sprite);
}

void BasicShip::move()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::S) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			up = true;
			down = false;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			down = true;
			up = false;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			sprite.rotate(-2);
			angle -= 2;
			angle = fmod(angle, 360);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			sprite.rotate(2);
			angle += 2;
			angle = fmod(angle, 360);
		}
	}
	else
	{
		up = false;
		down = false;
	}
}

void BasicShip::update()
{
	sf::Vector2f movement;
	if (up)
	{
		movement.y -= 1.5 * cos(angle * 3.14159265359 / 180);
		movement.x += 1.5 * sin(angle * 3.14159265359 / 180);
	}
	if (down)
	{
		movement.y += 1.5 * cos(angle * 3.14159265359 / 180);
		movement.x -= 1.5 * sin(angle * 3.14159265359 / 180);
	}

	sprite.move(movement);

	sf::Vector2f newPosition(sprite.getPosition());
	if (sprite.getPosition().x > maxLocation.x)
	{
		newPosition.x -= maxLocation.x;
	}
	else if (sprite.getPosition().x < 0)
	{
		newPosition.x += maxLocation.x;
	}

	if (sprite.getPosition().y > maxLocation.y)
	{
		newPosition.y -= maxLocation.y;
	}
	else if (sprite.getPosition().y < 0)
	{
		newPosition.y += maxLocation.y;
	}
	sprite.setPosition(newPosition);
}

float BasicShip::getAngle()
{
	return angle;
}

void BasicShip::setAngle(float rotation)
{
	angle = rotation;
	sprite.setRotation(rotation);
}

int BasicShip::getVersion()
{
	return version;
}

sf::Vector2f BasicShip::getPosition()
{
	return sprite.getPosition();
}

void BasicShip::setPosition(sf::Vector2f pos)
{
	sprite.setPosition(pos);
}

float BasicShip::getShipSize()
{
	return max(sprite.getTexture()->getSize().x, sprite.getTexture()->getSize().y);
}

lvl2Ship::lvl2Ship(float x, float y) : BasicShip(x, y)
{
	ship_texture.loadFromFile("ship2.png");
	sprite.setTexture(ship_texture, true);
	sprite.setOrigin(sprite.getTexture()->getSize().x / 2, sprite.getTexture()->getSize().y / 2);
	version = 2;
}

lvl3Ship::lvl3Ship(float x, float y) : lvl2Ship(x, y)
{
	ship_texture.loadFromFile("ship3.png");
	sprite.setTexture(ship_texture, true);
	sprite.setOrigin(sprite.getTexture()->getSize().x / 2, sprite.getTexture()->getSize().y / 2);
	version = 3;
}

lvl4Ship::lvl4Ship(float x, float y) : BasicShip(x, y)
{
	ship_texture.loadFromFile("ship4.png");
	sprite.setTexture(ship_texture, true);
	sprite.setOrigin(sprite.getTexture()->getSize().x / 2, sprite.getTexture()->getSize().y / 2);
	version = 4;
}

Asteroid::Asteroid(float x, float y, float shipRectSize, vector<sf::Vector2f> pos)
{
	texture.loadFromFile("asteroid.png");

	sprite.setTexture(texture);

	sprite.setOrigin(sprite.getTexture()->getSize().x / 2, sprite.getTexture()->getSize().y / 2);

	sprite.setScale(0.7f, 0.7f);

	float asteroidSize = max(sprite.getTexture()->getSize().x * 0.7, sprite.getTexture()->getSize().y * 0.7);

	maxLocation.x = x;
	maxLocation.y = y;

	random_device rd;
	mt19937 gen(rd());

	uniform_real_distribution<float> randAngle(0, 360);
	angle = randAngle(gen);

	sprite.setRotation(angle);

	turned = false;

	uniform_real_distribution<float> randX(sprite.getTexture()->getSize().x / 2, x - sprite.getTexture()->getSize().x / 2);
	float potX = randX(gen);
	uniform_real_distribution<float> randY(sprite.getTexture()->getSize().y / 2, y - sprite.getTexture()->getSize().y / 2);
	float potY = randY(gen);

	bool badPoint = true;
	while (badPoint)
	{
		badPoint = false;
		if (potX >= pos[0].x - 1.5 * shipRectSize && potX <= pos[0].x + 1.5 * shipRectSize &&
			potY >= pos[0].y - 1.5 * shipRectSize && potY <= pos[0].y + 1.5 * shipRectSize)
		{
			badPoint = true;
			potX = randX(gen);
			potY = randY(gen);
			continue;
		}
		for (int i = 1; i < pos.size(); i++)
		{
			if (potX >= pos[i].x - 1.5 * asteroidSize && potX <= pos[i].x + 1.5 * asteroidSize &&
				potY >= pos[i].y - 1.5 * asteroidSize && potY <= pos[i].y + 1.5 * asteroidSize)
			{
				badPoint = true;
				potX = randX(gen);
				potY = randY(gen);
				break;
			}
		}
	}

	sprite.setPosition(potX, potY);
}

void Asteroid::draw(sf::RenderWindow &window)
{
	window.draw(sprite);
}

void Asteroid::move()
{
	sf::Vector2f position = sprite.getPosition();

	bool wallCollision = false;
	float alpha = angle;
	if (position.x > maxLocation.x - sprite.getTexture()->getSize().x / 2)
	{
		wallCollision = true;
		alpha -= 90;
	}
	else if (position.x < sprite.getTexture()->getSize().x / 2)
	{
		wallCollision = true;
		alpha -= 270;
	}
	else if (position.y > maxLocation.y - sprite.getTexture()->getSize().y / 2)
	{
		wallCollision = true;
		alpha -= 180;
	}
	else if (position.y < sprite.getTexture()->getSize().y / 2)
	{
		wallCollision = true;
	}
	else
	{
		turned = false;
	}

	if (wallCollision && !turned)
	{
		angle += 180 - 2 * alpha;
		angle = fmod(angle, 360);
		sprite.rotate(180 - 2 * alpha);
		turned = true;
	}

	position.x += 2 * sin(angle * 3.14159265359 / 180);
	position.y -= 2 * cos(angle * 3.14159265359 / 180);
	sprite.setPosition(position);
}

sf::Vector2f Asteroid::getPosition()
{
	return sprite.getPosition();
}

int upgrade(int version)
{
	version++;
	if (version > 3)
	{
		version = 0;
	}
	return version;
}