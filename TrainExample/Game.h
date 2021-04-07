#pragma once
#include<SFML/Graphics.hpp>

struct node {
	int posx, posy;
	int direction;
	sf::Sprite* s;
	struct node* llink;
	struct node* rlink;
};
typedef struct node* NODE;
enum direction {
	up, down, left, right
};
class Game
{
	private:NODE head, tail, cur;
			int speed = 200;
			int count = 0;
			int score = -1;
			sf::Vector2f snoopyPos;
	private:sf::RenderWindow window;
			sf::Sprite TrainSprite;
			sf::Texture spriteSheet;
			sf::Sprite snoopy;
			sf::Event event;
			sf::Text Score;
			sf::Font font;
			sf::Texture Background;
			sf::Sprite bg;

	private:void Update();
			void Render();
			void Start();
			void LoadSprites();
			NODE insertHead(NODE head, sf::Sprite* sprite);
			NODE deleteTail(NODE tail);
			void processEvents();
	private:void setTrailingSprite(NODE node, sf::Sprite* sp);
			void setHeadSprite(NODE node, sf::Sprite* sp);
			sf::Vector2f getSnoopyCoords();
			void HandleSnoopyTouched();
			bool isSnoopyTouched(NODE head, sf::Sprite* snoopy);
			void checkIntersection(NODE head, sf::RenderWindow* window);
	public:	void Run();
};

