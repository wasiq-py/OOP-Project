#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include <memory>
using namespace std;

// =====================
// Bird class
// =====================
class Bird {
public:
    Bird(sf::Texture& texture, sf::Vector2f deckPos) : deckPosition(deckPos) {
        sprite.setTexture(texture);
        sprite.setPosition(deckPosition);
    }

    virtual void draw(sf::RenderWindow& window) { window.draw(sprite); }

    virtual void lift() {
        if (!launched) {
            lifted = true;
            sprite.setPosition(deckPosition.x, deckPosition.y - 50);
        }
    }

    virtual void reset() {
        lifted = false;
        launched = false;
        velocity = {0, 0};
        sprite.setPosition(deckPosition);
    }

    virtual void launch(sf::Vector2f vel) {
        if (!launched) {
            launched = true;
            lifted = false;
            velocity = vel;
        }
    }

    virtual void update(float dt) {
        if (launched) {
            velocity.y += GRAVITY * dt;
            sprite.move(velocity * dt);
        }
    }

    sf::Sprite& getSprite() { return sprite; }
    bool isLaunched() const { return launched; }
    void setDeckPosition(sf::Vector2f pos) { deckPosition = pos; sprite.setPosition(deckPosition); }

protected:
    sf::Sprite sprite;
    sf::Vector2f deckPosition;
    sf::Vector2f velocity;
    bool lifted = false;
    bool launched = false;
    const float GRAVITY = 500.0f;
};

// Derived birds
class RedBird : public Bird { public: RedBird(sf::Texture& tex, sf::Vector2f pos) : Bird(tex, pos) {} };
class BlueBird : public Bird { public: BlueBird(sf::Texture& tex, sf::Vector2f pos) : Bird(tex, pos) {} };
class YellowBird : public Bird { public: YellowBird(sf::Texture& tex, sf::Vector2f pos) : Bird(tex, pos) {} };

// =========================
// NEW: function to create birds
// =========================
unique_ptr<Bird> createBird(int type,
    sf::Texture& redTex,
    sf::Texture& blueTex,
    sf::Texture& yellowTex,
    float x, float y, float scale)
{
    switch(type) {
        case 1: return make_unique<RedBird>(redTex, sf::Vector2f(x,y));
        case 2: return make_unique<BlueBird>(blueTex, sf::Vector2f(x,y));
        case 3: return make_unique<YellowBird>(yellowTex, sf::Vector2f(x,y));
    }
    return nullptr;
}

// =====================
// main 
// =====================
int main() {
    const int WINDOW_WIDTH  = 1920;
    const int WINDOW_HEIGHT = 1080;
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Scrolling Background with Ship");
    window.setFramerateLimit(60);

    // =====================================================
    // LOAD TEXTURES
    // =====================================================
    sf::Texture bgTexture, shipTexture, wavesTexture;
    sf::Texture redBirdTexture, blueBirdTexture, yellowBirdTexture;

    if (!bgTexture.loadFromFile("OOP-Project\\bg.png") ||
        !shipTexture.loadFromFile("OOP-Project\\ship.png") ||
        !wavesTexture.loadFromFile("OOP-Project\\1waves.png") ||
        !redBirdTexture.loadFromFile("OOP-Project\\bird_red.png") ||
        !blueBirdTexture.loadFromFile("OOP-Project\\bird_blue.png") ||
        !yellowBirdTexture.loadFromFile("OOP-Project\\bird_yellow.png")) 
    {
        cout << "Error loading textures" << endl;
        return 1;
    }

    bgTexture.setSmooth(true);
    shipTexture.setSmooth(true);
    wavesTexture.setSmooth(true);
    redBirdTexture.setSmooth(true);
    blueBirdTexture.setSmooth(true);
    yellowBirdTexture.setSmooth(true);

    // =====================================================
    // BACKGROUND MUSIC
    // =====================================================
    sf::Music music;
    if (music.openFromFile("Angry Birds Theme Song.mp3")) {
        music.setLoop(true);
        music.play();
    } else cout << "Warning: could not load music" << endl;

    // =====================================================
    // BACKGROUND SETUP
    // =====================================================
    sf::Sprite bgSprite(bgTexture);
    sf::IntRect bgRect(0,0,WINDOW_WIDTH,WINDOW_HEIGHT);
    bgSprite.setTextureRect(bgRect);
    const float BG_SPEED = 200.0f;
    const int BG_MAX_OFFSET = static_cast<int>(bgTexture.getSize().x) - WINDOW_WIDTH;
    float bgOffsetX = 0.0f;

    // =====================================================
    // SHIP SETUP
    // =====================================================
    sf::Sprite shipSprite(shipTexture);
    float desiredShipHeight = WINDOW_HEIGHT * 0.3f;
    float shipScale = desiredShipHeight / static_cast<float>(shipTexture.getSize().y);
    shipSprite.setScale(shipScale, shipScale);
    float shipX = WINDOW_WIDTH * 0.08f;
    float shipY = WINDOW_HEIGHT * 0.65f;
    shipSprite.setPosition(shipX, shipY);
    sf::FloatRect shipBounds = shipSprite.getGlobalBounds();

    float birdDeckX = shipBounds.left + shipBounds.width * 0.45f;
    float birdDeckY = shipBounds.top + shipBounds.height * 0.25f;

    // =====================================================
    // WAVES SETUP
    // =====================================================
    sf::Sprite waves1(wavesTexture), waves2(wavesTexture);
    float wavesWidth  = static_cast<float>(wavesTexture.getSize().x);
    float wavesHeight = static_cast<float>(wavesTexture.getSize().y);
    float wavesScaleX = 2.0f, wavesScaleY = 1.5f;
    waves1.setScale(wavesScaleX,wavesScaleY);
    waves2.setScale(wavesScaleX,wavesScaleY);
    float wavesWidthScaled  = wavesWidth  * wavesScaleX;
    float wavesHeightScaled = wavesHeight * wavesScaleY;
    float wavesY = WINDOW_HEIGHT - wavesHeightScaled;
    waves1.setPosition(0.0f, wavesY);
    waves2.setPosition(wavesWidthScaled, wavesY);
    const float WAVES_SPEED = 300.0f;

    // =====================================================
    // NEW BIRD SYSTEM
    // =====================================================
    float desiredBirdHeight = shipBounds.height * 0.25f;
    float birdScale = desiredBirdHeight / redBirdTexture.getSize().y;

    unique_ptr<Bird> currentBird = nullptr;

    // =====================================================
    // TIME & STATE
    // =====================================================
    sf::Clock clock;
    bool scrolling = true;
    bool dragging = false;
    sf::Vector2f dragStartPos;
    sf::Vector2f birdVelocity(0,0);
    bool birdLaunched = false;
    const float GRAVITY = 500.0f;

    while (window.isOpen()) {
        // -------------------------------------------------
        // EVENT LOOP
        // -------------------------------------------------
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            else if (event.type == sf::Event::KeyPressed) {

                if (event.key.code == sf::Keyboard::Space) {
                    scrolling = !scrolling;
                    if (scrolling) music.play(); else music.pause();
                }

                // --- NEW BIRD CREATION ---
                else if (event.key.code == sf::Keyboard::Num1 ||
                         event.key.code == sf::Keyboard::Num2 ||
                         event.key.code == sf::Keyboard::Num3) 
                {
                    int type = (event.key.code == sf::Keyboard::Num1) ? 1 :
                               (event.key.code == sf::Keyboard::Num2) ? 2 : 3;

                    currentBird.reset();
                    birdLaunched = false;

                    currentBird = createBird(type,
                        redBirdTexture,
                        blueBirdTexture,
                        yellowBirdTexture,
                        birdDeckX, birdDeckY, birdScale);

                    currentBird->getSprite().setScale(birdScale, birdScale);
                }

                else if (event.key.code == sf::Keyboard::Up &&
                         currentBird && !currentBird->isLaunched())
                {
                    currentBird->lift();
                }
            }

            // Mouse drag start
            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left) {

                sf::Vector2f mousePos(event.mouseButton.x,event.mouseButton.y);

                if (currentBird &&
                    currentBird->getSprite().getGlobalBounds().contains(mousePos)) {

                    dragging = true;
                    dragStartPos = mousePos;
                }
            }

            // Mouse release
            if (event.type == sf::Event::MouseButtonReleased &&
                event.mouseButton.button == sf::Mouse::Left && dragging) {

                dragging = false;
                birdLaunched = true;

                sf::Vector2f releasePos(event.mouseButton.x,event.mouseButton.y);
                sf::Vector2f dragVector = dragStartPos - releasePos;
                dragVector.y -= 100.0f;

                float velocityScale = 5.0f;
                birdVelocity = dragVector * velocityScale;

                float speed = sqrt(birdVelocity.x*birdVelocity.x +
                                   birdVelocity.y*birdVelocity.y);

                float maxSpeed = 2000.0f;
                if (speed > maxSpeed)
                    birdVelocity = birdVelocity / speed * maxSpeed;
            }
        }

        // -------------------------------------------------
        // UPDATE
        // -------------------------------------------------
        float dt = clock.restart().asSeconds();

        // Background
        if (scrolling) {
            bgOffsetX += BG_SPEED * dt;
            if (bgOffsetX > BG_MAX_OFFSET) bgOffsetX -= BG_MAX_OFFSET;
            bgRect.left = static_cast<int>(bgOffsetX);
            bgSprite.setTextureRect(bgRect);

            waves1.move(-WAVES_SPEED * dt, 0);
            waves2.move(-WAVES_SPEED * dt, 0);

            if (waves1.getPosition().x <= -wavesWidthScaled)
                waves1.setPosition(waves2.getPosition().x + wavesWidthScaled, wavesY);

            if (waves2.getPosition().x <= -wavesWidthScaled)
                waves2.setPosition(waves1.getPosition().x + wavesWidthScaled, wavesY);
        }

        // --- NEW BIRD UPDATE ---
        if (currentBird) {

            if (dragging) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                currentBird->getSprite().setPosition(
                    static_cast<float>(mousePos.x),
                    static_cast<float>(mousePos.y)
                );
            }

            else if (birdLaunched) {
                birdVelocity.y += GRAVITY * dt;
                currentBird->getSprite().move(birdVelocity * dt);

                // auto delete if out of screen
                auto pos = currentBird->getSprite().getPosition();
                if (pos.y > WINDOW_HEIGHT + 300 ||
                    pos.x > WINDOW_WIDTH + 300 ||
                    pos.x < -300)
                {
                    currentBird.reset();
                    birdLaunched = false;
                }
            }

            else {
                // idle → sit on deck
                currentBird->setDeckPosition(sf::Vector2f(birdDeckX, birdDeckY));
            }
        }

        // -------------------------------------------------
        // DRAW
        // -------------------------------------------------
        window.clear(sf::Color::Black);
        window.draw(bgSprite);
        window.draw(shipSprite);
        window.draw(waves1);
        window.draw(waves2);

        if (currentBird)
            currentBird->draw(window);

        window.display();
    }

    return 0;
}
