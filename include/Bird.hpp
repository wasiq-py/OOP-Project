#ifndef BIRD_HPP
#define BIRD_HPP

#include <SFML/Graphics.hpp>
#include <memory>
using namespace std;

// =====================
// Bird class
// =====================
class Bird {
public:
    Bird(sf::Texture& texture, sf::Vector2f deckPos);

    virtual void draw(sf::RenderWindow& window);

    virtual void lift();

    virtual void reset();

    virtual void launch();

    sf::Sprite& getSprite();
    bool isLaunched() const;
    void setDeckPosition(sf::Vector2f pos);

protected:
    sf::Sprite sprite;
    sf::Vector2f deckPosition;
    bool lifted;
    bool launched;
};

// Derived birds
class RedBird : public Bird {
public:
    RedBird(sf::Texture& tex, sf::Vector2f pos);
};

class BlueBird : public Bird {
public:
    BlueBird(sf::Texture& tex, sf::Vector2f pos);
};

class YellowBird : public Bird {
public:
    YellowBird(sf::Texture& tex, sf::Vector2f pos);
};

// =========================
// function to create birds
// =========================
unique_ptr<Bird> createBird(int type,
    sf::Texture& redTex,
    sf::Texture& blueTex,
    sf::Texture& yellowTex,
    float x, float y, float scale);

#endif
