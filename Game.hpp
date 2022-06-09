#pragma once

#include <SFML\Graphics.hpp>
#include "Exception.hpp"
#include "Snake.hpp"

class Game
{
private:
	sf::Image * cell;
	sf::Image * border;
	sf::Vector2<__int64> field_size;
	__int64 step_mcs;

	sf::Texture * texture;
	sf::Sprite * sprite;

	sf::RenderWindow * window;

	Snake * snake;
	sf::Image * head;
	sf::Image * body;

	sf::Vector2<__int64> apple_pos;
	sf::Image * apple;

	sf::Font * font;
	sf::Text * text;
	__int64 score;

	sf::Image * game_over;
	sf::Texture * game_over_t;
	sf::Sprite * game_over_s;

	void Init();

	void Draw();
	void DrawField();
	void DrawApple();
	void DrawScore();
	void DrawGameOver();

	void UpdateStep();
	void UpdateKeyboard();
	void AddApple();

	bool direction_updated;
	bool last_step_on_apple;
public:
	void Run();
	~Game();
};
