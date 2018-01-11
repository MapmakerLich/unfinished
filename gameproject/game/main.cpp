#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include <SFML\System.hpp>
#include <string>
#include <cmath>
#include <vector>
#include <iostream>

constexpr float GRAVITY = 10.0;
constexpr float JUMP_START = -600.0;
constexpr float CANON_Y = 540;
constexpr float GRENADE_MULTIPLIER = 0.15f;
constexpr unsigned WINDOW_HEIGHT = 600;
constexpr unsigned WINDOW_WIDTH = 800;
const sf::Vector2f HEAP = {2 * WINDOW_WIDTH, 2 * WINDOW_HEIGHT};
constexpr float BOT_SPAWN_TIME = 10.0;
constexpr float WIN_TIME = 900;
const float SPAWN_POINTS[10] = {40, 80, 120, 160, 200, 760, 720, 680, 640, 600};
const float BOT_SPEED = 100;

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

void jumping(Character &character, float deltaTime)
{
    character.speed.y += GRAVITY;
    character.position.y += character.speed.y * deltaTime;
    if (character.position.y >= WINDOW_HEIGHT - character.texture.getSize().y / 2)
    {
        character.speed.y = 0;
        character.isJumped = 0;
    }
}

void flying(Grenade &grenade, Canon &canon, float deltaTime)
{
    grenade.position = grenade.sprite.getPosition();
    grenade.time += deltaTime * 10;
    float nextY = CANON_Y + GRENADE_MULTIPLIER * JUMP_START * grenade.time + GRAVITY * std::pow(grenade.time, 2) * 0.5;
    grenade.position.y = nextY;
    if ((grenade.position.y >= WINDOW_HEIGHT - grenade.texture.getSize().y / 10) ||
        (grenade.position.y <= grenade.texture.getSize().y / 10))
    {
        grenade.time = 0;
        grenade.isFlying = false;
    }
}

void update(Character &character, Grenade &grenade, Canon &canon, float deltaTime, std::vector<EasyBot> &vectorOfBots, Texts &texts, Platform &platform)
{
    character.position = character.sprite.getPosition();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        character.speed.x = -2 * BOT_SPEED;
        character.sprite.setScale(-1, 1);
        if (character.position.x < character.texture.getSize().x / 2)
        {
            character.sprite.setScale(1, 1);
            character.speed.x = -character.speed.x;
        }
        character.position.x += character.speed.x * deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        character.speed.x = 2 * BOT_SPEED;
        character.sprite.setScale(1, 1);
        if (character.position.x > WINDOW_WIDTH - character.texture.getSize().x / 2)
        {
            character.sprite.setScale(-1, 1);
            character.speed.x = -character.speed.x;
        }
        character.position.x += character.speed.x * deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        if (character.isJumped == 0)
        {
            character.speed.y = JUMP_START;
        }
        character.isJumped = 1;
    }
    if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
    {
        if (!grenade.isFlying)
        {
            grenade.sprite.setPosition(canon.fixer.getPosition());
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
        if ((grenade.sprite.getPosition().y <= WINDOW_HEIGHT - character.texture.getSize().y / 2) &&
            (grenade.sprite.getPosition().y + character.texture.getSize().y / 2 <= platform.sprite.getPosition().y - platform.texture.getSize().y / 2))
        {
            character.position = grenade.sprite.getPosition();
            character.isJumped = 2;
            grenade.isFlying = false;
            grenade.time = 0;
            grenade.position = character.position;
        }
        else
        {
            grenade.isFlying = false;
            grenade.time = 0;
            grenade.position = character.position;
        }
    }
    if ((character.isJumped == 2) && (character.sprite.getPosition().y != WINDOW_HEIGHT - character.texture.getSize().y / 2))
    {
        character.speed.y = 0;
        character.isJumped = 1;
    }
    if ((character.position.y + character.texture.getSize().y / 2 >= platform.sprite.getPosition().y - platform.texture.getSize().y / 2) && (character.speed.y > 0))
    {
        character.isJumped = 0;
        platform.isCharacterOnPlatform = true;
    }
    if (character.isJumped == 1)
    {
        jumping(character, deltaTime);
    }
    if (grenade.isFlying)
    {
        flying(grenade, canon, deltaTime);
    }
    if ((character.isJumped == 0) && (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) && (platform.isCharacterOnPlatform))
    {
        character.position.y = WINDOW_HEIGHT - character.texture.getSize().y / 2;
        platform.isCharacterOnPlatform = false;
    }
    if ((character.position.y == WINDOW_HEIGHT - character.texture.getSize().y / 2) && (platform.isCharacterOnPlatform))
    {
        platform.isCharacterOnPlatform = false;
    }
    grenade.sprite.setPosition(grenade.position);
    character.sprite.setPosition(character.position);
    if (grenade.isFlying == false)
    {
        grenade.sprite.setPosition(canon.fixer.getPosition());
    }
    character.timeAlive += deltaTime;
    texts.lifeText.setString("Health:" + std::to_string(character.lifeCount));
    texts.killText.setString("Score:" + std::to_string((character.score * 10 / (6 - character.lifeCount)) + int(character.timeAlive)));
}

void initCharacter(Character &character)
{
    character.texture.loadFromFile("./game/release hero.png");
    character.sprite.setTexture(character.texture);
    character.sprite.setOrigin(character.texture.getSize().x / 2, character.texture.getSize().y / 2);
    character.sprite.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT - float(character.texture.getSize().y / 2));
    character.swordTexture.loadFromFile("./game/sword.png");
    character.sword.setTexture(character.swordTexture);
    character.sword.setOrigin(0, 5);
    character.lifeCount = 5;
    character.isImmune = false;
    character.score = 0;
}

void initCanon(Canon &canon)
{
    canon.fixer.setRadius(10);
    canon.fixer.setFillColor(sf::Color::Black);
    canon.fixer.setPosition({WINDOW_WIDTH / 2, CANON_Y});
    canon.fixer.setOrigin(10, 10);
    canon.position = {WINDOW_WIDTH / 2, CANON_Y};
}

void initGrenade(Grenade &grenade)
{
    grenade.texture.loadFromFile("./game/gren.png");
    grenade.sprite.setTexture(grenade.texture);
    grenade.sprite.setOrigin(grenade.texture.getSize().x / 2, grenade.texture.getSize().y / 2);
    grenade.sprite.setScale(0.2, 0.2);
    grenade.isFlying = false;
}

void initBot(EasyBot &easyBot)
{
    easyBot.texture.loadFromFile("./game/release bot.png");
    easyBot.sprite.setTexture(easyBot.texture);
    easyBot.sprite.setOrigin(easyBot.texture.getSize().x / 2, easyBot.texture.getSize().y / 2);
    easyBot.sprite.setPosition(HEAP);
    easyBot.isAlive = false;
    easyBot.speed.x = BOT_SPEED;
    easyBot.isJumpable = false;
}

void initBackground(Background &background)
{
    background.texture.loadFromFile("./resources/arena.png");
    background.sprite.setTexture(background.texture);
    background.sprite.setPosition(0, 0);
}

void spawnSomeBots(EasyBot &easyBot, float deltaTime, int i)
{
    easyBot.time += deltaTime;
    float deltaBot = remainder(easyBot.time, BOT_SPAWN_TIME);
    if (((deltaBot > -0.01) && (deltaBot < 0.01)) && (easyBot.isAlive == false))
    {
        easyBot.sprite.setPosition({SPAWN_POINTS[i], float(WINDOW_HEIGHT) - easyBot.texture.getSize().y / 2});
        easyBot.isAlive = true;
    }
}

void botBrain(EasyBot &easyBot, Character &character, float deltaTime, Platform &platform)
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
    if (easyBot.sprite.getPosition().y + easyBot.texture.getSize().y / 2 == platform.sprite.getPosition().y - platform.texture.getSize().y / 2)
    {
        platform.isBotOnPlatform = true;
    }
    else
    {
        platform.isBotOnPlatform = false;
    }
    if ((((platform.isBotOnPlatform) && (platform.isCharacterOnPlatform)) || (!(platform.isBotOnPlatform) && !(platform.isCharacterOnPlatform))) && (easyBot.isAlive))
    {
        easyBot.isOnAnotherLine = false;
    }
    else
    {
        easyBot.isOnAnotherLine = true;
    }
    if ((std::abs(easyBot.distance.x) <= (character.texture.getSize().x / 2 + easyBot.texture.getSize().x / 2)) && (std::abs(easyBot.distance.y) <= (character.texture.getSize().y / 2 + easyBot.texture.getSize().y / 2)) && (character.isImmune == false))
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
    if ((character.sprite.getScale().x == 1) && (easyBot.distance.x < 0) && (std::abs(easyBot.distance.x) < 0.4 * character.swordTexture.getSize().x) && (character.sword.getPosition() == character.sprite.getPosition()) && (!easyBot.isOnAnotherLine))
    {
        easyBot.isAlive = false;
        easyBot.sprite.setPosition(HEAP);
        character.score += 1;
    }
    if ((character.sprite.getScale().x == -1) && (easyBot.distance.x > 0) && (std::abs(easyBot.distance.x) < 0.4 * character.swordTexture.getSize().x) && (character.sword.getPosition() == character.sprite.getPosition()) && (!easyBot.isOnAnotherLine))
    {
        easyBot.isAlive = false;
        easyBot.sprite.setPosition(HEAP);
        character.score += 1;
    }
    if ((easyBot.isJumpable) && (easyBot.isOnAnotherLine) && (std::abs(easyBot.distance.x) > WINDOW_WIDTH / 4) && (easyBot.isAlive))
    {
        if (platform.isBotOnPlatform)
        {
            easyBot.position.y = WINDOW_HEIGHT - (easyBot.texture.getSize().y / 2);
            easyBot.sprite.setPosition(easyBot.position);
            platform.isBotOnPlatform = false;
        }
        if (!(platform.isBotOnPlatform))
        {
            easyBot.position.y = platform.sprite.getPosition().y - (platform.texture.getSize().y / 2) - (easyBot.texture.getSize().y / 2);
            easyBot.sprite.setPosition(easyBot.position);
            platform.isBotOnPlatform = true;
        }
    }
}

void onMouseMove(sf::Event::MouseMoveEvent &event, sf::Vector2f &mousePosition, Canon &canon)
{
    mousePosition = {float(event.x), float(event.y)};
    canon.position.x = mousePosition.x;
    canon.fixer.setPosition(canon.position);
}

void redrawFrame(sf::RenderWindow &window, std::vector<EasyBot> &vectorOfBots, Character &character, Grenade &grenade, Canon &canon, Texts &texts, Platform &platform, Background &background, GameCondintion &condition)
{
    window.clear();
    window.draw(background.sprite);
    if (condition.isInGame)
    {
        window.clear(sf::Color::White);
        background.texture.loadFromFile("./resources/ingame.png");
        background.sprite.setTexture(background.texture);
        window.draw(background.sprite);
        window.draw(platform.sprite);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            window.draw(character.sword);
        }
        window.draw(character.sprite);
        for (int i = 0; i < vectorOfBots.size(); ++i)
        {
            if (vectorOfBots[i].isAlive)
            {
                window.draw(vectorOfBots[i].sprite);
            }
        }
        if (grenade.isFlying)
        {
            window.draw(grenade.sprite);
        }
        window.draw(canon.fixer);
        window.draw(texts.lifeText);
        window.draw(texts.killText);
    }
    else
    {
        if (condition.didDied)
        {
            window.draw(texts.deathText);
            window.draw(texts.killText);
        }
        else if (condition.didWin)
        {
            window.draw(texts.winText);
            window.draw(texts.killText);
        }
    }
    window.display();
}

void initText(Texts &texts)
{
    texts.font.loadFromFile("./resources/arial.ttf");
    texts.deathText.setFont(texts.font);
    texts.deathText.setString("YOU DIED");
    texts.deathText.setCharacterSize(72);
    texts.deathText.setFillColor(sf::Color::Red);
    const sf::FloatRect deathTextBounds = texts.deathText.getLocalBounds();
    texts.deathText.setOrigin({deathTextBounds.width / 2, deathTextBounds.height / 2});
    texts.deathText.setPosition({WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2});
    texts.lifeText.setFont(texts.font);
    texts.lifeText.setPosition({0, 0});
    texts.lifeText.setCharacterSize(24);
    texts.lifeText.setFillColor(sf::Color::Black);
    texts.killText.setFont(texts.font);
    texts.killText.setCharacterSize(24);
    texts.killText.setPosition({0, 25});
    texts.killText.setFillColor(sf::Color::Green);
    texts.winText.setFont(texts.font);
    texts.winText.setString("VICTORY");
    texts.winText.setCharacterSize(72);
    texts.winText.setFillColor(sf::Color::Green);
    const sf::FloatRect winTextBounds = texts.winText.getLocalBounds();
    texts.winText.setOrigin({winTextBounds.width / 2, winTextBounds.height / 2});
    texts.winText.setPosition({WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2});
    texts.menuText.setFont(texts.font);
    texts.menuText.setString("Colosseum of Fools");
    const sf::FloatRect menuTextBounds = texts.menuText.getLocalBounds();
    texts.menuText.setOrigin({menuTextBounds.width / 2, menuTextBounds.height / 2});
    texts.menuText.setFillColor(sf::Color::Yellow);
    texts.menuText.setPosition({WINDOW_HEIGHT / 2, WINDOW_HEIGHT / 2});
}

void initPlatform(Platform &platform)
{
    platform.texture.loadFromFile("./game/platform.png");
    platform.sprite.setTexture(platform.texture);
    platform.sprite.setOrigin(platform.texture.getSize().x / 2, platform.texture.getSize().y / 2);
    platform.sprite.setPosition({WINDOW_WIDTH / 2, WINDOW_HEIGHT * 2 / 3});
    platform.isCharacterOnPlatform = false;
}

int main()
{
    sf::Clock clock;

    sf::Vector2f mousePosition;

    Character character;
    initCharacter(character);
    Grenade grenade;
    initGrenade(grenade);
    Platform platform;
    initPlatform(platform);
    Canon canon;
    initCanon(canon);
    Background background;
    initBackground(background);
    GameCondintion condition;
    std::vector<EasyBot> vectorOfBots(10);
    for (int i = 0; i < vectorOfBots.size(); ++i)
    {
        initBot(vectorOfBots[i]);
        if (i % 2 == 0)
        {
            vectorOfBots[i].isJumpable = true;
        }
    }
    Texts texts;
    initText(texts);

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Game testing screen");
    window.setFramerateLimit(60);

    condition.isInMenu = true;

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
                onMouseMove(event.mouseMove, mousePosition, canon);
            }
        }

        float deltaTime = clock.restart().asSeconds();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab))
        {
            character.sprite.setPosition({WINDOW_WIDTH / 2, WINDOW_HEIGHT - float(character.texture.getSize().y / 2)});
            character.sprite.setScale(1, 1);
            character.lifeCount = 5;
            for (int i = 0; i < vectorOfBots.size(); ++i)
            {
                vectorOfBots[i].isAlive = false;
                vectorOfBots[i].sprite.setPosition(HEAP);
                vectorOfBots[i].time = 0;
            }
            character.isJumped = 0;
            character.speed.y = 0;
            character.score = 0;
            character.timeAlive = 0;
            canon.fixer.setPosition({WINDOW_WIDTH / 2, CANON_Y});
            platform.isCharacterOnPlatform = false;
            condition.didWin = false;
            condition.didDied = false;
            condition.isInMenu = false;
        }
        if ((character.lifeCount > 0) && (character.timeAlive < WIN_TIME) && (!condition.isInMenu))
        {
            condition.isInGame = true;
            condition.didWin = false;
            condition.didDied = false;
        }
        if (character.lifeCount <= 0)
        {
            condition.didDied = true;
            condition.isInGame = false;
        }
        if (character.timeAlive >= WIN_TIME)
        {
            condition.isInGame = false;
            condition.didWin = true;
        }
        if (condition.isInGame)
        {
            update(character, grenade, canon, deltaTime, vectorOfBots, texts, platform);
            for (int i = 0; i < vectorOfBots.size(); ++i)
            {
                botBrain(vectorOfBots[i], character, deltaTime, platform);
                if (vectorOfBots.size() <= 10)
                {
                    spawnSomeBots(vectorOfBots[i], deltaTime, i);
                }
            }
        }
        redrawFrame(window, vectorOfBots, character, grenade, canon, texts, platform, background, condition);
    }
}