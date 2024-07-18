#pragma once

#include<SFML/Graphics.hpp>
#include "Helper.h"

class RayHit {
public:
	sf::Vector2f vHitPosition;
	bool bIsHit = false;
	float fHitDistance = 0.0f;
	float fPerpendicularDistance = 0.0f;
	int iWallType = 0;
	//sf::Vector2f UVCoord = { -1 , -1 };
};


class Ray
{
public:
	sf::Vector2f vOrigin;
	sf::Vector2f vDirection;
	float fAngle = 0.0f;

	Ray(sf::Vector2f origin, float angle) :
		vOrigin(origin), fAngle(angle)
	{
		vDirection.x = cos(DegreeToRadians(angle));
		vDirection.y = sin(DegreeToRadians(angle));
	}

	void CastRay(float, RayHit&);
	void CastRayDDA(float , RayHit&);
};

