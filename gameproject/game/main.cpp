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
constexpr float botSpawnTime = 10.0;
sf::Vector2f spawnPoint = {100, 430};

struct Ezbot
{
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Vector2f position;
    sf::Vector2f speed;
    float time = 0;
    bool isAlive;
    sf::Vector2f distanse;
};

struct Character
{
    sf::Texture texture;
    sf::Sprite sprite;
    sf::RectangleShape hand;
    sf::Texture swordTexture;
    sf::Sprite sword;
    sf::Vector2f position;
    sf::Vector2f speed;
    bool isImmune;
    float time = 0;
    float startY;
    float angle;
    float immuneTime = 0;
    int life_count;
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
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        if (character.sprite.getScale().x == 1)
        {
            character.sword.setScale(0.4, 0.4);
            character.sword.setPosition(character.sprite.getPosition());
        }
        if (character.sprite.getScale().x == -1)
        {
            character.sword.setScale(-0.4, 0.4);
            character.sword.setPosition(character.sprite.getPosition());
        }
    }
    else
    {
        character.sword.setPosition(200, 200);
    }
    if ((grenade.isFlying) && (sf::Keyboard::isKeyPressed(sf::Keyboard::F)))
    {
        character.sprite.setPosition(grenade.sprite.getPosition());
        character.hand.setPosition(grenade.sprite.getPosition());
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
    character.swordTexture.loadFromFile("./game/sword.png");
    character.sword.setTexture(character.swordTexture);
    character.sword.setOrigin(0, 5);
    character.life_count = 5;
    character.isImmune = false;
}

void initGrenade(Grenade &grenade)
{
    grenade.texture.loadFromFile("./game/gren.png");
    grenade.sprite.setTexture(grenade.texture);
    grenade.sprite.setOrigin(grenade.texture.getSize().x / 2, grenade.texture.getSize().y / 2);
    grenade.sprite.setScale(0.2, 0.2);
    grenade.isFlying = false;
}

void initBot(Ezbot &ezbot)
{
    ezbot.texture.loadFromFile("./game/another cat.png");
    ezbot.sprite.setTexture(ezbot.texture);
    ezbot.sprite.setOrigin(ezbot.texture.getSize().x / 2, ezbot.texture.getSize().y / 2);
    ezbot.sprite.setPosition(100, 100);
    ezbot.isAlive = false;
    ezbot.speed.x = 50;
}

void spawnSomeBotz(Ezbot &ezbot, float deltaTime, sf::Vector2f spawnPoint)
{
    ezbot.time += deltaTime;
    float deltaBot = remainder(ezbot.time, botSpawnTime);
    if (((deltaBot > -0.01) && (deltaBot < 0.01)) && (ezbot.isAlive == false))
    {
        ezbot.sprite.setPosition(spawnPoint);
        ezbot.isAlive = true;
    }
}

void botBrain(Ezbot &ezbot, Character &character, float deltaTime)
{
    ezbot.distanse.x = character.sprite.getPosition().x - ezbot.sprite.getPosition().x;
    if ((ezbot.isAlive == true) && (ezbot.distanse.x != 0))
    {
        ezbot.position = ezbot.sprite.getPosition();
        if (ezbot.distanse.x < 0)
        {
            ezbot.sprite.setScale(-1, 1);
            ezbot.position.x = ezbot.position.x - ezbot.speed.x * deltaTime;
        }
        else
        {
            if (ezbot.sprite.getScale().x == -1)
            {
                ezbot.sprite.setScale(1, 1);
            };
            ezbot.position.x = ezbot.position.x + ezbot.speed.x * deltaTime;
        }
        ezbot.sprite.setPosition(ezbot.position);
    }
    if ((std::abs(ezbot.distanse.x) <= (character.texture.getSize().x + ezbot.texture.getSize().x)) && (character.isImmune == false))
    {
        character.life_count = character.life_count - 1;
        character.isImmune = true;
    }
    if ((character.immuneTime >= 0) && (character.immuneTime <= 3.0) && (character.isImmune == true))
    {
        character.immuneTime += deltaTime;
    }
    else
    {
        character.isImmune = false;
        character.immuneTime = 0;
    }
    if ((character.sprite.getScale().x == 1) && (ezbot.distanse.x < 0) && (std::abs(ezbot.distanse.x) < 0.4 * character.swordTexture.getSize().x) && (character.sword.getPosition() == character.sprite.getPosition()))
    {
        ezbot.isAlive = false;
        ezbot.sprite.setPosition(100, 100);
    }
    if ((character.sprite.getScale().x == -1) && (ezbot.distanse.x > 0) && (std::abs(ezbot.distanse.x) < 0.4 * character.swordTexture.getSize().x) && (character.sword.getPosition() == character.sprite.getPosition()))
    {
        ezbot.isAlive = false;
        ezbot.sprite.setPosition(100, 100);
    }
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

    Ezbot ezbot;
    initBot(ezbot);

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Game testing screen");
    window.setFramerateLimit(60);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if ((event.type == sf::Event::Closed) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) || (character.life_count <= 0))
            {
                window.close();
            }
            if (event.type == sf::Event::MouseMoved)
            {
                onMouseMove(event.mouseMove, mousePosition, character, grenade);
            }
        }

        float deltaTime = clock.restart().asSeconds();

        spawnSomeBotz(ezbot, deltaTime, spawnPoint);
        update(character, grenade, deltaTime, isJumped);
        botBrain(ezbot, character, deltaTime);
        window.clear(sf::Color(255, 255, 255));
        window.draw(character.sprite);
        if (ezbot.isAlive)
        {
            window.draw(ezbot.sprite);
        }
        if (grenade.isFlying)
        {
            window.draw(grenade.sprite);
        }
        window.draw(character.hand);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            window.draw(character.sword);
        }
        window.display();
    }
}