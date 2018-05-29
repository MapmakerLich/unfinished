#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include <SFML\System.hpp>
#include <cmath>
#include <random>
#include <cassert>
#include <ctime>
#include <iostream>

const unsigned WINDOW_WIDTH = 800;
const unsigned WINDOW_HEIGHT = 600;
const unsigned PADDLE_LENGTH = WINDOW_HEIGHT / 10;
const unsigned PADDLE_WIDTH = WINDOW_WIDTH / 60;
const unsigned BALL_RADIUS = 10;
const unsigned PADDLE_SPEED = 300;
const unsigned BALL_MULTIPLIER = 20;

struct PRNG
{
    std::mt19937 engine;
};

struct Paddle
{
    int speedY;
    sf::RectangleShape paddle;
    sf::Vector2f position;
};

struct Ball
{
    sf::Vector2f speed;
    sf::Vector2f position;
    sf::CircleShape ball;
};

struct Texts
{
    sf::Font font;
    sf::Text menuText;
    sf::Text leftVictoryText;
    sf::Text rightVictoryText;
};

struct GameCondition
{
    bool isInMenu;
    bool isInGame;
    bool isLeftWon;
    bool isRightWon;
};

void initGenerator(PRNG& generator)
{
    std::random_device device;
    generator.engine.seed(device());
}

void initPaddles(Paddle& leftPaddle, Paddle& rightPaddle)
{
    leftPaddle.paddle.setSize({PADDLE_WIDTH, PADDLE_LENGTH});
    leftPaddle.paddle.setFillColor(sf::Color::White);
    leftPaddle.paddle.setOrigin(PADDLE_WIDTH / 2, PADDLE_LENGTH / 2);
    leftPaddle.paddle.setPosition(PADDLE_WIDTH / 2, WINDOW_HEIGHT / 2);
    leftPaddle.speedY = 0;

    rightPaddle.paddle.setSize({PADDLE_WIDTH, PADDLE_LENGTH});
    rightPaddle.paddle.setFillColor(sf::Color::White);
    rightPaddle.paddle.setOrigin(PADDLE_WIDTH / 2, PADDLE_LENGTH / 2);
    rightPaddle.paddle.setPosition(WINDOW_WIDTH - (PADDLE_WIDTH / 2), WINDOW_HEIGHT / 2);
    rightPaddle.speedY = 0;
}

void initTexts(Texts& texts)
{
    texts.font.loadFromFile("./game/arial.ttf");
    texts.menuText.setFont(texts.font);
    texts.menuText.setString("Space for start, Esc for exit");
    texts.menuText.setCharacterSize(48);
    texts.menuText.setFillColor(sf::Color::White);
    const sf::FloatRect menuTextBounds = texts.menuText.getLocalBounds();
    texts.menuText.setOrigin({menuTextBounds.width / 2, menuTextBounds.height / 2});
    texts.menuText.setPosition({WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2});
    
    texts.leftVictoryText.setFont(texts.font);
    texts.leftVictoryText.setString("Left player wins");
    texts.leftVictoryText.setCharacterSize(48);
    texts.leftVictoryText.setFillColor(sf::Color::White);
    const sf::FloatRect leftVictoryTextBounds = texts.leftVictoryText.getLocalBounds();
    texts.leftVictoryText.setOrigin({leftVictoryTextBounds.width / 2, leftVictoryTextBounds.height / 2});
    texts.leftVictoryText.setPosition({WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2});
    
    texts.rightVictoryText.setFont(texts.font);
    texts.rightVictoryText.setString("Right player wins");
    texts.rightVictoryText.setCharacterSize(48);
    texts.rightVictoryText.setFillColor(sf::Color::White);
    const sf::FloatRect rightVictoryTextTextBounds = texts.rightVictoryText.getLocalBounds();
    texts.rightVictoryText.setOrigin({rightVictoryTextTextBounds.width / 2, rightVictoryTextTextBounds.height / 2});
    texts.rightVictoryText.setPosition({WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2});
}

float randomFloat(PRNG& generator, float minValue, float maxValue)
{
    assert(minValue < maxValue);
    std::uniform_real_distribution<float> distribution(minValue, maxValue);
    return distribution(generator.engine);
}

void initBall(Ball& ball, PRNG& generator)
{
    ball.ball.setRadius(BALL_RADIUS);
    ball.ball.setOrigin(BALL_RADIUS, BALL_RADIUS);
    ball.ball.setFillColor(sf::Color::White);
    ball.ball.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    ball.speed.x = randomFloat(generator, 0 - BALL_MULTIPLIER * std::sqrt(PADDLE_SPEED), BALL_MULTIPLIER * std::sqrt(PADDLE_SPEED));
    ball.speed.y = randomFloat(generator, 0 - BALL_MULTIPLIER * std::sqrt(PADDLE_SPEED), BALL_MULTIPLIER * std::sqrt(PADDLE_SPEED));
}

void ballPhysics(Ball& ball, Paddle& leftPaddle, Paddle& rightPaddle, float deltaTime, GameCondition& condition)
{
    ball.position = ball.ball.getPosition();
    ball.position += ball.speed * deltaTime;
    leftPaddle.position = leftPaddle.paddle.getPosition();
    rightPaddle.position = rightPaddle.paddle.getPosition();
    if (ball.position.x < BALL_RADIUS)
    {
        ball.speed.x = 0;
        ball.position.x = BALL_RADIUS;
        condition.isRightWon = true;
        condition.isInGame = false;
    }
    if (ball.position.x > WINDOW_WIDTH - BALL_RADIUS)
    {
        ball.speed.x = 0;
        ball.position.x = WINDOW_WIDTH - BALL_RADIUS;
        condition.isLeftWon = true;
        condition.isInGame = false;
    }
    if (ball.position.y < BALL_RADIUS)
    {
        ball.speed.y = -ball.speed.y;
        ball.position.y = BALL_RADIUS;
    }
    if (ball.position.y > WINDOW_HEIGHT - BALL_RADIUS)
    {
        ball.speed.y = -ball.speed.y;
        ball.position.y = WINDOW_HEIGHT - BALL_RADIUS;
    }
    if (ball.position.x - BALL_RADIUS < leftPaddle.position.x + (PADDLE_WIDTH / 2) && ball.position.y + BALL_RADIUS >= leftPaddle.position.y - (PADDLE_LENGTH / 2) && ball.position.y - BALL_RADIUS <= leftPaddle.position.y + (PADDLE_LENGTH / 2))
    {
        ball.speed.x = -ball.speed.x;
        ball.position.x = BALL_RADIUS + PADDLE_WIDTH / 2;
    }
    if (ball.position.x + BALL_RADIUS > rightPaddle.position.x - (PADDLE_WIDTH / 2) && ball.position.y + BALL_RADIUS >= rightPaddle.position.y - (PADDLE_LENGTH / 2) && ball.position.y - BALL_RADIUS <= rightPaddle.position.y + (PADDLE_LENGTH / 2))
    {
        ball.speed.x = -ball.speed.x;
        ball.position.x = WINDOW_WIDTH - BALL_RADIUS - PADDLE_WIDTH / 2;
    }
    ball.ball.setPosition(ball.position);
}

void paddlePhysics(Paddle& leftPaddle, Paddle& rightPaddle, float deltaTime)
{
    leftPaddle.position = leftPaddle.paddle.getPosition();
    rightPaddle.position = rightPaddle.paddle.getPosition();
    leftPaddle.speedY = 0;
    rightPaddle.speedY = 0;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        leftPaddle.speedY = -PADDLE_SPEED;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        leftPaddle.speedY = PADDLE_SPEED;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        rightPaddle.speedY = -PADDLE_SPEED;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        rightPaddle.speedY = PADDLE_SPEED;
    }
    if (leftPaddle.position.y < PADDLE_LENGTH / 2)
    {
        leftPaddle.speedY = PADDLE_SPEED;
    }
    if (leftPaddle.position.y > WINDOW_HEIGHT - PADDLE_LENGTH / 2)
    {
        leftPaddle.speedY = -PADDLE_SPEED;
    }
    if (rightPaddle.position.y < PADDLE_LENGTH / 2)
    {
        rightPaddle.speedY = PADDLE_SPEED;
    }
    if (rightPaddle.position.y > WINDOW_HEIGHT - PADDLE_LENGTH / 2)
    {
        rightPaddle.speedY = -PADDLE_SPEED;
    }
    leftPaddle.position.y += leftPaddle.speedY * deltaTime;
    rightPaddle.position.y += rightPaddle.speedY * deltaTime;
    leftPaddle.paddle.setPosition(leftPaddle.position);
    rightPaddle.paddle.setPosition(rightPaddle.position);
}

void pollEvents(sf::RenderWindow &window)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if ((event.type == sf::Event::Closed) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)))
        {
            window.close();
        }
    }
}

void redrawFrame(sf::RenderWindow& window, Ball& ball, Paddle& leftPaddle, Paddle& rightPaddle, Texts& texts, GameCondition& condition)
{
    window.clear(sf::Color::Black);
    if (condition.isInGame)
    {
    window.draw(ball.ball);
    window.draw(leftPaddle.paddle);
    window.draw(rightPaddle.paddle);
    }
    if (condition.isInMenu)
    {
        window.draw(texts.menuText);
    }
    if (condition.isLeftWon)
    {
        window.draw(texts.leftVictoryText);
    }
    if (condition.isRightWon)
    {
        window.draw(texts.rightVictoryText);
    }
    window.display();
}

void switchConditions(GameCondition& condition)
{
    if (condition.isInMenu)
    {
        condition.isInGame = false;
        condition.isLeftWon = false;
        condition.isRightWon = false;
    }
    if (condition.isInGame)
    {
        condition.isInMenu = false;
        condition.isLeftWon = false;
        condition.isRightWon = false;
    }
    if (condition.isLeftWon)
    {
        condition.isInGame = false;
        condition.isInMenu = false;
        condition.isRightWon = false;
    }
    if (condition.isRightWon)
    {
        condition.isInGame = false;
        condition.isLeftWon = false;
        condition.isInMenu = false;
    }
    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) && (!condition.isLeftWon) && (!condition.isRightWon))
    {
        condition.isInGame = true;
        condition.isInMenu = false;
        condition.isLeftWon = false;
        condition.isRightWon = false;
    }
}

int main()
{
    Ball ball;
    Paddle leftPaddle;
    Paddle rightPaddle;
    sf::Clock clock;
    PRNG generator;
    Texts texts;
    GameCondition condition;

    condition.isInMenu = true;
    condition.isInGame = false;
    condition.isLeftWon = false;
    condition.isRightWon = false;

    initBall(ball, generator);
    initPaddles(leftPaddle, rightPaddle);
    initGenerator(generator);
    initTexts(texts);

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Pong");
    while(window.isOpen())
    {
        pollEvents(window);
        switchConditions(condition);
        float deltaTime = clock.restart().asSeconds();
        if (condition.isInGame)
        {
            ballPhysics(ball, leftPaddle, rightPaddle, deltaTime, condition);
            paddlePhysics(leftPaddle, rightPaddle, deltaTime);
        }
        redrawFrame(window, ball, rightPaddle, leftPaddle, texts, condition);
    }
}