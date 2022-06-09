#include "Snake.hpp"

void Snake::Init(sf::Vector2<__int64> field_size)
{
	cells.clear();
	head_pos.x = field_size.x / 2;
	head_pos.y = field_size.y / 2;
	direction = Direction::UP;

	cells.push_back(head_pos);
	//x cells.push_back(sf::Vector2<__int64>(head_pos.x, head_pos.y + 1));
	//x cells.push_back(sf::Vector2<__int64>(head_pos.x, head_pos.y + 2));

	texture = new sf::Texture;
	sprite = new sf::Sprite;
}

void Snake::Draw(sf::RenderWindow * window, sf::Image * head, sf::Image * body) const
{
	texture->loadFromImage(*body);
	sprite->setTexture(*texture);
	for (auto it = cells.cbegin(); it != cells.cend(); ++it)
	{
		if (it != cells.cbegin())
		{
			sprite->setPosition(
				(float)((it->x + 1) * body->getSize().x),
				(float)((it->y + 1) * body->getSize().y)
			);
			window->draw(*sprite);
		}
	}

	texture->loadFromImage(*head);
	sprite->setTexture(*texture);
	sprite->setPosition(
		(float)((head_pos.x + 1) * head->getSize().x),
		(float)((head_pos.y + 1) * head->getSize().y)
	);
	window->draw(*sprite);
}

void Snake::UpdateStep(bool head_on_apple, sf::Vector2<__int64> field_size)
{
	if (GameOver(field_size))
		return;

	switch (direction)
	{
	case Snake::Direction::UP:
		--head_pos.y;
		break;
	case Snake::Direction::DOWN:
		++head_pos.y;
		break;
	case Snake::Direction::RIGHT:
		++head_pos.x;
		break;
	case Snake::Direction::LEFT:
		--head_pos.x;
		break;
	default:
		THROW;
	}

	auto last_cell = cells.back();

	for (auto it = cells.rbegin(); it != cells.rend(); ++it)
	{
		auto jt = it;
		++jt;
		if (jt != cells.rend())
			*it = *jt;
	}

	cells.pop_front();
	if (head_on_apple)
		cells.push_back(last_cell);
	cells.push_front(head_pos);
}

bool Snake::GameOver(sf::Vector2<__int64> field_size) const
{
	if (head_pos.x < 0 || head_pos.x >= field_size.x || head_pos.y < 0 || head_pos.y >= field_size.y)
		return true;

	for (auto it = cells.cbegin(); it != cells.cend(); ++it)
	{
		if (it != cells.cbegin() && *it == head_pos)
			return true;
	}
	return false;
}

void Snake::UpdateDirection(Direction new_direction)
{
	if (GetLength() == 1)
	{
		direction = new_direction;
		return;
	}

	if (
		direction == Direction::UP && new_direction == Direction::DOWN ||
		direction == Direction::DOWN && new_direction == Direction::UP ||
		direction == Direction::RIGHT && new_direction == Direction::LEFT ||
		direction == Direction::LEFT && new_direction == Direction::RIGHT
		)
		return;

	direction = new_direction;
}

__int64 Snake::GetLength() const
{
	return (__int64)cells.size();
}

std::list<sf::Vector2<__int64>> Snake::GetCells() const
{
	return cells;
}

sf::Vector2<__int64> Snake::GetHeadPosition() const
{
	return head_pos;
}

Snake::~Snake()
{
	delete texture;
	delete sprite;
}
