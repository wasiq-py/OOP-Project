#ifndef BIRD_HPP
#define BIRD_HPP

#include <SFML/Graphics.hpp>
using namespace std;

// =====================
// Bird base class
// =====================
class Bird
{
protected:
    sf::Sprite sprite;
    sf::Vector2f deckPosition;
    bool lifted;
    bool launched;

public:
    Bird(sf::Texture & texture, sf::Vector2f deckPos);

    virtual void draw(sf::RenderWindow & window);

    sf::Sprite & getSprite();
    const sf::Sprite & getSprite() const;

    void setDeckPosition(const sf::Vector2f & pos);
    void lift();
    bool isLaunched() const;
    void launch();

    // update motion given dt and velocity
    virtual void update(float dt, sf::Vector2f & velocity);

    // special ability while airborne (mouse click)
    virtual void specialAbility(sf::Vector2f & velocity);
};

// =====================
// RedBird class
// =====================
class RedBird : public Bird
{
public:
    RedBird(sf::Texture & tex, sf::Vector2f pos);
};

// =====================
// BlueBird class
//  - splits into three, with spread trajectories
// =====================
class BlueBird : public Bird
{
private:
    bool splitUsed;
    float splitTime;
    sf::Vector2f leftDir;
    sf::Vector2f rightDir;
    float leftSpeed;
    float rightSpeed;

public:
    BlueBird(sf::Texture & tex, sf::Vector2f pos);

    virtual void draw(sf::RenderWindow & window);
    virtual void update(float dt, sf::Vector2f & velocity);
    virtual void specialAbility(sf::Vector2f & velocity);
};

// =====================
// YellowBird class
//  - one-time speed boost
// =====================
class YellowBird : public Bird
{
private:
    bool boostUsed;

public:
    YellowBird(sf::Texture & tex, sf::Vector2f pos);

    virtual void specialAbility(sf::Vector2f & velocity);
};

// =========================
// function to create birds
// =========================
Bird * createBird(int type,
                  sf::Texture & redTex,
                  sf::Texture & blueTex,
                  sf::Texture & yellowTex,
                  float x, float y, float scale);

#endif
