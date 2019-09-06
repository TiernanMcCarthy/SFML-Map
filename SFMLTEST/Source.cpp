#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>
#include <math.h>

sf::Color colours[4] = { sf::Color(255,0,0),sf::Color(0,255,0),sf::Color(0,0,255),sf::Color(255,0,255) };

using namespace std;
SFML_VECTOR2_HPP;
sf::Image Map;

#define Width 980
#define Height 1520

#define TileSize 10 //Tiles are square, x*y pixels e.t.c




void DrawSegment(sf::Image &map, int x, int y, sf::Color DrawColour)
{
	for (int Y = y; Y < y + TileSize; Y++)
	{
		for (int X = x; X < x + TileSize; X++)
		{
			map.setPixel(X, Y, DrawColour);
		}
	}

}



class Tile
{
public:
	float x, y; //origin of the top left corner. Add tilesize to them for outer margins
	bool land;
	bool occupied;
	//sf::Image* localmap;

	Tile(float X, float Y, bool L)
	{
		x = X;
		y = Y;
		land = L;
		occupied = false;
	}
	Tile()
	{

	}
	void DrawSegment(sf::Color c)
	{
		if (x > 0 && y > 0)
		{
			for (int Y = y; Y < y + TileSize; Y++)
			{
				for (int X = x; X < x + TileSize; X++)
				{
					Map.setPixel(X, Y, c);
				}
			}
		}
	}
};

vector<Tile> TileMap;
//Tiles are added in steps defined by their size. As they are evenly distributed they can be found by retrieving their position.
Tile& GetTileFromCord(int X, int Y)
{

	if (Y < 1)
	{
		return(TileMap[X / TileSize]);
	}
	else
	{
		int StepsX = 980 / TileSize;
		int StepsY = 1520 / TileSize;
		if (Y / TileSize != 1)
		{
			int Position = (Y / TileSize);
			Position = Position * StepsX;
			Position = Position + X / TileSize;
			return(TileMap[Position]);
		}
		else
		{
			int Position = StepsX + X / TileSize;
			return TileMap[Position];
		}
	}


}



Tile* CheckAdjacentTiles(Tile* CurrentTile)
{
	
	Tile* tilett = new Tile();
	//Left
	if (CurrentTile->x != 0) //Not offmap to the left
	{
		Tile* tilett = &GetTileFromCord(CurrentTile->x - TileSize, CurrentTile->y);
		if (tilett->land == true && tilett->occupied == false)
		{
			return tilett;
		}
	}

	//Right
	if (CurrentTile->x!=Width-TileSize) //Not offmap to the right
	{
		Tile* tilett = &GetTileFromCord(CurrentTile->x + TileSize, CurrentTile->y);
		if (tilett->land == true && tilett->occupied == false)
		{
			return tilett;
		}
	}

	//Down
	if (CurrentTile->y!=Height-TileSize)
	{
		Tile* tilett = &GetTileFromCord(CurrentTile->x, CurrentTile->y+TileSize);
		if (tilett->land == true && tilett->occupied == false)
		{
			return tilett;
		}
	}


	//UP
	if (CurrentTile->y !=0)
	{
		Tile* tilett = &GetTileFromCord(CurrentTile->x, CurrentTile->y - TileSize);
		if (tilett->land == true && tilett->occupied == false)
		{
			return tilett;
		}
	}

	return &Tile(-1, -1, false);
}
class Player;
vector<Player>PlayerList;

class Player
{
public:

	Player(Tile& home, sf::Color colour)
	{

		Colour = colour;
		if (home.x != -1)
		{
			Home = &home;
			Home->DrawSegment(colour);
			Home->occupied = true;
		}
	}

	sf::Vector2i GetPos()
	{
		return sf::Vector2i(x, y);
	}

	void Die(bool Natural) //A natural death means we draw the land as green again;
	{
		if (Natural == true)
		{
			Home->DrawSegment(sf::Color(51, 204, 0, 255));
			Home->occupied = false;
		}
		delete Home;
		Home = NULL;
	}

	void Reproduce()
	{
		if (Home!=NULL)
		{

		
		Player* Temp = new Player(*CheckAdjacentTiles(Home), sf::Color(255,255,255));
		if (Temp->x!=-1)
		{
			PlayerList.push_back(*Temp);
		}
		Temp = NULL;
		delete Temp;
		}
	//	PlayerList.push_back(new Player())
	}

	~Player()
	{

	}

private:
	int x, y;
	Tile *Home;
	sf::Color Colour;


};



//Player F = Player(-5, -5, sf::Color(1, 1, 1));


//Fix your map for this to work nicely too
bool TestSegment(int x, int y) //If a single pixel is not perfectly green (land) then ignore it, this is an early implementation
{
	for (int Y = y;  Y < y+TileSize; Y++)
	{
		for (int X = x; X < x+TileSize; X++)
		{
			sf::Color Test = Map.getPixel(X, Y);
			if (Map.getPixel(X,Y)!=sf::Color(51,204,0,255)) //Just the colours of the green
			{
				return false; //Sea
			}
		}
	}
	return true; //Land 
}

void CreateTileMap()
{
	//Map is not even, just pretend 980x1520 for now and make something to adjust for stuff like this later

	int StepsX = 980 / TileSize;
	int StepsY = 1520 / TileSize;
	Tile *TempTile = new Tile();
	for (int Y = 0; Y < StepsY;)
	{
		for (int X = 0; X < StepsX;)
		{
			
			TempTile=new Tile(X*TileSize,Y*TileSize,TestSegment(X*TileSize,Y*TileSize)); //Test Segment checks all the pixels, if there is a single non green pixel, sea is implied.
			TileMap.push_back(*TempTile);
			delete TempTile;
			X += 1;
		}
		Y += 1;

	}
	TempTile = NULL;
}





//limit to 4 for now
void SpawnPlayers(int Amount)
{
	int size = TileMap.size();
	int place;
	for (int i = 0; i < Amount;)
	{
		place = rand() % size;
		if (TileMap[place].land)
		{
			PlayerList.push_back(Player(TileMap[place], colours[i])); 
			i++;
		}
	}
}


//https://www.sfml-dev.org/tutorials/2.5/graphics-sprite.php
int main()
{
	srand(time(NULL)); //seed for random function
	sf::RenderWindow window(sf::VideoMode(981, 1521), "Please work I'm going crazy");

	sf::CircleShape shape(100.0f);
	shape.setFillColor(sf::Color::Green);

	sf::Texture texture;



	if (!Map.loadFromFile("map.png"))
	{
	printf("Map load failed");
	}


	CreateTileMap();

	TileMap.size();
	TileMap[7300].land;
	GetTileFromCord(10, 10);
	if (!texture.loadFromImage(Map))
	{
		printf("Loading the texture form map failed");
	}

	//create texture

	if (!texture.create(981, 1521))
	{
		printf("Texture failed");
	}

	DrawSegment(Map, 10, 0, sf::Color(255, 0, 0));

	//Player Fun = Player(TileMap[6420], sf::Color(0, 0, 255));

	SpawnPlayers(4);
	PlayerList[0].Reproduce();
	texture.update(Map); //fill the map contents
	texture.setRepeated(false);
	texture.setSmooth(false);


	sf::Sprite sprite;
	sprite.setTexture(texture);
	window.setFramerateLimit(5);// limit frame rate to a reasonable level

	int FrameSpeed = 5;
	int size;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type==sf::Event::KeyPressed)
			{
				//sf::Keyboard::isKeyPressed(sf::Keyboard::Left)
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
				{
					FrameSpeed += 1;
					window.setFramerateLimit(FrameSpeed);

				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
				{
					FrameSpeed -= 1;
					window.setFramerateLimit(FrameSpeed);
				}
			}
		}
		
		window.clear();
		//window.draw(shape);
		//window.
		size = PlayerList.size();
		PlayerList[size-1].Reproduce();
		texture.update(Map);
		window.draw(sprite);
		window.display();
	}

	return 0;
}