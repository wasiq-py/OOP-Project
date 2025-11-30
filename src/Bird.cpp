#include "Bird.hpp"

// =====================
// Bird class
// =====================
Bird::Bird(sf::Texture& texture, sf::Vector2f deckPos) : deckPosition(deckPos)
{
    sprite.setTexture(texture);
    sprite.setPosition(deckPosition);
    lifted = false;
    launched = false;
}

void Bird::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}

void Bird::lift()
{
    if (!launched) {
        lifted = true;
        sprite.setPosition(deckPosition.x, deckPosition.y - 50.0f);
    }
}

void Bird::reset()
{
    lifted = false;
    launched = false;
    sprite.setPosition(deckPosition);
}

void Bird::launch()
{
    if (!launched) {
        launched = true;
        lifted = false;
    }
}

sf::Sprite& Bird::getSprite()
{
    return sprite;
}

bool Bird::isLaunched() const
{
    return launched;
}

void Bird::setDeckPosition(sf::Vector2f pos)
{
    deckPosition = pos;
    sprite.setPosition(deckPosition);
}

// Derived birds
RedBird::RedBird(sf::Texture& tex, sf::Vector2f pos)
    : Bird(tex, pos)
{
}

BlueBird::BlueBird(sf::Texture& tex, sf::Vector2f pos)
    : Bird(tex, pos)
{
}

YellowBird::YellowBird(sf::Texture& tex, sf::Vector2f pos)
    : Bird(tex, pos)
{
}

// =========================
// function to create birds
// =========================
unique_ptr<Bird> createBird(int type,
    sf::Texture& redTex,
    sf::Texture& blueTex,
    sf::Texture& yellowTex,
    float x, float y, float scale)
{
    if (type == 1) {
        unique_ptr<Bird> ptr(new RedBird(redTex, sf::Vector2f(x, y)));
        ptr->getSprite().setScale(scale, scale);
        return ptr;
    }
    else if (type == 2) {
        unique_ptr<Bird> ptr(new BlueBird(blueTex, sf::Vector2f(x, y)));
        ptr->getSprite().setScale(scale, scale);
        return ptr;
    }
    else if (type == 3) {
        unique_ptr<Bird> ptr(new YellowBird(yellowTex, sf::Vector2f(x, y)));
        ptr->getSprite().setScale(scale, scale);
        return ptr;
    }

    unique_ptr<Bird> empty;
    return empty;
}
