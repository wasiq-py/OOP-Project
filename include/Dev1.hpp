#ifndef DEV1_HPP
#define DEV1_HPP

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <string>
using namespace std;

// ======================================================
//                 PIG BASE CLASS
// ======================================================

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

// weak pig = low HP (1 good hit)
class WeakPig : public Pig
{
public:
    WeakPig(const sf::Texture& texture, sf::Vector2f position);
};

// strong pig = higher HP (2–3 good hits)
class StrongPig : public Pig
{
public:
    StrongPig(const sf::Texture& texture, sf::Vector2f position);
};

// king pig = high HP (5–6 good hits), used in last level
class KingPig : public Pig
{
public:
    KingPig(const sf::Texture& texture, sf::Vector2f position);
};

// ======================================================
//                 OBSTACLE BASE CLASS
// ======================================================

class Obstacle
{
protected:
    sf::Sprite sprite;
    bool destroyed;
    string material;   // "ice", "wood", "metal"

public:
    Obstacle(const sf::Texture& texture, sf::Vector2f position, string m);

    virtual void update(float dt);

    sf::Sprite& getSprite();
    const sf::Sprite& getSprite() const;

    bool isDestroyed() const;
    string getMaterial() const;

    virtual void onHit(sf::Vector2f& birdVelocity) = 0;
};

// ice obstacle: weakest
class IceObstacle : public Obstacle
{
public:
    IceObstacle(const sf::Texture& texture, sf::Vector2f position);

    virtual void onHit(sf::Vector2f& birdVelocity);
};

// wood obstacle: medium
class WoodObstacle : public Obstacle
{
public:
    WoodObstacle(const sf::Texture& texture, sf::Vector2f position);

    virtual void onHit(sf::Vector2f& birdVelocity);
};

// stone obstacle (metal): strongest
class StoneObstacle : public Obstacle
{
public:
    StoneObstacle(const sf::Texture& texture, sf::Vector2f position);

    virtual void onHit(sf::Vector2f& birdVelocity);
};

// ======================================================
//                 COLLISION HELPERS
// ======================================================

bool handleBirdObstacleCollision(const sf::Sprite& bird,
                                 sf::Vector2f& birdVelocity,
                                 Obstacle& obs);

int computeDamageFromVelocity(const sf::Vector2f& velocity);

bool handleBirdPigCollision(const sf::Sprite& bird,
                            const sf::Vector2f& birdVelocity,
                            Pig& pig);

#endif
