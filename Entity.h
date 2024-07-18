#pragma once
#include<SFML/Graphics.hpp>

class Entity
{
public:
	sf::Vector2f vPosition;
	sf::Sprite sprite;

public:
	Entity();
	Entity(sf::Vector2f&);
};

