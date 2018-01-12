#include "main.hpp"
#include "bot.hpp"
#include "init.hpp"
#include "physics.hpp"

void onMouseMove(sf::Event::MouseMoveEvent &event, sf::Vector2f &mousePosition, Canon &canon)
{
    mousePosition = {float(event.x), float(event.y)};
    canon.position.x = mousePosition.x;
    canon.fixer.setPosition(canon.position);
}

void redrawFrame(sf::RenderWindow &window, std::vector<EasyBot> &vectorOfBots, Character &character, Grenade &grenade, Canon &canon, Texts &texts, Platform &platform, Background &background, GameCondintion &condition)
{
    window.clear();
    if (condition.isInMenu)
    {
        background.texture.loadFromFile("./resources/arena.png");
        background.sprite.setTexture(background.texture);
    }
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

void pollEvents(sf::RenderWindow &window, sf::Vector2f &mousePosition, Canon &canon)
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
}

void restart(Character &character, Canon &canon, std::vector<EasyBot> &vectorOfBots, Platform &platform, GameCondintion &condition)
{
    character.sprite.setPosition({WINDOW_WIDTH / 2, WINDOW_HEIGHT - float(character.texture.getSize().y / 2)});
    character.sprite.setScale(1, 1);
    character.lifeCount = MAX_LIFES;
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

void outOfGame(GameCondintion &condition, Character &character)
{
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
    condition.isInGame = false;
    condition.didDied = false;
    condition.didWin = false;

    while (window.isOpen())
    {
        pollEvents(window, mousePosition, canon);
        float deltaTime = clock.restart().asSeconds();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab))
        {
            restart(character, canon, vectorOfBots, platform, condition);
        }
        outOfGame(condition, character);
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