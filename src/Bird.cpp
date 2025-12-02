#include "Bird.hpp"
#include <cmath>

// =====================
// Bird class
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
    // simple center bird motion
    sprite.move(velocity.x * dt, velocity.y * dt);
}

void Bird :: specialAbility(sf::Vector2f & velocity)
{
    // base bird has no special ability
}

// =====================
// RedBird class
// =====================
RedBird :: RedBird(sf::Texture & tex, sf::Vector2f pos)
    : Bird(tex, pos)
{
}

// =====================
// BlueBird class
// =====================
BlueBird :: BlueBird(sf::Texture & tex, sf::Vector2f pos)
    : Bird(tex, pos)
{
    splitUsed = false;
    splitTime = 0.0f;
    leftDir.x = 0.0f;
    leftDir.y = 0.0f;
    rightDir.x = 0.0f;
    rightDir.y = 0.0f;
    leftSpeed = 0.0f;
    rightSpeed = 0.0f;
}

void BlueBird :: draw(sf::RenderWindow & window)
{
    if (splitUsed == false)
    {
        // just one normal bird
        Bird :: draw(window);
    }
    else
    {
        // three birds following slightly different projectiles
        sf::Sprite center = sprite;
        sf::Sprite left = sprite;
        sf::Sprite right = sprite;

        sf::Vector2f posCenter = sprite.getPosition();

        float baseScaleX = sprite.getScale().x;
        float baseScaleY = sprite.getScale().y;

        float smallScaleX = baseScaleX * 0.7f;
        float smallScaleY = baseScaleY * 0.7f;

        center.setScale(smallScaleX, smallScaleY);
        left.setScale(smallScaleX, smallScaleY);
        right.setScale(smallScaleX, smallScaleY);

        // middle follows original path
        center.setPosition(posCenter.x, posCenter.y);

        // left and right spread over time using side directions
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
    // center bird moves same as normal
    Bird :: update(dt, velocity);

    if (splitUsed == true)
    {
        // track how long it has been split to spread further over time
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
    splitTime = 0.0f;

    // base velocity length
    float len = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);

    // perpendicular direction to create spread
    sf::Vector2f perp;
    if (len > 0.0f)
    {
        perp.x = -velocity.y / len;
        perp.y = velocity.x / len;
    }
    else
    {
        // default sideways if somehow zero velocity
        perp.x = 0.0f;
        perp.y = -1.0f;
    }

    leftDir = perp;
    rightDir.x = -perp.x;
    rightDir.y = -perp.y;

    // left bird has slightly more side speed, right bird a bit less
    leftSpeed = len * 0.40f;
    rightSpeed = len * 0.25f;

    // optional: slightly reduce main velocity so total "damage" feels fair
    velocity.x = velocity.x * 0.7f;
    velocity.y = velocity.y * 0.7f;
}

// =====================
// YellowBird class
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

    // speed boost
    velocity.x = velocity.x * 1.6f;
    velocity.y = velocity.y * 1.6f;
}

// =========================
// function to create birds
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
