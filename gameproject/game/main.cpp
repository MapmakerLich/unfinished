#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include <SFML\System.hpp>
#include <string>
#include <cmath>
#include <vector>
#include <iostream>

constexpr float GRAVITY = 10.0;
constexpr float JUMP_START = -50.0;
const sf::Vector2f GRENADE_MULTIPILER = {-3.f, -1.f};
const sf::Vector2f HEAP = {800, 800};
constexpr unsigned WINDOW_HEIGHT = 600;
constexpr unsigned WINDOW_WIDTH = 800;
constexpr unsigned MAX_HAND_ANGLE = 85;
constexpr float BOT_SPAWN_TIME = 10.0;
sf::Vector2f SPAWN_POINTS[5] = {{100, 560}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};

struct easyBot
{
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Vector2f position;
    sf::Vector2f speed;
    float time = 0;
    bool isAlive;
    sf::Vector2f distance;
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
    int lifeCount;
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

void jumping(Character &character, float deltaTime, int &isJumped)
{
    character.time += deltaTime * 10;
    float nextY = character.startY + JUMP_START * character.time + GRAVITY * std::pow(character.time, 2) * 0.5;
    character.position.y = nextY;
    if (character.position.y >= WINDOW_HEIGHT - character.texture.getSize().y / 2)
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
    grenade.speed.x = GRENADE_MULTIPILER.x * JUMP_START * std::cos(startAngle * M_PI / 180);
    float nextY = character.startY + GRENADE_MULTIPILER.y * JUMP_START * std::sin(startAngle * M_PI / 180) * grenade.time + GRAVITY * std::pow(grenade.time, 2) * 0.5;
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

void update(Character &character, Grenade &grenade, float deltaTime, int &isJumped, easyBot &bot, sf::Text &lifeText)
{
    character.position = character.sprite.getPosition();
    float startAngle;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        character.speed.x = -100;
        character.sprite.setScale(-1, 1);
        if (character.position.x < character.texture.getSize().x / 2)
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
        if (character.position.x > WINDOW_WIDTH - character.texture.getSize().x / 2)
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
        character.position = {WINDOW_WIDTH / 2, WINDOW_HEIGHT - float(character.texture.getSize().y / 2)};
        character.hand.setRotation(0);
        character.sprite.setScale(1, 1);
        character.lifeCount = 5;
        bot.isAlive = false;
        bot.sprite.setPosition(HEAP);
        isJumped = 0;
    }
    if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
    {
        if (!grenade.isFlying)
        {
            grenade.sprite.setPosition(character.sprite.getPosition());
            grenade.isFlying = true;
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
        character.sword.setPosition(HEAP);
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
    lifeText.setString("Health:" + std::to_string(character.lifeCount));
}

void initCharacter(Character &character)
{
    character.texture.loadFromFile("./game/cat.png");
    character.sprite.setTexture(character.texture);
    character.sprite.setOrigin(character.texture.getSize().x / 2, character.texture.getSize().y / 2);
    character.sprite.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT - float(character.texture.getSize().y / 2));
    character.startY = character.sprite.getPosition().y;
    character.hand.setSize({30, 10});
    character.hand.setOrigin(0, 5);
    character.hand.setPosition(character.sprite.getPosition());
    character.hand.setFillColor(sf::Color(52, 101, 52));
    character.hand.setRotation(0);
    character.swordTexture.loadFromFile("./game/sword.png");
    character.sword.setTexture(character.swordTexture);
    character.sword.setOrigin(0, 5);
    character.lifeCount = 5;
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

void initBot(easyBot &easyBot)
{
    easyBot.texture.loadFromFile("./game/another cat.png");
    easyBot.sprite.setTexture(easyBot.texture);
    easyBot.sprite.setOrigin(easyBot.texture.getSize().x / 2, easyBot.texture.getSize().y / 2);
    easyBot.sprite.setPosition(HEAP);
    easyBot.isAlive = false;
    easyBot.speed.x = 50;
}

void spawnSomeBots(easyBot &easyBot, float deltaTime)
{
    easyBot.time += deltaTime;
    float deltaBot = remainder(easyBot.time, BOT_SPAWN_TIME);
    if (((deltaBot > -0.01) && (deltaBot < 0.01)) && (easyBot.isAlive == false))
    {
        easyBot.sprite.setPosition(SPAWN_POINTS[0]);
        easyBot.isAlive = true;
    }
}

void botBrain(easyBot &easyBot, Character &character, float deltaTime)
{
    easyBot.distance = character.sprite.getPosition() - easyBot.sprite.getPosition();
    if ((easyBot.isAlive == true) && (easyBot.distance.x != 0))
    {
        easyBot.position = easyBot.sprite.getPosition();
        if (easyBot.distance.x < 0)
        {
            easyBot.sprite.setScale(-1, 1);
            easyBot.position.x = easyBot.position.x - easyBot.speed.x * deltaTime;
        }
        else
        {
            if (easyBot.sprite.getScale().x == -1)
            {
                easyBot.sprite.setScale(1, 1);
            };
            easyBot.position.x = easyBot.position.x + easyBot.speed.x * deltaTime;
        }
        easyBot.sprite.setPosition(easyBot.position);
    }
    if ((std::abs(easyBot.distance.x) <= (character.texture.getSize().x + easyBot.texture.getSize().x)) && (character.isImmune == false))
    {
        character.lifeCount = character.lifeCount - 1;
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
    if ((character.sprite.getScale().x == 1) && (easyBot.distance.x < 0) && (std::abs(easyBot.distance.x) < 0.4 * character.swordTexture.getSize().x) && (character.sword.getPosition() == character.sprite.getPosition()))
    {
        easyBot.isAlive = false;
        easyBot.sprite.setPosition(HEAP);
    }
    if ((character.sprite.getScale().x == -1) && (easyBot.distance.x > 0) && (std::abs(easyBot.distance.x) < 0.4 * character.swordTexture.getSize().x) && (character.sword.getPosition() == character.sprite.getPosition()))
    {
        easyBot.isAlive = false;
        easyBot.sprite.setPosition(HEAP);
    }
}

float toDegrees(float radians)
{
    return float(double(radians) * 180.0 / M_PI);
}

void onMouseMove(sf::Event::MouseMoveEvent &event, sf::Vector2f &mousePosition, Character &character, Grenade &grenade)
{
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
    easyBot easyBot;
    initBot(easyBot);
    sf::Font font;
    font.loadFromFile("./resources/arial.ttf");
    sf::Text deathText;
    deathText.setFont(font);
    deathText.setString("YOU DIED");
    deathText.setCharacterSize(72);
    deathText.setFillColor(sf::Color::Red);
    const sf::FloatRect deathTextBounds = deathText.getLocalBounds();
    deathText.setOrigin({deathTextBounds.width / 2, deathTextBounds.height / 2});
    deathText.setPosition({WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2});
    sf::Text lifeText;
    lifeText.setFont(font);
    lifeText.setPosition({0, 0});
    lifeText.setCharacterSize(24);
    lifeText.setFillColor(sf::Color::Black);

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

        spawnSomeBots(easyBot, deltaTime);
        update(character, grenade, deltaTime, isJumped, easyBot, lifeText);
        botBrain(easyBot, character, deltaTime);
        window.clear(sf::Color::Black);
        if (character.lifeCount > 0)
        {
            window.clear(sf::Color::White);
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                window.draw(character.sword);
            }
            window.draw(character.sprite);
            if (easyBot.isAlive)
            {
                window.draw(easyBot.sprite);
            }
            if (grenade.isFlying)
            {
                window.draw(grenade.sprite);
            }
            window.draw(character.hand);
            window.draw(lifeText);
        }
        else
        {
            window.draw(deathText);
        }
        window.display();
    }
}