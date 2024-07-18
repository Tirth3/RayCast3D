/*
    TODO1 : Add textured walls
    TODO2 : Implement DDA algorithm
*/
#include <SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<fstream>
#include<iostream>
#include<chrono>
#include"Ray.h"
#include"Player.h"
#include "Helper.h"

Player *player;
sf::Texture WallTexture;
sf::Texture EntityTexture;
sf::Texture BGTexture;
Entity Steve;
int Map[iMapWidth][iMapHeight];

bool DrawUI = true;
bool DrawTexturedWalls = false;
float fMiniMapSize = 200.0f;
std::string sMapAddress = "Map.txt";

int main()
{
    sf::Texture UITexture;
    float UIangle = 0;
    float ButtonTime = 10.0f;
    float ButtonTimeCounter = 0.0f;
    if (!font.loadFromFile("./Assets/Arialn.ttf"))
        std::cout << "[MAIN] Cannot load font";
    if (!UITexture.loadFromFile("Images/GameUI1.png"))
        std::cout << "[MAIN] Cannot load ui texture";
    if (!BGTexture.loadFromFile("Images/Background1.png"))
        std::cout << "[MAIN] Cannot load BG texture";

    sf::Sprite UIsprite;
    UIsprite.setTexture(UITexture);
    UIsprite.setPosition(0, 0);
    UIsprite.setScale((iHalfScreenWidth / (float)UITexture.getSize().x), 1.0f);
    sf::Vector2f PlayerPos;

    float fps = 0;
    sf::RenderWindow window(sf::VideoMode(iHalfScreenWidth , iScreenHeight), "3D but 2D world!");
    //window.setFramerateLimit(60);
    LoadMap(sMapAddress , PlayerPos);

    player = new Player(PlayerPos , 0.0f);

    sf::Clock deltaClock;
    std::chrono::high_resolution_clock::time_point start;
    std::chrono::high_resolution_clock::time_point end;

    while (window.isOpen())
    {
        start = std::chrono::high_resolution_clock::now();

        sf::Time deltatime = deltaClock.restart();
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window.close();

            if (ButtonTimeCounter >= ButtonTime && sf::Keyboard::isKeyPressed(sf::Keyboard::B))
            {
                ButtonTimeCounter = 0.0f;
                DrawTexturedWalls = !DrawTexturedWalls;
            }
            if (ButtonTimeCounter >= ButtonTime && sf::Keyboard::isKeyPressed(sf::Keyboard::V))
            {
                ButtonTimeCounter = 0.0f;
                DrawUI = !DrawUI;
            }
        }

        // Updating Player
        player->Update(deltatime.asSeconds());
        
        window.clear();  // Clearing screen
        DrawSky(window);  // Rendering sky and floor in BG

        player->CastRays(window , DrawTexturedWalls);  // Casting rays and draw the wall from player
        //player->DrawMiniMap({ 10.0f , 30.0f } , window , fMiniMapSize);  // Drawing Minimap

        //DrawText(window, font, sf::Vector2f (10.0f , 10.0f ), fps, 16);  // Displaying the current fps
        ButtonTimeCounter++;
        float yOffset = std::sin(UIangle * 0.5f) * 5.0f;
        if(player->bIsMoving)
            UIsprite.setPosition(0.0f, 10.0f + yOffset);
        if(DrawUI)
            window.draw(UIsprite);
        window.display(); // Updating the window
        UIangle += 0.1f;
        // Calculating fps
        end = std::chrono::high_resolution_clock::now();
        fps = (float)1e9 / (float)std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    }

    return 0;
}