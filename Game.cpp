#include "Game.h"
#include<SFML/Graphics.hpp>
#define side 15
#define size 208.5


NODE Game::insertHead(NODE head, sf::Sprite* sprite) {
	NODE temp;
	temp = (NODE)malloc(sizeof(struct node));
	switch (head->direction) {
	case direction(up):
		temp->posy = head->posy - side * 2;
		temp->posx = head->posx; break;
	case direction(down):
		temp->posy = head->posy + side * 2;
		temp->posx = head->posx; break;
	case direction(left):
		temp->posx = head->posx - side * 2;
		temp->posy = head->posy; break;
	case direction(right):
		temp->posx = head->posx + side * 2;
		temp->posy = head->posy; break;
	}
	temp->direction = head->direction;
	temp->s = sprite;
	temp->llink = NULL;
	temp->rlink = head;
	head->llink = temp;
	return temp;
}

NODE Game::deleteTail(NODE tail) {
	NODE temp;
	temp = tail->llink;
	temp->rlink = NULL;
	free(tail);
	tail = NULL;
	return temp;
}

void Game::Update() {
	if (head->posx > 810)
		head->posx = 0;
	if (head->posx < 0)
		head->posx = 810;
	if (head->posy > 600)
		head->posy = 0;
	if (head->posy < 0)
		head->posy = 600;
	HandleSnoopyTouched();
	head = insertHead(head, &TrainSprite);
	tail = deleteTail(tail);
	checkIntersection(head, &window);
	count = 0;
}

void Game::Start() {
	LoadSprites();
	head = (NODE)malloc(sizeof(struct node));
	head->posx = 0;
	head->posy = 0;
	head->direction = right;
	head->s = &TrainSprite;
	head->llink = head->rlink = NULL;
	tail = head;
	snoopy.setPosition(head->posx, head->posy);
	Score.setPosition(700, 10);
	Score.setString(sf::String(std::to_string(score + 1)));
	window.create(sf::VideoMode(800, 600), "Train", sf::Style::Titlebar | sf::Style::Close);
}

void Game::Run() {
	Start();
	while (window.isOpen()) {
		Render();
		processEvents();
		if (count == speed) {
			Update();
		}
		count++;
	}
}

void Game::Render() {
	window.clear();
	window.draw(bg);
	window.draw(Score);
	setHeadSprite(head, &TrainSprite);
	cur = head;
	while (cur != NULL) {
		if (cur != head)
			setTrailingSprite(cur, &TrainSprite);
		(*cur->s).setPosition(cur->posx, cur->posy);
		window.draw(*cur->s);
		cur = cur->rlink;
	}
	window.draw(snoopy);
	window.display();
}

void Game::LoadSprites() {
	this->spriteSheet.loadFromFile("resources/trainsprite.png");
	TrainSprite.setTexture(spriteSheet);
	TrainSprite.setScale(sf::Vector2f(0.25, 0.25));
	TrainSprite.setOrigin(104.5, 104.5);

	snoopy.setTexture(spriteSheet);
	snoopy.setTextureRect(sf::IntRect(size * 3, size * 1, size, size));
	snoopy.setScale(0.2, 0.2);
	snoopy.setOrigin(104.5, 104.5);

	font.loadFromFile("resources/Roboto-Bold.ttf");
	Score.setFont(font);
	Score.setFillColor(sf::Color::Black);

	this->Background.loadFromFile("resources/sand.jpg");
	bg.setTexture(Background);
	bg.setScale(2, 2);
}

void Game::setHeadSprite(NODE node, sf::Sprite* sp) {
	switch (head->direction) {
	case up: sp->setTextureRect(sf::IntRect(size * 1, size * 0, size, size));
		head->s = sp; break;
	case down: sp->setTextureRect(sf::IntRect(size * 0, size * 0, size, size));
		head->s = sp; break;
	case left: sp->setTextureRect(sf::IntRect(size * 3, size * 0, size, size));
		head->s = sp; break;
	case right: sp->setTextureRect(sf::IntRect(size * 2, size * 0, size, size));
		head->s = sp; break;
	}
}

void Game::setTrailingSprite(NODE node, sf::Sprite* sp) {
	switch (node->direction) {
	case up: sp->setTextureRect(sf::IntRect(size * 1, size * 1, size, size));
		node->s = sp; break;
	case down: sp->setTextureRect(sf::IntRect(size * 2, size * 1, size, size));
		node->s = sp; break;
	case left: sp->setTextureRect(sf::IntRect(size * 0, size * 1, size, size));
		node->s = sp; break;
	case right: sp->setTextureRect(sf::IntRect(size * 0, size * 1, size, size));
		node->s = sp; break;
	}
}

void Game::processEvents() {
	if (window.pollEvent(event)) {
		switch (event.type) {
		case sf::Event::Closed: window.close(); break;
		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::W && head->direction != down) {
				head->direction = up;
			}
			else if (event.key.code == sf::Keyboard::S && head->direction != up) {
				head->direction = down;
			}
			else if (event.key.code == sf::Keyboard::A && head->direction != right) {
				head->direction = left;
			}
			else if (event.key.code == sf::Keyboard::D && head->direction != left) {
				head->direction = right;
			}
		}
	}
}

sf::Vector2f Game::getSnoopyCoords() {
	return sf::Vector2f((rand() % 26 + 1) * side * 2, (rand() % 19 + 1) * side * 2);
}

void Game::HandleSnoopyTouched() {
	if (isSnoopyTouched(head, &snoopy)) {
		head = insertHead(head, &TrainSprite);
		speed--;
		while (1) {
			snoopyPos = getSnoopyCoords();
			cur = head;
			while (cur != NULL) {
				if (cur->posx == snoopyPos.x && cur->posy == snoopyPos.y) {
					break;
				}
				cur = cur->rlink;
			}
			if (cur == NULL)
				break;
		}
		snoopy.setPosition(getSnoopyCoords());
		score++;
		Score.setString(sf::String(std::to_string(score)));
	}
}

bool Game::isSnoopyTouched(NODE head, sf::Sprite* snoopy) {
	return sf::Vector2f(head->posx, head->posy) == snoopy->getPosition();
}

void Game::checkIntersection(NODE head, sf::RenderWindow* window) {
	NODE cur = head->rlink;
	while (cur != NULL) {
		if (head->posx == cur->posx && head->posy == cur->posy) {
			window->close();
		}
		cur = cur->rlink;
	}
}
