#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include <SFML\System.hpp>
#include <SFML\Audio.hpp>
#include <cmath>
#include <conio.h>
#include <iostream>

constexpr float GRAVITY = 10.0;
constexpr float JUMP_START = -40.0;
constexpr unsigned WINDOW_HEIGHT = 480;
constexpr unsigned WINDOW_WIDTH = 640;

struct Character
{
    sf::RectangleShape body;
    sf::Vector2f position;
    sf::Vector2f speed;
    float time = 0;
    float startY;
};

void jumping(Character &character, float deltaTime, int &isJumped)
{
    character.time += deltaTime * 10;
    float nextY = character.startY + JUMP_START * character.time + GRAVITY * std::pow(character.time, 2) * 0.5;
    character.position.y = nextY;
    if (character.position.y >= WINDOW_HEIGHT - 50)
    {
        character.time = 0;
        isJumped = 1;
    }
}

void update(Character &character, float deltaTime, int &isJumped)
{
    character.position = character.body.getPosition();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        character.speed.x = -100;
        if (character.position.x < 10)
        {
            character.speed.x = -character.speed.x;
        }
        character.position += character.speed * deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        character.speed.x = 100;
        if (character.position.x > WINDOW_WIDTH - 10)
        {
            character.speed.x = -character.speed.x;
        }
        character.position += character.speed * deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        isJumped = 2;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab))
    {
        character.position = {WINDOW_WIDTH / 2, WINDOW_HEIGHT - 50};
        isJumped = 0;
    }
    if (isJumped == 2)
    {
        jumping(character, deltaTime, isJumped);
    }
    character.body.setPosition(character.position);
}

void initCharacter(Character &character)
{
    character.body.setSize({20, 100});
    character.body.setOrigin(10, 50);
    character.body.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT - 50);
    character.body.setRotation(0);
    character.body.setFillColor(sf::Color(101, 52, 101));
    character.startY = character.body.getPosition().y;
}

int main()
{
    sf::Clock clock;

    int isJumped = 0;

    Character character;
    initCharacter(character);

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Game testing screen");
    window.setFramerateLimit(60);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if ((event.type == sf::Event::Closed) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)))
            {
                window.close();
            }
        }

        float deltaTime = clock.restart().asSeconds();

        update(character, deltaTime, isJumped);
        window.clear(sf::Color(255, 255, 255));
        window.draw(character.body);
        window.display();
    }
}