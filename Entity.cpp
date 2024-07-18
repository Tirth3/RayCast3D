#include "Entity.h"

Entity::Entity() 
{
	vPosition = { 0.0f , 0.0f };
}

Entity::Entity(sf::Vector2f& vPos):
	vPosition(vPos)
{
}