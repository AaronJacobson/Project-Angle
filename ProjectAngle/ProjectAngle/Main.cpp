#ifdef SFML_STATIC
#pragma comment(lib, "glew.lib")
#pragma comment(lib, "freetype.lib")
#pragma comment(lib, "jpeg.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "gdi32.lib")  
#endif // SFML_STATIC

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include "String"
using namespace std;

enum Direction{NORTH,EAST,SOUTH,WEST};
enum Team{BLUE,RED};
struct Actor{
	int LOCATION_X;
	int LOCATION_Y;
	int HEALTH;
	Direction DIRECTION;
	Team TEAM;
	bool EXISTS = false;
	sf::Sprite SPRITE;
};

struct Bullet{
	sf::Sprite SPRITE;
	Team TEAM;
	Direction DIRECTION;
};


struct Tile{
	int LOCATION_X;
	int LOCATION_Y;
	Actor * ACTOR = NULL;
	Bullet BULLET;
	bool WALL;
};
const int BOARD_SIZE = 11;
Tile GAME_BOARD[BOARD_SIZE][BOARD_SIZE];
sf::RenderWindow WINDOW(sf::VideoMode(BOARD_SIZE * 50, BOARD_SIZE * 50), "Project Angle");

//textures for all the things
sf::Texture PLAYER_NORTH;
sf::Texture PLAYER_EAST;
sf::Texture PLAYER_SOUTH;
sf::Texture PLAYER_WEST;

sf::Texture ENEMY_NORTH;
sf::Texture ENEMY_EAST;
sf::Texture ENEMY_SOUTH;
sf::Texture ENEMY_WEST;

sf::Texture PLAYER_BULLET_VERTICAL;
sf::Texture PLAYER_BULLET_HORIZONTAL;

sf::Texture ENEMY_BULLET_VERTICAL;
sf::Texture ENEMY_BULLET_HORIZONTAL;

sf::Texture WALL;

Actor PLAYER;
sf::Sprite WALL_SPRITE;
void loadTextures(){
	PLAYER_NORTH.loadFromFile("Game Art\\Player Art\\Finished Art\\Player North.png");
	PLAYER_EAST.loadFromFile("Game Art\\Player Art\\Finished Art\\Player East.png");
	PLAYER_SOUTH.loadFromFile("Game Art\\Player Art\\Finished Art\\Player South.png");
	PLAYER_WEST.loadFromFile("Game Art\\Player Art\\Finished Art\\Player West.png");

	ENEMY_NORTH.loadFromFile("Game Art\\Enemy Art\\Finished Art\\Enemy North.png");
	ENEMY_EAST.loadFromFile("Game Art\\Enemy Art\\Finished Art\\Enemy East.png");
	ENEMY_SOUTH.loadFromFile("Game Art\\Enemy Art\\Finished Art\\Enemy South.png");
	ENEMY_WEST.loadFromFile("Game Art\\Enemy Art\\Finished Art\\Enemy West.png");

	PLAYER_BULLET_VERTICAL.loadFromFile("Game Art\\Bullet Art\\Finished Player Bullets\\Player Bullet Vertical.png");
	PLAYER_BULLET_HORIZONTAL.loadFromFile("Game Art\\Bullet Art\\Finished Player Bullets\\Player Bullet Horizontal.png");

	ENEMY_BULLET_VERTICAL.loadFromFile("Game Art\\Bullet Art\\Finished Enemy Bullets\\Enemy Bullet Vertical.png");
	ENEMY_BULLET_HORIZONTAL.loadFromFile("Game Art\\Bullet Art\\Finished Enemy Bullets\\Enemy Bullet Horizontal.png");

	WALL.loadFromFile("Game Art\\Wall Art\\Wall.png");
	WALL_SPRITE.setTexture(WALL);
}

void placeActor(Actor* actor){
	GAME_BOARD[actor->LOCATION_X][actor->LOCATION_Y].ACTOR = actor;
}



void updateTexture(Actor* actor){
	actor->SPRITE.setPosition((actor->LOCATION_X - 1) * 50, (actor->LOCATION_Y - 1) * 50);
	if (actor->DIRECTION == NORTH){
		actor->SPRITE.setTexture(PLAYER_NORTH);
		cout << "NORTH\n";
	}
	else if (actor->DIRECTION == EAST){
		actor->SPRITE.setTexture(PLAYER_EAST);
		cout << "EAST\n";
	}
	else if (actor->DIRECTION == SOUTH){
		actor->SPRITE.setTexture(PLAYER_SOUTH);
		cout << "SOUTH\n";
	}
	else if (actor->DIRECTION == WEST){
		actor->SPRITE.setTexture(PLAYER_WEST);
		cout << "WEST\n";
	}
}

bool isValidMove(Tile tile){
	if ((tile.LOCATION_X >= 0 && tile.LOCATION_X <= BOARD_SIZE) && (tile.LOCATION_Y >= 0 && tile.LOCATION_Y <= BOARD_SIZE)){
		if (tile.ACTOR == NULL){
			return true;
			if (!tile.WALL && tile.ACTOR->EXISTS){
				//if (tile.BULLET.TEAM != NULL){//they will walk into the bullet
				//	return false;
				//}
				//else{//there is no bullet
				//	return true;
				//}
				return true;
			}
		}
		else{
			cout << "The actor is not null\n";
			return false;
		}
	}
	else{
		cout << tile.LOCATION_X << " " << tile.LOCATION_Y << endl;
	}
	return false;
}

bool moveThis(Tile* tile, Actor actor){
	if (isValidMove(*tile)){
		actor.SPRITE.setPosition(tile->LOCATION_X, tile->LOCATION_Y);
		tile->ACTOR = &actor;
		updateTexture(&actor);
		cout << "moved\n";
		return true;
	}
	cout << "Can't move there\n";
	return false;
}
void checkKeyboardInputs(){
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
		cout << "Moving west\n";
		PLAYER.DIRECTION = WEST;
		moveThis(&GAME_BOARD[PLAYER.LOCATION_X - 1][PLAYER.LOCATION_Y], PLAYER);
		//PLAYER.SPRITE.setTexture(PLAYER_WEST);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
		cout << "Moving north\n";
		PLAYER.DIRECTION = NORTH;
		moveThis(&GAME_BOARD[PLAYER.LOCATION_X][PLAYER.LOCATION_Y-1], PLAYER);
		//PLAYER.SPRITE.setTexture(PLAYER_NORTH);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
		cout << "Moving south\n";
		PLAYER.DIRECTION = SOUTH;
		moveThis(&GAME_BOARD[PLAYER.LOCATION_X][PLAYER.LOCATION_Y+1], PLAYER);
		//PLAYER.SPRITE.setTexture(PLAYER_SOUTH);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
		cout << "Moving east\n";
		PLAYER.DIRECTION = EAST;
		moveThis(&GAME_BOARD[PLAYER.LOCATION_X + 1][PLAYER.LOCATION_Y], PLAYER);
		//PLAYER.SPRITE.setTexture(PLAYER_EAST);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){

	}
}


void drawEverything(){
	for (int currentRow = 0; currentRow < BOARD_SIZE; currentRow++){
		for (int currentCol = 0; currentCol < BOARD_SIZE; currentCol++){
			if (GAME_BOARD[currentRow][currentCol].WALL){//it's a wall
				WALL_SPRITE.setPosition(currentRow, currentCol);
				WINDOW.draw(WALL_SPRITE);
			}
			else{//it's a bullet
			}
			if (GAME_BOARD[currentRow][currentCol].ACTOR != NULL){
				if (GAME_BOARD[currentRow][currentCol].ACTOR->EXISTS){//it's an actor

					WINDOW.draw(GAME_BOARD[currentRow][currentCol].ACTOR->SPRITE);
				}
			}
		}
	}
}

void makeNull(){
	for (int currentRow = 0; currentRow < BOARD_SIZE; currentRow++){
		for (int currentCol = 0; currentCol < BOARD_SIZE; currentCol++){
			GAME_BOARD[currentRow][currentCol].ACTOR = NULL;
		}
	}
}

int main()
{
	loadTextures();
	PLAYER.SPRITE.setTexture(PLAYER_NORTH);
	makeNull();
	PLAYER.DIRECTION = NORTH;
	PLAYER.EXISTS = true;
	PLAYER.LOCATION_X = 1;
	PLAYER.LOCATION_Y = 4;
	PLAYER.TEAM = BLUE;
	GAME_BOARD[1][4].ACTOR = &PLAYER;
	while (WINDOW.isOpen())
	{
		sf::Event event;
		while (WINDOW.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				WINDOW.close();
		}
		checkKeyboardInputs();
		WINDOW.clear();
		//draw everything here
		drawEverything();
		WINDOW.display();
	}

	return 0;
}