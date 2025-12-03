#include "Bird.hpp"
#include <cmath>

// =====================
// Bird base class
// =====================
Bird :: Bird(sf::Texture & texture, sf::Vector2f deckPos)
{
    sprite.setTexture(texture);
    deckPosition = deckPos;
    sprite.setPosition(deckPosition);
    lifted = false;
    launched = false;
}

void Bird :: draw(sf::RenderWindow & window)
{
    window.draw(sprite);
}

sf::Sprite & Bird :: getSprite()
{
    return sprite;
}

const sf::Sprite & Bird :: getSprite() const
{
    return sprite;
}

void Bird :: setDeckPosition(const sf::Vector2f & pos)
{
    deckPosition = pos;
    if (launched == false)
    {
        sprite.setPosition(deckPosition);
    }
}

void Bird :: lift()
{
    if (launched == false)
    {
        sprite.move(0.0, -20.0);
        lifted = true;
    }
}

bool Bird :: isLaunched() const
{
    return launched;
}

void Bird :: launch()
{
    launched = true;
}

void Bird :: update(float dt, sf::Vector2f & velocity)
{
    sprite.move(velocity.x * dt, velocity.y * dt);
}

void Bird :: specialAbility(sf::Vector2f & velocity)
{
    // base bird has no special ability
}

// =====================
// RedBird
// =====================
RedBird :: RedBird(sf::Texture & tex, sf::Vector2f pos)
    : Bird(tex, pos)
{
}

// =====================
// BlueBird
// =====================
BlueBird :: BlueBird(sf::Texture & tex, sf::Vector2f pos)
    : Bird(tex, pos)
{
    splitUsed = false;
    splitTime = 0.0;
    leftDir.x = 0.0;
    leftDir.y = 0.0;
    rightDir.x = 0.0;
    rightDir.y = 0.0;
    leftSpeed = 0.0;
    rightSpeed = 0.0;
}

void BlueBird :: draw(sf::RenderWindow & window)
{
    if (splitUsed == false)
    {
        Bird :: draw(window);
    }
    else
    {
        sf::Sprite center = sprite;
        sf::Sprite left = sprite;
        sf::Sprite right = sprite;

        sf::Vector2f posCenter = sprite.getPosition();

        float baseScaleX = sprite.getScale().x;
        float baseScaleY = sprite.getScale().y;

        float smallScaleX = baseScaleX * 0.7;
        float smallScaleY = baseScaleY * 0.7;

        center.setScale(smallScaleX, smallScaleY);
        left.setScale(smallScaleX, smallScaleY);
        right.setScale(smallScaleX, smallScaleY);

        center.setPosition(posCenter.x, posCenter.y);

        sf::Vector2f leftPos;
        sf::Vector2f rightPos;

        leftPos.x = posCenter.x + leftDir.x * leftSpeed * splitTime;
        leftPos.y = posCenter.y + leftDir.y * leftSpeed * splitTime;

        rightPos.x = posCenter.x + rightDir.x * rightSpeed * splitTime;
        rightPos.y = posCenter.y + rightDir.y * rightSpeed * splitTime;

        left.setPosition(leftPos.x, leftPos.y);
        right.setPosition(rightPos.x, rightPos.y);

        window.draw(left);
        window.draw(center);
        window.draw(right);
    }
}

void BlueBird :: update(float dt, sf::Vector2f & velocity)
{
    Bird :: update(dt, velocity);

    if (splitUsed == true)
    {
        splitTime = splitTime + dt;
    }
}

void BlueBird :: specialAbility(sf::Vector2f & velocity)
{
    if (launched == false)
    {
        return;
    }
    if (splitUsed == true)
    {
        return;
    }

    splitUsed = true;
    splitTime = 0.0;

    float len = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);

    sf::Vector2f perp;
    if (len > 0.0)
    {
        perp.x = -velocity.y / len;
        perp.y = velocity.x / len;
    }
    else
    {
        perp.x = 0.0;
        perp.y = -1.0;
    }

    leftDir = perp;
    rightDir.x = -perp.x;
    rightDir.y = -perp.y;

    leftSpeed = len * 0.40;
    rightSpeed = len * 0.25;

    velocity.x = velocity.x * 0.7;
    velocity.y = velocity.y * 0.7;
}

// =====================
// YellowBird
// =====================
YellowBird :: YellowBird(sf::Texture & tex, sf::Vector2f pos)
    : Bird(tex, pos)
{
    boostUsed = false;
}

void YellowBird :: specialAbility(sf::Vector2f & velocity)
{
    if (launched == false)
    {
        return;
    }
    if (boostUsed == true)
    {
        return;
    }

    boostUsed = true;

    velocity.x = velocity.x * 1.6;
    velocity.y = velocity.y * 1.6;
}

// =========================
// factory
// =========================
Bird * createBird(int type,
                  sf::Texture & redTex,
                  sf::Texture & blueTex,
                  sf::Texture & yellowTex,
                  float x, float y, float scale)
{
    Bird * ptr = 0;

    if (type == 1)
    {
        ptr = new RedBird(redTex, sf::Vector2f(x, y));
    }
    else if (type == 2)
    {
        ptr = new BlueBird(blueTex, sf::Vector2f(x, y));
    }
    else if (type == 3)
    {
        ptr = new YellowBird(yellowTex, sf::Vector2f(x, y));
    }

    if (ptr != 0)
    {
        ptr->getSprite().setScale(scale, scale);
    }

    return ptr;
}
