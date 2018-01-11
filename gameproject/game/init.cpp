#include "init.hpp"

void initCharacter(Character &character)
{
    character.texture.loadFromFile("./resources/release hero.png");
    character.sprite.setTexture(character.texture);
    character.sprite.setOrigin(character.texture.getSize().x / 2, character.texture.getSize().y / 2);
    character.sprite.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT - float(character.texture.getSize().y / 2));
    character.swordTexture.loadFromFile("./resources/sword.png");
    character.sword.setTexture(character.swordTexture);
    character.sword.setOrigin(0, character.swordTexture.getSize().y / 2);
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
    grenade.texture.loadFromFile("./resources/gren.png");
    grenade.sprite.setTexture(grenade.texture);
    grenade.sprite.setOrigin(grenade.texture.getSize().x / 2, grenade.texture.getSize().y / 2);
    grenade.isFlying = false;
}

void initBot(EasyBot &easyBot)
{
    easyBot.texture.loadFromFile("./resources/release bot.png");
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
    texts.killText.setPosition({0, float(texts.lifeText.getCharacterSize()) + 1});
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
    platform.texture.loadFromFile("./resources/platform.png");
    platform.sprite.setTexture(platform.texture);
    platform.sprite.setOrigin(platform.texture.getSize().x / 2, platform.texture.getSize().y / 2);
    platform.sprite.setPosition({WINDOW_WIDTH / 2, WINDOW_HEIGHT * 2 / 3});
    platform.isCharacterOnPlatform = false;
}