#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>

using namespace std;
SFML_VECTOR2_HPP;

#define TileSize 10 //Tiles are square, x*y pixels e.t.c


class Player
{
public:
	
	Player(int xx, int yy, sf::Color colour, sf::Image &map)
	{

		x = xx;
		y = yy;
		Colour = colour;
		mapP = &map;
		mapP->setPixel(x, y, Colour);
	}
private:
	int x, y;
	sf::Image *mapP;
	sf::Color Colour;


};






class Tile
{
public:
	float x, y; //origin of the top left corner. Add tilesize to them for outer margins
	bool land;
	Tile(float X, float Y, bool L)
	{
		x = X;
		y = Y;
		land = L;
	}
	Tile()
	{

	}
};

vector<Tile> TileMap;


//Fix your map for this to work nicely too
bool TestSegment(sf::Image &map,int x, int y) //If a single pixel is not perfectly green (land) then ignore it, this is an early implementation
{
	for (int Y = y;  Y < y+TileSize; Y++)
	{
		for (int X = x; X < x+TileSize; X++)
		{
			sf::Color Test = map.getPixel(X, Y);
			if (map.getPixel(X,Y)!=sf::Color(51,204,0,255)) //Just the colours of the green
			{
				return false; //Sea
			}
		}
	}
	return true; //Land 
}

void CreateTileMap(sf::Image &map)
{
	//Map is not even, just pretend 980x1520 for now and make something to adjust for stuff like this later

	int StepsX = 980 / TileSize;
	int StepsY = 1520 / TileSize;
	Tile *TempTile = new Tile();
	for (int Y = 0; Y < StepsY;)
	{
		for (int X = 0; X < StepsX;)
		{
			TempTile=new Tile(X*TileSize,Y*TileSize,TestSegment(map,X*TileSize,Y*TileSize)); //Test Segment checks all the pixels, if there is a single non green pixel, sea is implied.
			TileMap.push_back(*TempTile);
			delete TempTile;
			X += 1;
		}
		Y += 1;

	}

	TempTile = NULL;
}







//https://www.sfml-dev.org/tutorials/2.5/graphics-sprite.php
int main()
{
	sf::RenderWindow window(sf::VideoMode(981, 1521), "Please work I'm going crazy");

	sf::CircleShape shape(100.0f);
	shape.setFillColor(sf::Color::Green);

	sf::Texture texture;
	sf::Image Map;




	if (!Map.loadFromFile("map.png"))
	{
	printf("Map load failed");
	}


	CreateTileMap(Map);

	TileMap.size();
	TileMap[7300].land;

	if (!texture.loadFromImage(Map))
	{
		printf("Loading the texture form map failed");
	}

	//create texture

	if (!texture.create(981, 1521))
	{
		printf("Texture failed");
	}


	texture.update(Map); //fill the map contents
	texture.setRepeated(false);
	texture.setSmooth(false);


	sf::Sprite sprite;
	sprite.setTexture(texture);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		//window.draw(shape);
		//window.
		window.draw(sprite);
		window.display();
	}

	return 0;
}