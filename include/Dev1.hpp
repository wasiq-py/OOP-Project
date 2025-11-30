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
    string type;     // "weak" or "strong"
    bool alive;

public:
    Pig(const sf::Texture& texture, sf::Vector2f position, int hp, string t);

    virtual ~Pig();

    virtual void update(float dt);

    virtual void draw(sf::RenderWindow& window);

    virtual void takeDamage(int amount);

    bool isAlive() const;
    int getHP() const;
    int getMaxHP() const;

    sf::Sprite& getSprite();
    sf::FloatRect getBounds() const;
};


// ======================================================
//          WEAK + STRONG PIGS (DERIVED)
// ======================================================

class WeakPig : public Pig
{
public:
    WeakPig(const sf::Texture& texture, sf::Vector2f pos);
};

class StrongPig : public Pig
{
public:
    StrongPig(const sf::Texture& texture, sf::Vector2f pos);
};


// ======================================================
//                OBSTACLE BASE CLASS
// ======================================================
//
// Each obstacle:
//  - breaks (destroyed) in ONE hit
//  - slows down bird velocity depending on type
//
// Ice:   small slow
// Wood:  medium slow
// Stone: strong slow
//

class Obstacle
{
protected:
    sf::Sprite sprite;
    bool destroyed;
    string type;   // "ice", "wood", "stone"

public:
    Obstacle(const sf::Texture& texture, sf::Vector2f pos, string t);

    virtual ~Obstacle();

    virtual void update(float dt);

    virtual void draw(sf::RenderWindow& window);

    bool isDestroyed() const;
    sf::Sprite& getSprite();
    sf::FloatRect getBounds() const;
    string getType() const;

    // Called when bird hits this obstacle.
    // velocity is passed BY REFERENCE so it can be modified.
    virtual void onHit(sf::Vector2f& velocity) = 0;
};


// ======================================================
//              OBSTACLE TYPES
// ======================================================

class IceObstacle : public Obstacle
{
public:
    IceObstacle(const sf::Texture& tex, sf::Vector2f pos);

    virtual void onHit(sf::Vector2f& velocity);
};

class WoodObstacle : public Obstacle
{
public:
    WoodObstacle(const sf::Texture& tex, sf::Vector2f pos);

    virtual void onHit(sf::Vector2f& velocity);
};

class StoneObstacle : public Obstacle
{
public:
    StoneObstacle(const sf::Texture& tex, sf::Vector2f pos);

    virtual void onHit(sf::Vector2f& velocity);
};


// ======================================================
//                COLLISION HELPERS
// ======================================================

bool checkCollision(const sf::Sprite& a, const sf::Sprite& b);

// Bird ↔ obstacle
// Each hit:
//  - modifies bird velocity (slow)
//  - destroys the obstacle in ONE hit
bool handleBirdObstacleCollision(const sf::Sprite& bird,
                                 sf::Vector2f& birdVelocity,
                                 Obstacle& obs);

// Damage function based on bird velocity
int computeDamageFromVelocity(const sf::Vector2f& velocity);

// Bird ↔ pig
bool handleBirdPigCollision(const sf::Sprite& bird,
                            const sf::Vector2f& birdVelocity,
                            Pig& pig);

#endif
