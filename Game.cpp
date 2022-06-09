#include "Game.hpp"
#include "Random.hpp"

void Game::Init()
{
	{
		FILE * parameters;
		fopen_s(&parameters, "data/parameters.txt", "r");
		if (!parameters)
			THROW;

		if (fscanf_s(parameters, "%lld%lld%lld", &field_size.x, &field_size.y, &step_mcs) <3)
		{
			fclose(parameters);
			THROW;
		}

		if (!(field_size.x >= 2 && field_size.x <= 70 && field_size.y >= 2 && field_size.y <= 70 && step_mcs >= 100'000 && step_mcs <= 10'000'000))
		{
			fclose(parameters);
			THROW;
		}

		fclose(parameters);
	}

	{
		cell = new sf::Image;
		if (!cell->loadFromFile("data/cell.png"))
			THROW;
		border = new sf::Image;
		if (!border->loadFromFile("data/border.png"))
			THROW;

		if (cell->getSize() != border->getSize() || cell->getSize().x != cell->getSize().y)
			THROW;

		texture = new sf::Texture;
		sprite = new sf::Sprite;
	}
	{
		head = new sf::Image;
		if (!head->loadFromFile("data/head.png"))
			THROW;
		body = new sf::Image;
		if (!body->loadFromFile("data/body.png"))
			THROW;

		if (head->getSize() != cell->getSize() || body->getSize() != cell->getSize())
			THROW;

		apple = new sf::Image;
		if (!apple->loadFromFile("data/apple.png"))
			THROW;

		if (apple->getSize() != cell->getSize())
			THROW;
	}

	{
		window = new sf::RenderWindow(
			sf::VideoMode((unsigned)(cell->getSize().x * (field_size.x + 2)), (unsigned)(cell->getSize().y * (field_size.y + 2 + 1))),
				L"Змейка"
		);
	}

	{
		snake = new Snake;
		snake->Init(field_size);
	}

	{
		direction_updated = false;
	}

	{
		AddApple();
		last_step_on_apple = false;
	}

	{
		font = new sf::Font;
		if (!font->loadFromFile("data/font.ttf"))
			THROW;
		text = new sf::Text;
		text->setFont(*font);
		text->setCharacterSize(cell->getSize().y);
		text->setPosition(0.f, (float)(cell->getSize().y * (field_size.y + 2)));

		score = 0;
	}

	{
		game_over = new sf::Image;
		if (!game_over->loadFromFile("data/game_over.png"))
			THROW;
		game_over_t = new sf::Texture;
		game_over_s = new sf::Sprite;

		game_over_t->loadFromImage(*game_over);
		game_over_s->setTexture(*game_over_t);
	}
}

void Game::Draw()
{
	window->clear();
	DrawField();
	snake->Draw(window, head, body);
	DrawApple();
	DrawScore();
	DrawGameOver();
	window->display();
}

void Game::DrawField()
{
	for (__int64 x = 0; x < field_size.x + 2; ++x)
		for (__int64 y = 0; y < field_size.y + 2; ++y)
		{
			sf::Image * image = nullptr;
			if (x == 0 || x == field_size.x + 1 || y == 0 || y == field_size.y + 1)
				image = border;
			else
				image = cell;

			texture->loadFromImage(*image);
			sprite->setTexture(*texture);
			sprite->setPosition((float)(x * cell->getSize().x), (float)(y * cell->getSize().y));
			window->draw(*sprite);
		}
}

void Game::DrawApple()
{
	texture->loadFromImage(*apple);
	sprite->setTexture(*texture);
	sprite->setPosition(
		(float)(apple->getSize().x * (apple_pos.x + 1)),
		(float)(apple->getSize().y * (apple_pos.y + 1))
	);
	window->draw(*sprite);
}

void Game::DrawScore()
{
	char s[100];
	sprintf_s(s, "Score: %lld", score);
	text->setString(s);
	window->draw(*text);
}

void Game::DrawGameOver()
{
	if (!snake->GameOver(field_size))
		return;

	game_over_s->setPosition(
		(float)(__int64)((field_size.x + 2) / 2.f * cell->getSize().x - game_over->getSize().x / 2.f),
		(float)(__int64)((field_size.y + 2) / 2.f * cell->getSize().y - game_over->getSize().y / 2.f)
	);

	window->draw(*game_over_s);
}

void Game::UpdateStep()
{
	snake->UpdateStep(last_step_on_apple, field_size);
	if (last_step_on_apple = apple_pos == snake->GetHeadPosition())
	{
		AddApple();
		++score;
	}
}

void Game::UpdateKeyboard()
{
	if (!window->hasFocus())
		return;
	if (direction_updated)
		return;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
	{
		snake->UpdateDirection(Snake::Direction::LEFT);
		direction_updated = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
	{
		snake->UpdateDirection(Snake::Direction::DOWN);
		direction_updated = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
	{
		snake->UpdateDirection(Snake::Direction::RIGHT);
		direction_updated = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
	{
		snake->UpdateDirection(Snake::Direction::UP);
		direction_updated = true;
	}
}

void Game::AddApple()
{
	if (snake->GetLength() == field_size.x * field_size.y)
		THROW;

	std::vector<sf::Vector2<__int64>> free_cells;
	auto busy_cells = snake->GetCells();

	for (__int64 x = 0; x < field_size.x; ++x)
		for (__int64 y = 0; y < field_size.y; ++y)
		{
			bool is_busy = false;

			for (auto it = busy_cells.cbegin(); !is_busy && it != busy_cells.cend(); ++it)
				is_busy = *it == sf::Vector2<__int64>(x, y);

			if (!is_busy)
				free_cells.push_back(sf::Vector2<__int64>(x, y));
		}

	if (free_cells.empty())
		THROW;

	apple_pos = free_cells[GetRandomNumber() % free_cells.size()];
}

void Game::Run()
{
	Init();

	sf::Clock clk;
	clk.restart();

	while (window->isOpen())
	{
		sf::Event event;
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window->close();
		}

		if (window->hasFocus() && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
			window->close();

		if (!snake->GameOver(field_size))
		{
			UpdateKeyboard();

			if (clk.getElapsedTime().asMicroseconds() >= step_mcs)
			{
				UpdateStep();
				clk.restart();
				direction_updated = false;
			}
		}

		Draw();
	}
}

Game::~Game()
{
	delete cell;
	delete border;
	delete texture;
	delete sprite;
	delete window;
	delete head;
	delete body;

	delete apple;
	delete font;
	delete text;

	delete game_over;
	delete game_over_t;
	delete game_over_s;
}
