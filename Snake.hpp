#pragma once

#include <SFML\Graphics.hpp>
#include "Exception.hpp"
#include <list>

class Snake
{
public:
	enum class Direction { UP, DOWN, RIGHT, LEFT };
private:
	std::list<sf::Vector2<__int64>> cells;
	sf::Vector2<__int64> head_pos;
	Direction direction;

	sf::Texture * texture;
	sf::Sprite * sprite;
public:
	void Init(sf::Vector2<__int64> field_size);
	void Draw(sf::RenderWindow * window, sf::Image * head, sf::Image * body) const;
	void UpdateStep(bool head_on_apple, sf::Vector2<__int64> field_size);
	bool GameOver(sf::Vector2<__int64> field_size) const;
	void UpdateDirection(Direction new_direction);
	__int64 GetLength() const;

	std::list<sf::Vector2<__int64>> GetCells() const;
	sf::Vector2<__int64> GetHeadPosition() const;

	~Snake();
};
