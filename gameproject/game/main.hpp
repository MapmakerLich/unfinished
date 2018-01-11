#pragma once

#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include <SFML\System.hpp>
#include <string>
#include <cmath>
#include <vector>
#include <iostream>

constexpr float GRAVITY = 10.0;
constexpr float JUMP_START = -500.0;
constexpr float CANON_Y = 540;
constexpr float GRENADE_MULTIPLIER = 0.15f;
constexpr unsigned WINDOW_HEIGHT = 600;
constexpr unsigned WINDOW_WIDTH = 800;
const sf::Vector2f HEAP = {2 * WINDOW_WIDTH, 2 * WINDOW_HEIGHT};
constexpr float BOT_SPAWN_TIME = 10.0;
constexpr float WIN_TIME = 900;
const float SPAWN_POINTS[10] = {40, 80, 120, 160, 200, 760, 720, 680, 640, 600};
const float BOT_SPEED = 100;
constexpr unsigned MAX_LIFES = 5;

struct Background
{
    sf::Texture texture;
    sf::Sprite sprite;
};

struct Texts
{
    sf::Font font;
    sf::Text lifeText;
    sf::Text deathText;
    sf::Text killText;
    sf::Text winText;
    sf::Text menuText;
    sf::Text controlsText;
};

struct GameCondintion
{
    bool isInMenu;
    bool isInGame;
    bool didWin;
    bool didDied;
};

struct Canon
{
    sf::CircleShape fixer;
    sf::Vector2f position;
};

struct Platform
{
    sf::Texture texture;
    sf::Sprite sprite;
    bool isCharacterOnPlatform;
    bool isBotOnPlatform;
};

struct EasyBot
{
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Vector2f position;
    sf::Vector2f speed;
    float time = 0;
    bool isAlive;
    bool isJumpable;
    bool isOnAnotherLine;
    sf::Vector2f distance;
};

struct Character
{
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Texture swordTexture;
    sf::Sprite sword;
    sf::Vector2f position;
    sf::Vector2f speed;
    bool isImmune;
    float immuneTime = 0;
    int lifeCount;
    int score;
    float timeAlive = 0;
    int isJumped;
};

struct Grenade
{
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Vector2f position;
    sf::Vector2f speed;
    float time = 0;
    bool isFlying;
};