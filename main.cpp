#include "Graphics.hpp"
#include "Bird.hpp"
#include "Dev1.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include <memory>
#include <string>

using namespace std;

// Folder paths
static const std::string TEXTURES = "assets/textures/";
static const std::string AUDIO    = "assets/audio/";

// =====================
// main
// =====================
int main() {

    sf::RenderWindow window(
        sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
        "Scrolling Background with Ship"
    );
    window.setFramerateLimit(60);

    // =====================================================
    // LOAD TEXTURES
    // =====================================================
    sf::Texture bgTexture, shipTexture, wavesTexture;
    sf::Texture redBirdTexture, blueBirdTexture, yellowBirdTexture;
    sf::Texture weakPigTexture, strongPigTexture;
    sf::Texture iceTexture, woodTexture, stoneTexture;

    auto load = [&](sf::Texture& t, const string& file) -> bool {
        if (!t.loadFromFile(TEXTURES + file)) {
            cout << "Error loading " << TEXTURES + file << endl;
            return false;
        }
        t.setSmooth(true);
        return true;
    };

    if (!load(bgTexture,          "bg.png")) return 1;
    if (!load(shipTexture,        "ship.png")) return 1;
    if (!load(wavesTexture,       "1waves.png")) return 1;

    if (!load(redBirdTexture,     "bird_red.png")) return 1;
    if (!load(blueBirdTexture,    "bird_blue.png")) return 1;
    if (!load(yellowBirdTexture,  "bird_yellow.png")) return 1;

    if (!load(weakPigTexture,     "Pig.png")) return 1;
    if (!load(strongPigTexture,   "Pig.png")) return 1;

    if (!load(iceTexture,         "wood-block.png")) return 1;
    if (!load(woodTexture,        "wood-block.png")) return 1;
    if (!load(stoneTexture,       "wood-block.png")) return 1;

    // =====================================================
    // BACKGROUND MUSIC
    // =====================================================
    sf::Music music;
    if (!music.openFromFile(AUDIO + "Angry Birds Theme Song.mp3")) {
        cout << "Warning: could not load " 
             << AUDIO + "Angry Birds Theme Song.mp3" << endl;
    } else {
        music.setLoop(true);
        music.play();
    }

    // =====================================================
    // BACKGROUND SETUP
    // =====================================================
    sf::Sprite bgSprite(bgTexture);
    sf::IntRect bgRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    bgSprite.setTextureRect(bgRect);

    const float BG_SPEED = 200.0f;
    int bgMaxOffset = bgTexture.getSize().x - WINDOW_WIDTH;
    if (bgMaxOffset < 0) bgMaxOffset = 0;
    float bgOffsetX = 0.0f;

    // =====================================================
    // SHIP SETUP
    // =====================================================
    sf::Sprite shipSprite(shipTexture);
    float desiredShipHeight = WINDOW_HEIGHT * 0.3f;
    float shipScale = desiredShipHeight / (float)shipTexture.getSize().y;
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

    float wavesWidth  = wavesTexture.getSize().x;
    float wavesHeight = wavesTexture.getSize().y;

    float wavesScaleX = 2.0f;
    float wavesScaleY = 1.5f;

    waves1.setScale(wavesScaleX, wavesScaleY);
    waves2.setScale(wavesScaleX, wavesScaleY);

    float wavesWidthScaled  = wavesWidth  * wavesScaleX;
    float wavesHeightScaled = wavesHeight * wavesScaleY;

    float wavesY = WINDOW_HEIGHT - wavesHeightScaled;

    waves1.setPosition(0.0f, wavesY);
    waves2.setPosition(wavesWidthScaled, wavesY);

    const float WAVES_SPEED = 300.0f;

    // =====================================================
    // PIGS + OBSTACLES
    // =====================================================
    vector<Pig*> pigs;
    vector<Obstacle*> obstacles;

    pigs.push_back(new WeakPig(weakPigTexture,   sf::Vector2f(1200.0f, 650.0f)));
    pigs.push_back(new StrongPig(strongPigTexture, sf::Vector2f(1500.0f, 650.0f)));

    obstacles.push_back(new IceObstacle(iceTexture,   sf::Vector2f(950.0f, 700.0f)));
    obstacles.push_back(new WoodObstacle(woodTexture, sf::Vector2f(1100.0f, 700.0f)));
    obstacles.push_back(new StoneObstacle(stoneTexture, sf::Vector2f(1350.0f, 700.0f)));

    for (auto p : pigs)       p->getSprite().setScale(0.5f, 0.5f);
    for (auto o : obstacles)  o->getSprite().setScale(0.7f, 0.7f);

    // =====================================================
    // BIRDS
    // =====================================================
    float desiredBirdHeight = shipBounds.height * 0.25f;
    float birdScale = desiredBirdHeight / (float)redBirdTexture.getSize().y;

    unique_ptr<Bird> currentBird;

    // =====================================================
    // GAME STATE
    // =====================================================
    sf::Clock clock;
    bool scrolling = true;
    bool dragging = false;

    sf::Vector2f dragStartPos;
    sf::Vector2f birdVelocity(0,0);
    bool birdLaunched = false;

    const float GRAVITY = 500.0f;

    // =====================================================
    // GAME LOOP
    // =====================================================
    while (window.isOpen()) {

        // ------------------- EVENTS -----------------------
        sf::Event event;
        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {

                if (event.key.code == sf::Keyboard::Space) {
                    scrolling = !scrolling;
                    if (scrolling) music.play();
                    else music.pause();
                }

                if (event.key.code == sf::Keyboard::Num1 ||
                    event.key.code == sf::Keyboard::Num2 ||
                    event.key.code == sf::Keyboard::Num3) 
                {
                    int type = (event.key.code == sf::Keyboard::Num1) ? 1 :
                               (event.key.code == sf::Keyboard::Num2) ? 2 : 3;

                    currentBird.reset();
                    birdLaunched = false;
                    birdVelocity = {0,0};

                    currentBird = createBird(
                        type,
                        redBirdTexture,
                        blueBirdTexture,
                        yellowBirdTexture,
                        birdDeckX, birdDeckY, birdScale
                    );
                }

                if (event.key.code == sf::Keyboard::Up &&
                    currentBird && !currentBird->isLaunched())
                    currentBird->lift();
            }

            // drag start
            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);

                if (currentBird &&
                    currentBird->getSprite().getGlobalBounds().contains(mousePos))
                {
                    dragging = true;
                    dragStartPos = mousePos;
                }
            }

            // drag release → launch
            if (event.type == sf::Event::MouseButtonReleased &&
                event.mouseButton.button == sf::Mouse::Left &&
                dragging)
            {
                dragging = false;

                if (currentBird) {
                    birdLaunched = true;
                    currentBird->launch();

                    sf::Vector2f releasePos(event.mouseButton.x,event.mouseButton.y);
                    sf::Vector2f dragVector = dragStartPos - releasePos;

                    dragVector.y -= 100.f;

                    float velocityScale = 5.f;
                    birdVelocity = dragVector * velocityScale;

                    float speed = sqrt(birdVelocity.x * birdVelocity.x +
                                       birdVelocity.y * birdVelocity.y);

                    float maxSpeed = 2000.f;
                    if (speed > maxSpeed) {
                        birdVelocity = (birdVelocity / speed) * maxSpeed;
                    }
                }
            }
        }

        // ------------------ UPDATE ------------------------
        float dt = clock.restart().asSeconds();

        // background scroll
        if (scrolling) {
            bgOffsetX += BG_SPEED * dt;
            if (bgOffsetX > bgMaxOffset) bgOffsetX -= bgMaxOffset;

            bgRect.left = (int)bgOffsetX;
            bgSprite.setTextureRect(bgRect);

            waves1.move(-WAVES_SPEED * dt, 0);
            waves2.move(-WAVES_SPEED * dt, 0);

            if (waves1.getPosition().x <= -wavesWidthScaled)
                waves1.setPosition(waves2.getPosition().x + wavesWidthScaled, wavesY);

            if (waves2.getPosition().x <= -wavesWidthScaled)
                waves2.setPosition(waves1.getPosition().x + wavesWidthScaled, wavesY);
        }

        // pigs + obstacles
        for (auto p : pigs) p->update(dt);
        for (auto o : obstacles) o->update(dt);

        // bird update
        if (currentBird) {

            if (dragging) {
                auto mp = sf::Mouse::getPosition(window);
                currentBird->getSprite().setPosition((float)mp.x,(float)mp.y);
            }

            else if (birdLaunched) {

                birdVelocity.y += GRAVITY * dt;
                currentBird->getSprite().move(birdVelocity * dt);

                const auto& birdSprite = currentBird->getSprite();

                // collisions
                for (auto o : obstacles)
                    handleBirdObstacleCollision(birdSprite, birdVelocity, *o);

                for (auto p : pigs)
                    handleBirdPigCollision(birdSprite, birdVelocity, *p);

                // out of bounds
                auto pos = currentBird->getSprite().getPosition();
                if (pos.y > WINDOW_HEIGHT + 300 ||
                    pos.x > WINDOW_WIDTH + 300 ||
                    pos.x < -300)
                {
                    currentBird.reset();
                    birdLaunched = false;
                    birdVelocity = {0,0};
                }
            }

            else {
                currentBird->setDeckPosition({birdDeckX, birdDeckY});
            }
        }

        // ------------------ DRAW --------------------------
        window.clear(sf::Color::Black);

        window.draw(bgSprite);
        window.draw(shipSprite);
        window.draw(waves1);
        window.draw(waves2);

        for (auto o : obstacles) window.draw(o->getSprite());
        for (auto p : pigs)      window.draw(p->getSprite());

        if (currentBird) currentBird->draw(window);

        window.display();
    }

    // cleanup
    for (auto p : pigs) delete p;
    for (auto o : obstacles) delete o;

    return 0;
}
