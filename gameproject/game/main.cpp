#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include <SFML\System.hpp>
#include <SFML\Audio.hpp>
#include <cmath>
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
    float angle;
};

struct Grenade
{
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Vector2f position;
    sf::Vector2f speed;
    float time = 0;
    bool isFlying;
    bool isStartAngleChanged;
};

void jumping(Character &character, float deltaTime, int &isJumped)
{
    character.time += deltaTime * 10;
    float nextY = character.startY + JUMP_START * character.time + GRAVITY * std::pow(character.time, 2) * 0.5;
    character.position.y = nextY;
    if (character.position.y >= WINDOW_HEIGHT - 50)
    {
        character.time = 0;
        isJumped = 0;
    }
}

void flying(Grenade &grenade, Character &character, float deltaTime, float startAngle)
{
    startAngle = character.hand.getRotation();
    grenade.position = grenade.sprite.getPosition();
    grenade.time += deltaTime * 10;
    grenade.speed.x = -2 * JUMP_START * std::cos(startAngle * M_PI / 180);
    float nextY = character.startY - 2 * JUMP_START * std::sin(startAngle * M_PI / 180) * grenade.time + GRAVITY * std::pow(grenade.time, 2) * 0.5;
    grenade.position.y = nextY;
    grenade.position.x += grenade.speed.x * deltaTime;
    if ((grenade.position.y >= WINDOW_HEIGHT - grenade.texture.getSize().y / 10) ||
        (grenade.position.x >= WINDOW_WIDTH - grenade.texture.getSize().x / 10) ||
        (grenade.position.x <= grenade.texture.getSize().x / 2) ||
        (grenade.position.y <= grenade.texture.getSize().y / 2))
    {
        grenade.time = 0;
        grenade.isFlying = false;
    }
}

void update(Character &character, Grenade &grenade, float deltaTime, int &isJumped)
{
    character.position = character.sprite.getPosition();
    float startAngle;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
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
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
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
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        isJumped = 1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab))
    {
        character.position = {WINDOW_WIDTH / 2, WINDOW_HEIGHT - 50};
        character.hand.setRotation(0);
        character.sprite.setScale(1, 1);
        isJumped = 0;
    }
    if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
    {
        if (!grenade.isFlying)
        {
            grenade.isStartAngleChanged = false;
            grenade.sprite.setPosition(character.sprite.getPosition());
            grenade.isFlying = true;
        }
        if (!grenade.isStartAngleChanged)
        {
            startAngle = character.hand.getRotation();
        }
    }
    if ((grenade.isFlying) && (sf::Keyboard::isKeyPressed(sf::Keyboard::F)))
    {
        grenade.isFlying = false;
        grenade.time = 0;
        grenade.sprite.setPosition(character.sprite.getPosition());
    }
    if (isJumped == 1)
    {
        jumping(character, deltaTime, isJumped);
    }
    if (grenade.isFlying)
    {
        flying(grenade, character, deltaTime, startAngle);
    }
    grenade.sprite.setPosition(grenade.position);
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

void initGrenade(Grenade &grenade)
{
    grenade.texture.loadFromFile("./game/gren.png");
    grenade.sprite.setTexture(grenade.texture);
    grenade.sprite.setOrigin(grenade.texture.getSize().x / 2, grenade.texture.getSize().y / 2);
    grenade.sprite.setScale(0.2, 0.2);
    grenade.isFlying = false;
}

float toDegrees(float radians)
{
    return float(double(radians) * 180.0 / M_PI);
}

void onMouseMove(sf::Event::MouseMoveEvent &event, sf::Vector2f &mousePosition, Character &character, Grenade &grenade)
{
    grenade.isStartAngleChanged = true;
    mousePosition = {float(event.x), float(event.y)};
    sf::Vector2f delta = mousePosition - character.hand.getPosition();
    character.angle = toDegrees(atan2(delta.y, delta.x));
    if (character.sprite.getScale().x > 0)
    {
        if ((std::abs(character.angle) <= MAX_HAND_ANGLE))
        {
            character.hand.setRotation(character.angle);
        }
    }
    else
    {
        if ((std::abs(character.angle) <= 180) && (std::abs(character.angle) >= 180 - MAX_HAND_ANGLE))
        {
            character.hand.setRotation(character.angle);
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
    Grenade grenade;
    initGrenade(grenade);

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
                onMouseMove(event.mouseMove, mousePosition, character, grenade);
            }
        }

        float deltaTime = clock.restart().asSeconds();

        update(character, grenade, deltaTime, isJumped);
        window.clear(sf::Color(255, 255, 255));
        window.draw(character.sprite);
        if (grenade.isFlying)
        {
            window.draw(grenade.sprite);
        }
        window.draw(character.hand);
        window.display();
    }
}