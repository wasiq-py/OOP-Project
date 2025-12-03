#ifndef DEV1_HPP
#define DEV1_HPP

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <string>
using namespace std;

// pig base class -------------------------------------------

class Pig
{
protected:
    sf::Sprite sprite;
    int maxHP;
    int currentHP;
    bool alive;
    string type;

public:
    Pig(const sf::Texture& texture, sf::Vector2f position, int hp, string t);

    virtual void update(float dt);

    sf::Sprite& getSprite();
    const sf::Sprite& getSprite() const;

    bool isAlive() const;

    void takeDamage(int amount);

    int getHP() const;
    string getType() const;
};

// weak pig 
class WeakPig : public Pig
{
public:
    WeakPig(const sf::Texture& texture, sf::Vector2f position);
};

// strong pig
class StrongPig : public Pig
{
public:
    StrongPig(const sf::Texture& texture, sf::Vector2f position);
};

// king pig 
class KingPig : public Pig
{
public:
    KingPig(const sf::Texture& texture, sf::Vector2f position);
};

// obstacle base class --------------------------------------------------

class Obstacle
{
protected:
    sf::Sprite sprite;
    bool destroyed;
    string material;   // ice, wood, metal

public:
    Obstacle(const sf::Texture& texture, sf::Vector2f position, string m);

    virtual void update(float dt);

    sf::Sprite& getSprite();
    const sf::Sprite& getSprite() const;

    bool isDestroyed() const;
    string getMaterial() const;

    virtual void onHit(sf::Vector2f& birdVelocity) = 0;
};

// ice obs -----------------------------------------

class IceObstacle : public Obstacle
{
public:
    IceObstacle(const sf::Texture& texture, sf::Vector2f position);

    virtual void onHit(sf::Vector2f& birdVelocity);
};

// wood obs -----------------------------------------
class WoodObstacle : public Obstacle
{
public:
    WoodObstacle(const sf::Texture& texture, sf::Vector2f position);

    virtual void onHit(sf::Vector2f& birdVelocity);
};

// metal obs -----------------------------------------
class StoneObstacle : public Obstacle
{
public:
    StoneObstacle(const sf::Texture& texture, sf::Vector2f position);

    virtual void onHit(sf::Vector2f& birdVelocity);
};

// colliion helper funcs

bool handleBirdObstacleCollision(const sf::Sprite& bird,
                                 sf::Vector2f& birdVelocity,
                                 Obstacle& obs);

int computeDamageFromVelocity(const sf::Vector2f& velocity);

bool handleBirdPigCollision(const sf::Sprite& bird,
                            const sf::Vector2f& birdVelocity,
                            Pig& pig);

#endif
