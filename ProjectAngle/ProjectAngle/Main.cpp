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
	Team TEAM;
	Direction DIRECTION;
};

struct Tile{
	int LOCATION_X;
	int LOCATION_Y;
	Actor ACTOR;
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

	//ENEMY_BULLET_VERTICAL.loadFromFile("Game Art\\Bullet Art\\Finished Enemy Bullets\\Enemy Bullet Vertical.png"); need to make the file into a png
	//ENEMY_BULLET_HORIZONTAL.loadFromFile("Game Art\\Bullet Art\\Finished Enemy Bullets\\Enemy Bullet Horizontal.png"); need to make the file into a png

	WALL.loadFromFile("Game Art\\Wall Art\\Wall.png");
}

void makeActor(int x, int y, int health,Direction direction,Team team){
	Actor toAdd;
	toAdd.LOCATION_X = x;
	toAdd.LOCATION_Y = y;
	toAdd.HEALTH = health;
	toAdd.DIRECTION = direction;
	toAdd.TEAM = team;
	sf::Texture texture;
	if (team == BLUE){
		if (direction == NORTH){
			toAdd.SPRITE.setTexture(PLAYER_NORTH);
		}
		else if (direction == EAST){
			toAdd.SPRITE.setTexture(PLAYER_EAST);
		}
		else if (direction == SOUTH){
			toAdd.SPRITE.setTexture(PLAYER_SOUTH);
		}
		else if (direction == WEST){
			toAdd.SPRITE.setTexture(PLAYER_WEST);
		}
	}
	else if (team == RED){
		if (direction == NORTH){
			toAdd.SPRITE.setTexture(ENEMY_NORTH);
		}
		else if (direction == EAST){
			toAdd.SPRITE.setTexture(ENEMY_EAST);
		}
		else if (direction == SOUTH){
			toAdd.SPRITE.setTexture(ENEMY_SOUTH);
		}
		else if (direction == WEST){
			toAdd.SPRITE.setTexture(ENEMY_WEST);
		}
	}
}

void updateTexture(){

}

bool isValidMove(Tile tile){
	if (tile.LOCATION_X > 0 && tile.LOCATION_X <= BOARD_SIZE && tile.LOCATION_Y > 0 && tile.LOCATION_Y <= BOARD_SIZE){
		if (!tile.WALL && tile.ACTOR.EXISTS){
			if (tile.BULLET.TEAM != NULL){//they will walk into the bullet
				return false;
			}
			else{//there is no bullet
				return true;
			}
		}
	}
	return false;
}

bool moveThis(Tile tile, Actor actor){
	if (isValidMove(tile)){
		actor.LOCATION_X = tile.LOCATION_X;
		actor.LOCATION_Y = tile.LOCATION_Y;
		tile.ACTOR = actor;
		return true;
	}
	return false;
}
void checkKeyboardInputs(){
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
		//PLAYER_SPRITE.setTexture(PLAYER_WEST);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
		//PLAYER_SPRITE.setTexture(PLAYER_NORTH);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
		//PLAYER_SPRITE.setTexture(PLAYER_SOUTH);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
		//PLAYER_SPRITE.setTexture(PLAYER_EAST);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){

	}
}


void drawEverything(){
	for (int currentRow = 0; currentRow < BOARD_SIZE; currentRow++){
		for (int currentCol = 0; currentCol < BOARD_SIZE; currentCol++){
			if (GAME_BOARD[currentRow][currentCol].WALL){

			}
		}
	}
}

int main()
{
	loadTextures();

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
		WINDOW.display();
	}

	return 0;
}