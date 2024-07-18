#pragma once

#include<SFML/Graphics.hpp>
#include<iostream>
#include<fstream>
#include"Entity.h"

static const int iHalfScreenWidth = 720;
static const int iScreenHeight = 720;
static const int iMapWidth = 32;
static const int iMapHeight = 32;
static const int iTileSize = iHalfScreenWidth / iMapWidth;
static const float fMaxWallHeight = 90.0f;
static sf::Vector2f vOffset2D = { 0 , 0 };
static sf::Font font;
static sf::Color cColors[4] = { sf::Color::Red , sf::Color::Green  , sf::Color::Blue  , sf::Color::Magenta };
extern sf::Texture WallTexture;
extern sf::Texture EntityTexture;
extern sf::Texture BGTexture;
extern Entity Steve;
extern int Map[iMapWidth][iMapHeight];

inline void LoadMap(std::string address , sf::Vector2f &pPos) 
{
	for (int y = 0; y < iMapHeight; y++)
		for (int x = 0; x < iMapWidth; x++)
			Map[x][y] = 0;

	std::ifstream fin;
	fin.open(address);

	if (!fin.is_open()) 
	{
		std::cout << "[LOAD MAP]Cannot open Map.txt\n";
		return;
	}
	std::string line;

	int y = 0;
	while (std::getline(fin, line))
	{
		for (int x = 0; x < line.size(); x++) 
		{
			if (line[x] == '1')
				Map[x][y] = 1;
			else if(line[x] == '2')
				Map[x][y] = 2;
			else if (line[x] == '3')
				Map[x][y] = 3;
			else if (line[x] == '4') 
			{
				pPos.x = (x + 0.5f) * iTileSize;
				pPos.y = (y + 0.5f) * iTileSize;
				Map[x][y] = 0;
			}
			else if (line[x] == '5')
			{
				Steve.vPosition.x = (x + 0.5f) * iTileSize;
				Steve.vPosition.y = (y + 0.5f) * iTileSize;
				Steve.sprite.setTexture(EntityTexture);
				Map[x][y] = 5;
			}

			else
				Map[x][y] = 0;
		}
		y++;
	}
	fin.close();
}


inline float DegreeToRadians(float degree) {
	return degree * (float)0.01745f;
}

inline float RadiansToDegree(float radians) {
	return radians * (float)57.296;
}

inline void DrawLine(sf::Vector2f pos1, sf::Vector2f pos2, sf::RenderWindow& window)
{
	sf::Vertex line[2];
	line[0].position = pos1;
	line[0].color = sf::Color::White;
	line[1].position = pos2;
	line[1].color = sf::Color::White;

	window.draw(line, 2, sf::Lines);
}

inline float Distance(sf::Vector2f pos1, sf::Vector2f pos2)
{
	return std::sqrt(((pos2.x - pos1.x) * (pos2.x - pos1.x)) + ((pos2.y - pos1.y) * (pos2.y - pos1.y)));
}

inline float AngleOfVector(sf::Vector2f vector) 
{
	return atan2(vector.y, vector.x);
}

inline void DrawText(sf::RenderWindow& win, sf::Font font, sf::Vector2f pos, float num, int nsize)
{
	sf::Text tex;

	tex.setFont(font);
	tex.setCharacterSize(nsize);
	tex.setString(std::to_string(num));
	tex.setFillColor(sf::Color::White);
	tex.setPosition(pos);

	win.draw(tex);
}

inline float wrap(float angle)
{
	if (angle < 0) 
		return angle + 360;

	else if (angle > 360) 
		return angle - 360;
	
	else 
		return angle;
	
}

inline void DrawWallStrip(sf::Vector2f vpos, sf::Vector2f vSize, int type, float fog,  sf::RenderWindow& window)
{
	sf::Color color;
	//std::cout << type << "\n";
	if (type == 1)
		color = sf::Color(255 * fog , 0 , 0);
	else if (type == 2)
		color = sf::Color(0 , 255 * fog , 0);
	else if (type == 3)
		color = sf::Color(0 , 0 , 255 * fog);
	else if (type == 4)
		color = sf::Color(255 * fog, 255 * fog, 0);
	else if (type == 5)
		color = sf::Color(255 * fog, 100 * fog, 255 * fog);

	sf::RectangleShape rect;
	rect.setPosition(vpos);  // x, (ScreenHeight - fWallHeight) * 0.5f
	rect.setSize(vSize);  //sf::Vector2f(1, fWallHeight)
	rect.setFillColor(color); // 255 * (1.0f - hitinfo.fHitDistance / fMaxDepth)
	window.draw(rect);
}

inline void DrawSky(sf::RenderWindow& window) {
	/*sf::RectangleShape sky;
	sky.setPosition(0, 0);
	sky.setSize({ iHalfScreenWidth , iScreenHeight / 2.0f});
	sky.setFillColor(sf::Color(100, 100, 255));
	window.draw(sky);*/
	
	sf::Sprite sky;
	sky.setTexture(BGTexture);
	sky.setPosition({ 0.0f , 0.0f});
	window.draw(sky);

	sf::RectangleShape floor;
	floor.setPosition(0, iScreenHeight / 2);
	floor.setSize({ iHalfScreenWidth , iScreenHeight / 2.0f });
	floor.setFillColor(sf::Color(171, 128, 34));
	window.draw(floor);
}


inline void DrawMap(sf::RenderWindow& window) {
	for (int y = 0; y < iMapHeight; y++) {
		for (int x = 0; x < iMapWidth; x++) {
			if (Map[x][y] == 1) {
				sf::RectangleShape square;
				square.setPosition(vOffset2D.x + x * iTileSize, vOffset2D.y + y * iTileSize);
				square.setSize(sf::Vector2f(iTileSize, iTileSize));
				square.setFillColor(sf::Color::Red);
				square.setOutlineThickness(1);
				square.setOutlineColor(sf::Color::White);
				window.draw(square);
			}
			else if (Map[x][y] == 2) {
				sf::RectangleShape square;
				square.setPosition(vOffset2D.x + x * iTileSize, vOffset2D.y + y * iTileSize);
				square.setSize(sf::Vector2f(iTileSize, iTileSize));
				square.setFillColor(sf::Color::Green);
				square.setOutlineThickness(1);
				square.setOutlineColor(sf::Color::White);
				window.draw(square);
			}
			else if (Map[x][y] == 3) {
				sf::RectangleShape square;
				square.setPosition(vOffset2D.x + x * iTileSize, vOffset2D.y + y * iTileSize);
				square.setSize(sf::Vector2f(iTileSize, iTileSize));
				square.setFillColor(sf::Color::Blue);
				square.setOutlineThickness(1);
				square.setOutlineColor(sf::Color::White);
				window.draw(square);
			}
		}
	}
}

inline void DrawTexture(sf::RenderWindow& window) {
	float samplex = 0.0f;
	sf::Sprite sprite;
	sprite.setPosition(vOffset2D + sf::Vector2f(200.0f, 10.0f));
	//sprite.setTextureRect(sf::IntRect(45.0f * samplex, 0.0f , 10.0f, 45.0f));
	sprite.setTexture(WallTexture);

	window.draw(sprite);
}
