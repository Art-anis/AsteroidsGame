#include <SFML/Graphics.hpp>
#include <vector>

class BasicShip
{
public:
	BasicShip(float x, float y);

	void draw(sf::RenderWindow &window);

	void move();

	void update();

	float getAngle();

	void setAngle(float rotation);

	int getVersion();

	float getShipSize();

	sf::Vector2f getPosition();

	void setPosition(sf::Vector2f pos);

protected:
	sf::Sprite sprite;

	sf::Texture ship_texture;

	bool up;
	bool down;
	int version;
	float angle;

	sf::Vector2f maxLocation;
};

class lvl2Ship : public BasicShip
{
public:
	lvl2Ship(float x, float y);
};

class lvl3Ship : public lvl2Ship
{
public:
	lvl3Ship(float x, float y);
};

class lvl4Ship : public BasicShip
{
public:
	lvl4Ship(float x, float y);
};

class Asteroid
{
public:
	Asteroid(float x, float y, float shipRectSize, std::vector<sf::Vector2f> pos);

	void draw(sf::RenderWindow& window);

	void move();

	void update();

	sf::Vector2f getPosition();
private:
	sf::Sprite sprite;
	sf::Texture texture;

	float angle;
	bool clockwiseRotation;
	bool turned;

	sf::Vector2f maxLocation;
};

int upgrade(int version);