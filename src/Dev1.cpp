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

Pig::~Pig()
{
}

void Pig::update(float dt)
{
    // For now pigs do not move.
    // If you want movement later, add here.
}

void Pig::draw(sf::RenderWindow& window)
{
    if (alive)
        window.draw(sprite);
}

void Pig::takeDamage(int amount)
{
    if (!alive) return;

    currentHP = currentHP - amount;

    if (currentHP <= 0)
    {
        currentHP = 0;
        alive = false;
    }
}

bool Pig::isAlive() const { return alive; }
int Pig::getHP() const { return currentHP; }
int Pig::getMaxHP() const { return maxHP; }

sf::Sprite& Pig::getSprite() { return sprite; }
sf::FloatRect Pig::getBounds() const { return sprite.getGlobalBounds(); }


// ======================================================
//          WEAK + STRONG PIGS (DERIVED)
// ======================================================

WeakPig::WeakPig(const sf::Texture& texture, sf::Vector2f pos)
    : Pig(texture, pos, 8, "weak")   // example HP: 8
{
}

StrongPig::StrongPig(const sf::Texture& texture, sf::Vector2f pos)
    : Pig(texture, pos, 15, "strong") // example HP: 15
{
}


// ======================================================
//                OBSTACLE BASE CLASS
// ======================================================

Obstacle::Obstacle(const sf::Texture& texture, sf::Vector2f pos, string t)
{
    sprite.setTexture(texture);
    sprite.setPosition(pos);

    destroyed = false;
    type = t;
}

Obstacle::~Obstacle()
{
}

void Obstacle::update(float dt)
{
    // Static for now. Add physics later if needed.
}

void Obstacle::draw(sf::RenderWindow& window)
{
    if (!destroyed)
        window.draw(sprite);
}

bool Obstacle::isDestroyed() const { return destroyed; }
sf::Sprite& Obstacle::getSprite() { return sprite; }
sf::FloatRect Obstacle::getBounds() const { return sprite.getGlobalBounds(); }
string Obstacle::getType() const { return type; }


// ======================================================
//              OBSTACLE TYPES
// ======================================================

IceObstacle::IceObstacle(const sf::Texture& tex, sf::Vector2f pos)
    : Obstacle(tex, pos, "ice")
{
}

void IceObstacle::onHit(sf::Vector2f& velocity)
{
    // small slow: keep 80% of speed
    velocity.x = velocity.x * 0.8f;
    velocity.y = velocity.y * 0.8f;
    destroyed = true;   // breaks in one hit
}

WoodObstacle::WoodObstacle(const sf::Texture& tex, sf::Vector2f pos)
    : Obstacle(tex, pos, "wood")
{
}

void WoodObstacle::onHit(sf::Vector2f& velocity)
{
    // medium slow: keep 50% of speed
    velocity.x = velocity.x * 0.5f;
    velocity.y = velocity.y * 0.5f;
    destroyed = true;   // breaks in one hit
}

StoneObstacle::StoneObstacle(const sf::Texture& tex, sf::Vector2f pos)
    : Obstacle(tex, pos, "stone")
{
}

void StoneObstacle::onHit(sf::Vector2f& velocity)
{
    // strong slow: keep 20% of speed
    velocity.x = velocity.x * 0.2f;
    velocity.y = velocity.y * 0.2f;
    destroyed = true;   // breaks in one hit
}


// ======================================================
//                COLLISION HELPERS
// ======================================================

bool checkCollision(const sf::Sprite& a, const sf::Sprite& b)
{
    sf::FloatRect A = a.getGlobalBounds();
    sf::FloatRect B = b.getGlobalBounds();
    return A.intersects(B);
}

// Bird ↔ obstacle
bool handleBirdObstacleCollision(const sf::Sprite& bird,
                                 sf::Vector2f& birdVelocity,
                                 Obstacle& obs)
{
    if (obs.isDestroyed())
        return false;

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

    // scale factor: tune this
    int damage = (int)(speed / 200.0f);

    if (damage < 1)
        damage = 1;

    return damage;
}

// Bird ↔ pig
bool handleBirdPigCollision(const sf::Sprite& bird,
                            const sf::Vector2f& birdVelocity,
                            Pig& pig)
{
    if (!pig.isAlive())
        return false;

    if (checkCollision(bird, pig.getSprite()))
    {
        int damage = computeDamageFromVelocity(birdVelocity);
        pig.takeDamage(damage);
        return true;
    }

    return false;
}
