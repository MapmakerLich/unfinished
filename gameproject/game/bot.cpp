#include "bot.hpp"

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

void botMoving(EasyBot &easyBot, float deltaTime)
{
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
}

void botJumps(EasyBot &easyBot, Platform &platform)
{
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

void botBrain(EasyBot &easyBot, Character &character, float deltaTime, Platform &platform)
{
    easyBot.distance = character.sprite.getPosition() - easyBot.sprite.getPosition();
    botMoving(easyBot, deltaTime);
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
    if ((character.sprite.getScale().x == 1) && (easyBot.distance.x < 0) && (std::abs(easyBot.distance.x) < character.swordTexture.getSize().x) && (character.sword.getPosition() == character.sprite.getPosition()) && (!easyBot.isOnAnotherLine))
    {
        easyBot.isAlive = false;
        easyBot.sprite.setPosition(HEAP);
        character.score += 1;
    }
    if ((character.sprite.getScale().x == -1) && (easyBot.distance.x > 0) && (std::abs(easyBot.distance.x) < character.swordTexture.getSize().x) && (character.sword.getPosition() == character.sprite.getPosition()) && (!easyBot.isOnAnotherLine))
    {
        easyBot.isAlive = false;
        easyBot.sprite.setPosition(HEAP);
        character.score += 1;
    }
    botJumps(easyBot, platform);
}