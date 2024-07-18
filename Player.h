#pragma once

#include<SFML/Graphics.hpp>
#include<iostream>
#include"Ray.h"
#include "Helper.h"

class Player
{
public:
	sf::Vector2f vPosition;
	sf::Vector2f vDirection;
	bool bIsMoving = false;
	float fAngle;
	float fHalfFOV = 30.0f;
	float fVelocity = 100.0f;
	float fDepth = 300.0f;

public:
	Player(sf::Vector2f, float);

	void Draw2D(sf::RenderWindow&);
	void DrawMiniMap(sf::Vector2f , sf::RenderWindow& , float);
	void HandleInput(float);
	void Update(float);
	void CastRays(sf::RenderWindow& , bool);
};

