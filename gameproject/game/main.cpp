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
constexpr unsigned MAX_HAND_ANGLE = 85;

struct Character
{
    sf::Texture texture;
    sf::Sprite sprite;
    sf::RectangleShape hand;
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
    character.position = character.sprite.getPosition();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        character.speed.x = -100;
        character.sprite.setScale(-1, 1);
        if (character.position.x < 10)
        {
            character.sprite.setScale(1, 1);
            character.speed.x = -character.speed.x;
        }
        character.position += character.speed * deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        character.speed.x = 100;
        character.sprite.setScale(1, 1);
        if (character.position.x > WINDOW_WIDTH - 10)
        {
            character.sprite.setScale(-1, 1);
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
        character.hand.setRotation(0);
        character.sprite.setScale(1, 1);
        isJumped = 0;
    }
    if (isJumped == 2)
    {
        jumping(character, deltaTime, isJumped);
    }
    character.sprite.setPosition(character.position);
    character.hand.setPosition(character.position);
}

void initCharacter(Character &character)
{
    character.texture.loadFromFile("./game/cat.png");
    character.sprite.setTexture(character.texture);
    character.sprite.setOrigin(character.texture.getSize().x / 2, character.texture.getSize().y / 2);
    character.sprite.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT - 50);
    character.startY = character.sprite.getPosition().y;
    character.hand.setSize({40, 10});
    character.hand.setOrigin(0, 5);
    character.hand.setPosition(character.sprite.getPosition());
    character.hand.setFillColor(sf::Color(52, 101, 52));
    character.hand.setRotation(0);
}

float toDegrees(float radians)
{
    return float(double(radians) * 180.0 / M_PI);
}

void onMouseMove(sf::Event::MouseMoveEvent &event, sf::Vector2f &mousePosition, Character &character)
{
    mousePosition = {float(event.x), float(event.y)};
    sf::Vector2f delta = mousePosition - character.hand.getPosition();
    float angle = toDegrees(atan2(delta.y, delta.x));
    std::cout << angle << std::endl;
    if (character.sprite.getScale().x > 0)
    {
        if ((std::abs(angle) <= MAX_HAND_ANGLE))
        {
            character.hand.setRotation(angle);
        }
    }
    else
    {
        if ((std::abs(angle) <= 180) && (std::abs(angle) >= 180 - MAX_HAND_ANGLE))
        {
            character.hand.setRotation(angle);
        }
    }
}

int main()
{
    sf::Clock clock;

    int isJumped = 0;
    sf::Vector2f mousePosition;

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
            if (event.type == sf::Event::MouseMoved)
            {
                onMouseMove(event.mouseMove, mousePosition, character);
            }
        }

        float deltaTime = clock.restart().asSeconds();

        update(character, deltaTime, isJumped);
        window.clear(sf::Color(255, 255, 255));
        window.draw(character.sprite);
        window.draw(character.hand);
        window.display();
    }
}