#include "Ray.h"

void Ray::CastRay(float fDepth, sf::Vector2f vPlayerDir , RayHit& hit) 
{
	float fHitDistance = 0.0f;
	bool IsHitWall = false;
	int iTestX, iTestY;
	while (!IsHitWall && fHitDistance <= fDepth)
	{

		iTestX = (int)(vDirection.x * fHitDistance + vOrigin.x) / iTileSize; 
		iTestY = (int)(vDirection.y * fHitDistance + vOrigin.y) / iTileSize;

		if (Map[iTestX][iTestY] != 0) 
		{
			//hit.iWallType = Map[iTestX][iTestY];
			IsHitWall = true;
			break;
		}
		fHitDistance += 0.1f;
	}	
	
	hit.vHitPosition = vOrigin + vDirection * fHitDistance;
	hit.fHitDistance = fHitDistance;
	hit.bIsHit = IsHitWall;
	//if(IsHitWall)
		hit.iWallType = Map[iTestX][iTestY];
}

void Ray::CastRayDDA(float fDepth , RayHit &hit)
{
	bool isHit = false;
	float fHitDistance = 0.0f;

	sf::Vector2f Pos;
	Pos.x = vOrigin.x / iTileSize;
	Pos.y = vOrigin.y / iTileSize;

	sf::Vector2i MapPos;
	MapPos.x = (int)(vOrigin.x / iTileSize);
	MapPos.y = (int)(vOrigin.y / iTileSize);

	float sideDistX, sideDistY;

	float DeltaDistX = std::abs(1.0f / vDirection.x);
	float DeltaDistY = std::abs(1.0f / vDirection.y);
	
	int stepX;
	int stepY;
	int side = 0;
	if (vDirection.x < 0)
	{
		stepX = -1;
		sideDistX = (Pos.x - MapPos.x) * DeltaDistX;
	}
	else
	{
		stepX = 1;
		sideDistX = (MapPos.x + 1.0 - Pos.x) * DeltaDistX;
	}

	if (vDirection.y < 0)
	{
		stepY = -1;
		sideDistY = (Pos.y - MapPos.y) * DeltaDistY;
	}
	else
	{
		stepY = 1;
		sideDistY = (MapPos.y + 1.0 - Pos.y) * DeltaDistY;
	}


	while (!isHit && fHitDistance < fDepth)
	{
		if (sideDistX < sideDistY)
		{
			MapPos.x += stepX;
			fHitDistance = sideDistX * iTileSize;
			sideDistX += DeltaDistX;
			side = 0;
		}
		else
		{
			MapPos.y += stepY;
			fHitDistance = sideDistY * iTileSize;
			sideDistY += DeltaDistY;
			side = 1;
		}
		if (MapPos.x >= 0 && MapPos.x < iMapWidth && MapPos.y >= 0 && MapPos.y < iMapHeight)
		{
			if (Map[MapPos.x][MapPos.y] != 0)
			{
				isHit = true;
			}
		}
	}

	fHitDistance += 0.07f;

	hit.bIsHit = isHit;
	hit.fHitDistance = fHitDistance;
	if(isHit)
		hit.iWallType = Map[MapPos.x][MapPos.y];
	hit.vHitPosition = vOrigin + vDirection * fHitDistance;
	hit.fPerpendicularDistance = (side == 0) ? (sideDistX - DeltaDistX) *iTileSize : (sideDistY - DeltaDistY) * iTileSize;


}