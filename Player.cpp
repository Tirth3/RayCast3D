#include "Player.h"

Player::Player(sf::Vector2f pos, float angle) :
	vPosition(pos) , fAngle(angle)
{
	vDirection.x = cos(DegreeToRadians(angle));
	vDirection.y = sin(DegreeToRadians(angle));
}

void Player::HandleInput(float deltatime) {
	sf::Vector2f vDisplacement = { 0.0f , 0.0f };
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) { // Moving in direction facing
		vDisplacement.x += vDirection.x * fVelocity * deltatime;
		vDisplacement.y += vDirection.y * fVelocity * deltatime;
		bIsMoving = true;
	}

	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) { // Moving in direction facing opposite direction
		vDisplacement.x -= vDirection.x * fVelocity * deltatime;
		vDisplacement.y -= vDirection.y * fVelocity * deltatime;
		bIsMoving = true;
	}
	else
		bIsMoving = false;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {  // Changing facing angle
		fAngle -= 100.0f * deltatime;

	}

	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {  // Changing facing angle
		fAngle += 100.0f * deltatime;
	}

	vDirection.x = cos(DegreeToRadians(fAngle));
	vDirection.y = sin(DegreeToRadians(fAngle));

	int iTestX = (int)(vDisplacement.x + vPosition.x) / iTileSize;
	int iTestY = (int)(vDisplacement.y + vPosition.y) / iTileSize;
	if(Map[iTestX][iTestY] == 0){
		vPosition += vDisplacement;
	}
}

void Player::Draw2D(sf::RenderWindow& window)
{
	sf::CircleShape circle;
	float radius = 5.0f;
	circle.setOrigin(radius, radius);
	circle.setPosition(vPosition + vOffset2D);
	circle.setRadius(radius);
	circle.setFillColor(sf::Color::Red);

	window.draw(circle);

	//DrawLine(vOffset2D + vPosition, vOffset2D + vPosition + vDirection * 20.0f, window);
}

void Player::Update(float deltatime) 
{
	HandleInput(deltatime);
}

void Player::CastRays(sf::RenderWindow &window , bool drawtextwalls)
{
	
	// Verticle projection distance
	float fProjectionDistance = (fMaxWallHeight * 0.5f) / tan(DegreeToRadians(fHalfFOV));

	for (float x = 0; x <= iHalfScreenWidth; x++) 
	{
		float fCurrentAngle = (fAngle - fHalfFOV) + (x / iHalfScreenWidth) * fHalfFOV * 2.0f;

		Ray ray(vPosition, fCurrentAngle);
		RayHit hit; 
		ray.CastRayDDA(fDepth , hit);
		// uncomment below two line to use trivial raycasting approach
		//ray.CastRay(fDepth , vDirection , hit);
		//hit.fPerpendicularDistance = hit.fHitDistance * std::abs(cos(DegreeToRadians(fAngle - fCurrentAngle)));

		// calculate the height of wall according to hit distance
		float fWallHeight = fMaxWallHeight * fProjectionDistance / hit.fPerpendicularDistance;
		sf::Vector2f vWallPos = { (float)x, (iScreenHeight - fWallHeight) * 0.5f };
		sf::Vector2f vWallSize = { 1 , fWallHeight };
		
		if (hit.bIsHit)
		{
			float fog = 1 - (hit.fHitDistance / fDepth); // Alpaha value for wall
			
			//Drawing walls
			if(!drawtextwalls) // Draw static single color walls
				DrawWallStrip(vWallPos, vWallSize, hit.iWallType, fog , window);
			else  // Draw textured walls
			{
				sf::Vector2f UVPos = { 0.0f , 0.0f };
				sf::Vector2f vHitTilePos;//  = hit.vHitPosition / (float)iTileSize;
				vHitTilePos.x = (int)(hit.vHitPosition.x / iTileSize);
				vHitTilePos.y = (int)(hit.vHitPosition.y / iTileSize);

				vHitTilePos.x *= iTileSize;
				vHitTilePos.y *= iTileSize;

				vHitTilePos.x += iTileSize / 2;
				vHitTilePos.y += iTileSize / 2;

				sf::Vector2f vPOVector = hit.vHitPosition - vHitTilePos;
				float fPOAngle = std::atan2(vPOVector.y, vPOVector.x);

				if (fPOAngle >= -3.1415f * 0.25f && fPOAngle < 3.1415f * 0.25f)
				{
					UVPos.x = (hit.vHitPosition.y / iTileSize) - (int)(hit.vHitPosition.y / iTileSize);
					DrawWallStrip(vWallPos, vWallSize, 1 , 1 , window);
				}
				if (fPOAngle >= 3.1415f * 0.25f && fPOAngle < 3.1415f * 0.75f)
				{
					UVPos.x = (hit.vHitPosition.x / iTileSize) - (int)(hit.vHitPosition.x / iTileSize);
					DrawWallStrip(vWallPos, vWallSize, 2, 1, window);
				}
				if (fPOAngle < -3.1415f * 0.25f && fPOAngle > -3.1415f * 0.75f)
				{
					UVPos.x = (hit.vHitPosition.x / iTileSize) - (int)(hit.vHitPosition.x / iTileSize);
					DrawWallStrip(vWallPos, vWallSize, 3, 1, window);
				}
				if (fPOAngle >= 3.1415f * 0.75f || fPOAngle < -3.1415f * 0.75f)
				{
					UVPos.x = (hit.vHitPosition.y / iTileSize) - (int)(hit.vHitPosition.y / iTileSize);
					DrawWallStrip(vWallPos, vWallSize, 4, 1, window);
				}
				
				sf::Sprite sprite;
				sprite.setPosition(vWallPos);
				sprite.setTexture(WallTexture);
				
				float s = fWallHeight / fMaxWallHeight;
				sprite.setScale(1, s);
				sprite.setTextureRect(sf::IntRect(UVPos.x * fMaxWallHeight, UVPos.y, vWallSize.x, vWallSize.y));
				window.draw(sprite);
			}
		}
		else
		{
			//DrawWallStrip(vWallPos, vWallSize, sf::Color(0 , 0 , 0 , 200), window);
		}
	}
}

void Player::DrawMiniMap(sf::Vector2f vPos, sf::RenderWindow& window , float fMiniMapSize) 
{

	float fMiniMapTileSize = fMiniMapSize / iMapWidth;
	vPos += vOffset2D;
	sf::RectangleShape mapbg;
	mapbg.setPosition(vPos);
	mapbg.setFillColor(sf::Color(100, 100, 100));
	mapbg.setSize({ fMiniMapSize, fMiniMapSize });
	window.draw(mapbg);
	sf::Vector2f VMiniMapPlayerPos = { (vPosition.x / iHalfScreenWidth) * fMiniMapSize + vPos.x , (vPosition.y / iHalfScreenWidth) * fMiniMapSize + vPos.y };

	for (int y = 0; y < iMapHeight; y++) {
		for (int x = 0; x < iMapWidth; x++) {
			if (Map[x][y] == 1) {
				sf::RectangleShape square;
				square.setPosition(vPos.x + x * fMiniMapTileSize, vPos.y + y * fMiniMapTileSize);
				square.setSize(sf::Vector2f(fMiniMapTileSize, fMiniMapTileSize));
				square.setFillColor(sf::Color::Red);
				square.setOutlineThickness(1);
				square.setOutlineColor(sf::Color::White);
				window.draw(square);
			}
			else if (Map[x][y] == 2) {
				sf::RectangleShape square;
				square.setPosition(vPos.x + x * fMiniMapTileSize, vPos.y + y * fMiniMapTileSize);
				square.setSize(sf::Vector2f(fMiniMapTileSize, fMiniMapTileSize));
				square.setFillColor(sf::Color::Green);
				square.setOutlineThickness(1);
				square.setOutlineColor(sf::Color::White);
				window.draw(square);
			}
			else if (Map[x][y] == 3) {
				sf::RectangleShape square;
				square.setPosition(vPos.x + x * fMiniMapTileSize, vPos.y + y * fMiniMapTileSize);
				square.setSize(sf::Vector2f(fMiniMapTileSize, fMiniMapTileSize));
				square.setFillColor(sf::Color::Blue);
				square.setOutlineThickness(1);
				square.setOutlineColor(sf::Color::White);
				window.draw(square);
			}
			else if (Map[x][y] == 5) {
				sf::RectangleShape square;
				square.setPosition(vPos.x + x * fMiniMapTileSize, vPos.y + y * fMiniMapTileSize);
				square.setSize(sf::Vector2f(fMiniMapTileSize, fMiniMapTileSize));
				square.setFillColor(sf::Color::Blue);
				square.setOutlineThickness(1);
				square.setOutlineColor(sf::Color::Magenta);
				window.draw(square);
			}
		}
	}

	sf::CircleShape circle;
	circle.setOrigin(fMiniMapTileSize, fMiniMapTileSize);
	circle.setRadius(fMiniMapTileSize);
	circle.setPosition(VMiniMapPlayerPos);
	circle.setPointCount(3);
	circle.setFillColor(sf::Color::Yellow);
	//circle.setRotation(fAngle);
	window.draw(circle);
}
