#include "Dev1.hpp"

// ======================================================
//                 PIG BASE CLASS
// ======================================================

Pig::Pig(const sf::Texture& texture, sf::Vector2f position, int hp, string t)
{
    sprite.setTexture(texture);
    sprite.setPosition(position);

    maxHP = hp;
    currentHP = hp;
    type = t;
    alive = true;
}

void Pig::update(float dt)
{
    if (alive == false)
    {
        sprite.setColor(sf::Color(255, 255, 255, 0));
    }
}

sf::Sprite& Pig::getSprite()
{
    return sprite;
}

const sf::Sprite& Pig::getSprite() const
{
    return sprite;
}

bool Pig::isAlive() const
{
    return alive;
}

void Pig::takeDamage(int amount)
{
    if (alive == false)
    {
        return;
    }

    currentHP = currentHP - amount;

    if (currentHP <= 0)
    {
        currentHP = 0;
        alive = false;
        sprite.setColor(sf::Color(255, 255, 255, 0));
    }
}

int Pig::getHP() const
{
    return currentHP;
}

string Pig::getType() const
{
    return type;
}

// weak pig ~ 1 good hit
WeakPig::WeakPig(const sf::Texture& texture, sf::Vector2f position)
    : Pig(texture, position, 4, "weak")
{
}

// strong pig ~ 2–3 good hits
StrongPig::StrongPig(const sf::Texture& texture, sf::Vector2f position)
    : Pig(texture, position, 10, "strong")
{
}

// king pig ~ 5–6 good hits
KingPig::KingPig(const sf::Texture& texture, sf::Vector2f position)
    : Pig(texture, position, 22, "king")
{
}

// ======================================================
//                 OBSTACLE BASE CLASS
// ======================================================

Obstacle::Obstacle(const sf::Texture& texture,
                   sf::Vector2f position,
                   string m)
{
    sprite.setTexture(texture);
    sprite.setPosition(position);

    destroyed = false;
    material = m;
}

void Obstacle::update(float dt)
{
    if (destroyed == true)
    {
        sprite.setColor(sf::Color(255, 255, 255, 0));
    }
}

sf::Sprite& Obstacle::getSprite()
{
    return sprite;
}

const sf::Sprite& Obstacle::getSprite() const
{
    return sprite;
}

bool Obstacle::isDestroyed() const
{
    return destroyed;
}

string Obstacle::getMaterial() const
{
    return material;
}

// ======================================================
//          OBSTACLE DERIVED CLASSES: onHit
// ======================================================

static void slowBirdVelocity(sf::Vector2f& v, float factor)
{
    v.x = v.x * factor;
    v.y = v.y * factor;
}

IceObstacle::IceObstacle(const sf::Texture& texture, sf::Vector2f position)
    : Obstacle(texture, position, "ice")
{
}

void IceObstacle::onHit(sf::Vector2f& birdVelocity)
{
    if (destroyed == true)
    {
        return;
    }

    slowBirdVelocity(birdVelocity, 0.85);

    destroyed = true;
    sprite.setColor(sf::Color(255, 255, 255, 0));
}

WoodObstacle::WoodObstacle(const sf::Texture& texture, sf::Vector2f position)
    : Obstacle(texture, position, "wood")
{
}

void WoodObstacle::onHit(sf::Vector2f& birdVelocity)
{
    if (destroyed == true)
    {
        return;
    }

    slowBirdVelocity(birdVelocity, 0.70);

    destroyed = true;
    sprite.setColor(sf::Color(255, 255, 255, 0));
}

StoneObstacle::StoneObstacle(const sf::Texture& texture, sf::Vector2f position)
    : Obstacle(texture, position, "metal")
{
}

void StoneObstacle::onHit(sf::Vector2f& birdVelocity)
{
    if (destroyed == true)
    {
        return;
    }

    slowBirdVelocity(birdVelocity, 0.45);

    destroyed = true;
    sprite.setColor(sf::Color(255, 255, 255, 0));
}

// ======================================================
//                 COLLISION HELPERS
// ======================================================

static bool checkCollision(const sf::Sprite& a, const sf::Sprite& b)
{
    sf::FloatRect A = a.getGlobalBounds();
    sf::FloatRect B = b.getGlobalBounds();
    return A.intersects(B);
}

bool handleBirdObstacleCollision(const sf::Sprite& bird,
                                 sf::Vector2f& birdVelocity,
                                 Obstacle& obs)
{
    if (obs.isDestroyed() == true)
    {
        return false;
    }

    if (checkCollision(bird, obs.getSprite()))
    {
        obs.onHit(birdVelocity);
        return true;
    }

    return false;
}

int computeDamageFromVelocity(const sf::Vector2f& velocity)
{
    float speed = std::sqrt(velocity.x * velocity.x +
                            velocity.y * velocity.y);

    int damage = (int)(speed / 300.0);

    if (damage < 1)
    {
        damage = 1;
    }
    if (damage > 10)
    {
        damage = 10;
    }

    return damage;
}

bool handleBirdPigCollision(const sf::Sprite& bird,
                            const sf::Vector2f& birdVelocity,
                            Pig& pig)
{
    if (pig.isAlive() == false)
    {
        return false;
    }

    if (checkCollision(bird, pig.getSprite()))
    {
        int damage = computeDamageFromVelocity(birdVelocity);
        pig.takeDamage(damage);
        return true;
    }

    return false;
}
