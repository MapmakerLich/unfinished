#include "physics.hpp"

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
            character.sword.setScale(1, 1);
            character.sword.setPosition(character.sprite.getPosition());
        }
        if (character.sprite.getScale().x == -1)
        {
            character.sword.setScale(-1, 1);
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