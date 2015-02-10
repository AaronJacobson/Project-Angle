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

int main()
{
	bool works = true;
	sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
	sf::Texture playerTexture;
	playerTexture.setSmooth(true);
	playerTexture.loadFromFile("Game Art\\Player Art\\Finished Art\\Player North.png");
	sf::Sprite playerSprite;
	playerSprite.setTexture(playerTexture);
	
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.draw(playerSprite);
		window.clear();
		window.display();
	}

	return 0;
}