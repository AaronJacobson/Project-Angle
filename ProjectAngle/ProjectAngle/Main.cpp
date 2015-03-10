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

enum Team{BLUE,RED};
enum Direction{NORTH,EAST,SOUTH,WEST};
enum BulletMovement{WALL,ACTOR,BULLET,EMPTY};
const int fail = -1;
struct Actor{
	int LOCATION_X;
	int LOCATION_Y;
	Team TEAM;
	Direction DIRECTION;
	sf::Sprite SPRITE;
};

struct Bullet{
	int LOCATION_X;
	int LOCATION_Y;
	bool hasMoved = false;
	Team TEAM;
	Direction DIRECTION;
	sf::Sprite SPRITE;
};

struct Wall{
	int LOCATION_X;
	int LOCATION_Y;
	sf::Sprite SPRITE;
};
struct Tile{
	int LOCATION_X;
	int LOCATION_Y;
	Actor *ACTOR;
	Bullet *BULLET;
	Wall *WALL;
};

const int BOARD_SIZE = 11;
const int TILE_SIZE = 50;
Tile GAME_BOARD[BOARD_SIZE][BOARD_SIZE];
sf::RenderWindow WINDOW(sf::VideoMode(BOARD_SIZE * TILE_SIZE, BOARD_SIZE * TILE_SIZE), "Project Angle");
sf::Clock CLOCK;
bool hasInput = false;
double deltaTime;

//textures for all the things
sf::Texture PLAYER_NORTH_TEXTURE;
sf::Texture PLAYER_EAST_TEXTURE;
sf::Texture PLAYER_SOUTH_TEXTURE;
sf::Texture PLAYER_WEST_TEXTURE;

sf::Texture ENEMY_NORTH_TEXTURE;
sf::Texture ENEMY_EAST_TEXTURE;
sf::Texture ENEMY_SOUTH_TEXTURE;
sf::Texture ENEMY_WEST_TEXTURE;

sf::Texture PLAYER_BULLET_VERTICAL_TEXTURE;
sf::Texture PLAYER_BULLET_HORIZONTAL_TEXTURE;

sf::Texture ENEMY_BULLET_VERTICAL_TEXTURE;
sf::Texture ENEMY_BULLET_HORIZONTAL_TEXTURE;

sf::Texture WALL_TEXTURE;

Actor PLAYER;
void loadTextures(){
	PLAYER_NORTH_TEXTURE.loadFromFile("Game Art\\Player Art\\Finished Art\\Player North.png");
	PLAYER_EAST_TEXTURE.loadFromFile("Game Art\\Player Art\\Finished Art\\Player East.png");
	PLAYER_SOUTH_TEXTURE.loadFromFile("Game Art\\Player Art\\Finished Art\\Player South.png");
	PLAYER_WEST_TEXTURE.loadFromFile("Game Art\\Player Art\\Finished Art\\Player West.png");

	ENEMY_NORTH_TEXTURE.loadFromFile("Game Art\\Enemy Art\\Finished Art\\Enemy North.png");
	ENEMY_EAST_TEXTURE.loadFromFile("Game Art\\Enemy Art\\Finished Art\\Enemy East.png");
	ENEMY_SOUTH_TEXTURE.loadFromFile("Game Art\\Enemy Art\\Finished Art\\Enemy South.png");
	ENEMY_WEST_TEXTURE.loadFromFile("Game Art\\Enemy Art\\Finished Art\\Enemy West.png");

	PLAYER_BULLET_VERTICAL_TEXTURE.loadFromFile("Game Art\\Bullet Art\\Finished Player Bullets\\Player Bullet Vertical.png");
	PLAYER_BULLET_HORIZONTAL_TEXTURE.loadFromFile("Game Art\\Bullet Art\\Finished Player Bullets\\Player Bullet Horizontal.png");

	ENEMY_BULLET_VERTICAL_TEXTURE.loadFromFile("Game Art\\Bullet Art\\Finished Enemy Bullets\\Enemy Bullet Vertical.png");
	ENEMY_BULLET_HORIZONTAL_TEXTURE.loadFromFile("Game Art\\Bullet Art\\Finished Enemy Bullets\\Enemy Bullet Horizontal.png");

	WALL_TEXTURE.loadFromFile("Game Art\\Wall Art\\Wall.png");
}

void initPlayer(){
	PLAYER.DIRECTION = NORTH;
	PLAYER.TEAM = BLUE;
	PLAYER.SPRITE.setTexture(PLAYER_NORTH_TEXTURE);
	GAME_BOARD[PLAYER.LOCATION_Y][PLAYER.LOCATION_X].ACTOR = &PLAYER;
}
void makeActor(Tile *tile,int x,int y,Team team,Direction direction){
	tile->ACTOR = new Actor();
	tile->ACTOR->LOCATION_X = x;
	tile->ACTOR->LOCATION_Y = y;
	tile->ACTOR->TEAM = team;
	tile->ACTOR->DIRECTION = direction;
	if (team == BLUE){
		if (direction == NORTH){
			tile->ACTOR->SPRITE.setTexture(PLAYER_NORTH_TEXTURE);
		}
		else if (direction == EAST){
			tile->ACTOR->SPRITE.setTexture(PLAYER_EAST_TEXTURE);
		}
		else if (direction == SOUTH){
			tile->ACTOR->SPRITE.setTexture(PLAYER_SOUTH_TEXTURE);
		}
		else if (direction == WEST){
			tile->ACTOR->SPRITE.setTexture(PLAYER_WEST_TEXTURE);
		}
	}
	else if (team == RED){
		if (direction == NORTH){
			tile->ACTOR->SPRITE.setTexture(ENEMY_NORTH_TEXTURE);
		}
		else if (direction == EAST){
			tile->ACTOR->SPRITE.setTexture(ENEMY_EAST_TEXTURE);
		}
		else if (direction == SOUTH){
			tile->ACTOR->SPRITE.setTexture(ENEMY_SOUTH_TEXTURE);
		}
		else if (direction == WEST){
			tile->ACTOR->SPRITE.setTexture(ENEMY_WEST_TEXTURE);
		}
	}
	tile->ACTOR->SPRITE.setPosition(x * TILE_SIZE, y * TILE_SIZE);
}

void moveActor(Actor *actor,Tile tile){
	cout << "Moving actor to " << tile.LOCATION_X << ", " << tile.LOCATION_Y << endl;
	tile.ACTOR = actor;
	tile.ACTOR->LOCATION_X = tile.LOCATION_X;
	tile.ACTOR->LOCATION_Y = tile.LOCATION_Y;
}

void updateActorTexture(Actor *actor){
	if (actor->TEAM == BLUE){
		if (actor->DIRECTION == NORTH){
			actor->SPRITE.setTexture(PLAYER_NORTH_TEXTURE);
		}
		else if (actor->DIRECTION == EAST){
			actor->SPRITE.setTexture(PLAYER_EAST_TEXTURE);
		}
		else if (actor->DIRECTION == SOUTH){
			actor->SPRITE.setTexture(PLAYER_SOUTH_TEXTURE);
		}
		else if (actor->DIRECTION == WEST){
			actor->SPRITE.setTexture(PLAYER_WEST_TEXTURE);
		}
	}
	else if (actor->TEAM == RED){
		if (actor->DIRECTION == NORTH){
			actor->SPRITE.setTexture(ENEMY_NORTH_TEXTURE);
		}
		else if (actor->DIRECTION == EAST){
			actor->SPRITE.setTexture(ENEMY_EAST_TEXTURE);
		}
		else if (actor->DIRECTION == SOUTH){
			actor->SPRITE.setTexture(ENEMY_SOUTH_TEXTURE);
		}
		else if (actor->DIRECTION == WEST){
			actor->SPRITE.setTexture(ENEMY_WEST_TEXTURE);
		}
	}
	actor->SPRITE.setPosition(actor->LOCATION_X * TILE_SIZE, actor->LOCATION_Y * TILE_SIZE);
}

bool isValidMove(int y,int x){
	if (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE){
		cout << "Not a valid location. " << x << " " << y << endl;
		return false;
	}
	else  if (GAME_BOARD[y][x].ACTOR != nullptr || GAME_BOARD[y][x].BULLET != nullptr || GAME_BOARD[y][x].WALL != nullptr){
		cout << "The tile is filled. " << GAME_BOARD[y][x].LOCATION_X << " " << GAME_BOARD[y][x].LOCATION_Y << endl;
		return false;
	}
	else{
		cout << "That's a good move.\n";
		return true;
	}
}

void makeBullet(Tile *tile, int x, int y, Team team, Direction direction){
	tile->BULLET = new Bullet();
	tile->BULLET->LOCATION_X = x;
	tile->BULLET->LOCATION_Y = y;
	tile->BULLET->TEAM = team;
	tile->BULLET->DIRECTION = direction;
	if (team == BLUE){
		if (direction == NORTH || direction == SOUTH){
			tile->BULLET->SPRITE.setTexture(PLAYER_BULLET_VERTICAL_TEXTURE);
		}
		else if (direction == WEST || direction == EAST){
			tile->BULLET->SPRITE.setTexture(PLAYER_BULLET_HORIZONTAL_TEXTURE);
		}
	}
	else if (team == RED){
		if (direction == NORTH || direction == SOUTH){
			tile->BULLET->SPRITE.setTexture(ENEMY_BULLET_VERTICAL_TEXTURE);
		}
		else if (direction == WEST || direction == EAST){
			tile->BULLET->SPRITE.setTexture(ENEMY_BULLET_HORIZONTAL_TEXTURE);
		}
	}
	tile->BULLET->SPRITE.setPosition(x * TILE_SIZE, y * TILE_SIZE);
}

void moveBullet(Tile *tileFrom, Tile *tileTo){
	tileTo->BULLET = tileFrom->BULLET;
	tileFrom->BULLET = nullptr;
	tileTo->BULLET->SPRITE.setPosition(tileTo->LOCATION_X*TILE_SIZE, tileTo->LOCATION_Y*TILE_SIZE);
}

void removeBullet(Bullet *bullet){
	if (bullet != nullptr){
		delete GAME_BOARD[bullet->LOCATION_Y][bullet->LOCATION_X].BULLET;
	}
	else{
		cout << "I've been told to deallocate a null bullet pointer\n";
	}
}

void isValidBulletMove(int y,int x,Bullet *bullet){
	if (y < 0 || y >= BOARD_SIZE || x < 0 || x >= BOARD_SIZE){
		Tile *tile = &GAME_BOARD[y][x];
		if (tile->ACTOR != nullptr){
			removeBullet(bullet);
			delete GAME_BOARD[y][x].ACTOR;
		}
		else if (tile->BULLET != nullptr){
			delete GAME_BOARD[y][x].BULLET;
			removeBullet(bullet);
		}
		else if (tile->WALL != nullptr){
			removeBullet(bullet);
		}
		else{
			if (bullet->DIRECTION == NORTH){
				moveBullet(&GAME_BOARD[bullet->LOCATION_Y][bullet->LOCATION_X],tile);
			}
			else if (bullet->DIRECTION == EAST){
				moveBullet(&GAME_BOARD[bullet->LOCATION_Y][bullet->LOCATION_X], tile);
			}
			else if (bullet->DIRECTION == SOUTH){
				moveBullet(&GAME_BOARD[bullet->LOCATION_Y][bullet->LOCATION_X], tile);
			}
			else if (bullet->DIRECTION == WEST){
				moveBullet(&GAME_BOARD[bullet->LOCATION_Y][bullet->LOCATION_X], tile);
			}
		}
	}
	else{
		cout << x << " " << y << " " << BOARD_SIZE << endl;
		removeBullet(bullet);
	}
}
void moveBullets(){
	for (int currentRow = 0; currentRow < BOARD_SIZE; currentRow++){
		for (int currentCol = 0; currentCol < BOARD_SIZE; currentRow++){
			if (GAME_BOARD[currentRow][currentCol].BULLET != nullptr){
				if (!GAME_BOARD[currentRow][currentCol].BULLET->hasMoved){
					GAME_BOARD[currentRow][currentCol].BULLET->hasMoved = true;
					if (GAME_BOARD[currentRow][currentCol].BULLET->DIRECTION == NORTH){
						isValidBulletMove(currentRow - 1, currentCol,GAME_BOARD[currentRow][currentCol].BULLET);
					}
					else if (GAME_BOARD[currentRow][currentCol].BULLET->DIRECTION == EAST){
						isValidBulletMove(currentRow, currentCol + 1, GAME_BOARD[currentRow][currentCol].BULLET);
					}
					else if (GAME_BOARD[currentRow][currentCol].BULLET->DIRECTION == SOUTH){
						isValidBulletMove(currentRow + 1, currentCol, GAME_BOARD[currentRow][currentCol].BULLET);
					}
					else if (GAME_BOARD[currentRow][currentCol].BULLET->DIRECTION == WEST){
						isValidBulletMove(currentRow, currentCol - 1, GAME_BOARD[currentRow][currentCol].BULLET);
					}
				}
			}
		}
	}
}

void fireBullet(){
	GAME_BOARD[PLAYER.LOCATION_Y][PLAYER.LOCATION_X].BULLET = new Bullet();
	Bullet *bullet = GAME_BOARD[PLAYER.LOCATION_Y][PLAYER.LOCATION_X].BULLET;
	cout << "bullet " << GAME_BOARD[PLAYER.LOCATION_Y][PLAYER.LOCATION_X].BULLET << endl;
	bullet->LOCATION_X = PLAYER.LOCATION_X;
	bullet->LOCATION_Y = PLAYER.LOCATION_Y;
	bullet->TEAM = BLUE;
	bullet->DIRECTION = PLAYER.DIRECTION;
	if (bullet->DIRECTION == NORTH || bullet->DIRECTION == SOUTH){
		bullet->SPRITE.setTexture(PLAYER_BULLET_VERTICAL_TEXTURE);
	}
	else if (bullet->DIRECTION == EAST || bullet->DIRECTION == WEST){
		bullet->SPRITE.setTexture(PLAYER_BULLET_HORIZONTAL_TEXTURE);
	}
	bullet->SPRITE.setPosition(bullet->LOCATION_X * TILE_SIZE, bullet->LOCATION_Y * TILE_SIZE);
	if (PLAYER.DIRECTION == NORTH){
		isValidBulletMove(PLAYER.LOCATION_Y - 1, PLAYER.LOCATION_X, GAME_BOARD[PLAYER.LOCATION_Y][PLAYER.LOCATION_X].BULLET);
	}
	else if (PLAYER.DIRECTION == EAST){
		isValidBulletMove(PLAYER.LOCATION_Y - 1, PLAYER.LOCATION_X, GAME_BOARD[PLAYER.LOCATION_Y][PLAYER.LOCATION_X].BULLET);
	}
	else if (PLAYER.DIRECTION == SOUTH){
		isValidBulletMove(PLAYER.LOCATION_Y - 1, PLAYER.LOCATION_X, GAME_BOARD[PLAYER.LOCATION_Y][PLAYER.LOCATION_X].BULLET);
	}
	else if (PLAYER.DIRECTION == WEST){
		isValidBulletMove(PLAYER.LOCATION_Y - 1, PLAYER.LOCATION_X, GAME_BOARD[PLAYER.LOCATION_Y][PLAYER.LOCATION_X].BULLET);
	}
}

void makeWall(Tile *tile){
	tile->WALL = new Wall();
	tile->WALL->LOCATION_X = tile->LOCATION_X;
	tile->WALL->LOCATION_Y = tile->LOCATION_Y;
	tile->WALL->SPRITE.setTexture(WALL_TEXTURE);
	tile->WALL->SPRITE.setPosition(tile->LOCATION_X * TILE_SIZE, tile->LOCATION_Y * TILE_SIZE);
}

void removeWall(int x, int y){
	delete GAME_BOARD[y][x].WALL;
}

void makeBoard(){
	for (int currentRow = 0; currentRow < BOARD_SIZE; currentRow++){
		for (int currentCol = 0; currentCol < BOARD_SIZE; currentCol++){
			GAME_BOARD[currentRow][currentCol].LOCATION_X = currentCol;
			GAME_BOARD[currentRow][currentCol].LOCATION_Y = currentRow;
		}
	}
	makeWall(&GAME_BOARD[0][5]);
	makeWall(&GAME_BOARD[1][5]);
	makeWall(&GAME_BOARD[2][5]);
	makeWall(&GAME_BOARD[3][5]);
	makeWall(&GAME_BOARD[3][6]);
	makeWall(&GAME_BOARD[3][7]);
	initPlayer();
}
void checkKeyboardInputs(){
	int toMoveX = PLAYER.LOCATION_X;
	int toMoveY = PLAYER.LOCATION_Y;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
		hasInput = true;
		PLAYER.DIRECTION = WEST;
		if (isValidMove(PLAYER.LOCATION_Y,PLAYER.LOCATION_X-1)){
			moveActor(&PLAYER, GAME_BOARD[PLAYER.LOCATION_Y][PLAYER.LOCATION_X-1]);
		}
		else{
			cout << "Not a valid move.\n";
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
		hasInput = true;
		PLAYER.DIRECTION = NORTH;
		if (isValidMove(PLAYER.LOCATION_Y-1,PLAYER.LOCATION_X)){
			moveActor(&PLAYER, GAME_BOARD[PLAYER.LOCATION_Y - 1][PLAYER.LOCATION_X]);
		}
		else{
			cout << "Not a valid move.\n";
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
		hasInput = true;
		PLAYER.DIRECTION = SOUTH;
		if (isValidMove(PLAYER.LOCATION_Y +1,PLAYER.LOCATION_X)){
			moveActor(&PLAYER, GAME_BOARD[PLAYER.LOCATION_Y + 1][PLAYER.LOCATION_X]);
		}
		else{
			cout << "Not a valid move.\n";
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
		hasInput = true;
		PLAYER.DIRECTION = EAST;
		if (isValidMove(PLAYER.LOCATION_Y,PLAYER.LOCATION_X+1)){
			moveActor(&PLAYER, GAME_BOARD[PLAYER.LOCATION_Y][PLAYER.LOCATION_X + 1]);
		}
		else{
			cout << "Not a valid move.\n";
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
		hasInput = true;
		fireBullet();
	}
	updateActorTexture(&PLAYER);
}

void drawEverything(){
	for (int currentRow = 0; currentRow < BOARD_SIZE; currentRow++){
		for (int currentCol = 0; currentCol < BOARD_SIZE; currentCol++){
			if (GAME_BOARD[currentRow][currentCol].ACTOR != nullptr){
				WINDOW.draw(GAME_BOARD[currentRow][currentCol].ACTOR->SPRITE);
			}
			else if (GAME_BOARD[currentRow][currentCol].BULLET != nullptr){
				GAME_BOARD[currentRow][currentCol].BULLET->SPRITE.setTexture(PLAYER_BULLET_VERTICAL_TEXTURE);
				WINDOW.draw(GAME_BOARD[currentRow][currentCol].BULLET->SPRITE);//Error occurs here
			}
			else if (GAME_BOARD[currentRow][currentCol].WALL != nullptr){
				WINDOW.draw(GAME_BOARD[currentRow][currentCol].WALL->SPRITE);
			}
		}
	}
}

void deAllocateEverything(){
	for (int currentRow = 0; currentRow < BOARD_SIZE; currentRow++){
		for (int currentCol = 0; currentCol < BOARD_SIZE; currentCol++){
			delete GAME_BOARD[currentRow][currentCol].ACTOR;
			delete GAME_BOARD[currentRow][currentCol].BULLET;
			delete GAME_BOARD[currentRow][currentCol].WALL;
		}
	}
}

int main()
{
	loadTextures();
	makeBoard();
	drawEverything();
	while (WINDOW.isOpen())
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
			cout << "Pressed escape\n";
			WINDOW.close();
			deAllocateEverything();
			return 0;
		}
		while (!hasInput){
			deltaTime = CLOCK.getElapsedTime().asMilliseconds();
			if (deltaTime >= 75){
				checkKeyboardInputs();
				CLOCK.restart();
			}
		}
		hasInput = false;

		sf::Event event;
		while (WINDOW.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				WINDOW.close();
		}
		WINDOW.clear();
		//draw everything here
		drawEverything();
		WINDOW.display();
	}
	deAllocateEverything();
	return 0;
}