#include "Graphics.hpp"
#include "Bird.hpp"
#include "Dev1.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include <string>

using namespace std;

// folder paths
const string TEXTURES = "assets/textures/";
const string AUDIO    = "assets/audio/";

// Simple AABB collision helper
bool checkSpriteCollision(const sf::Sprite & a, const sf::Sprite & b)
{
    return a.getGlobalBounds().intersects(b.getGlobalBounds());
}

bool loadTexture(sf::Texture & t, const string & file)
{
    if (t.loadFromFile(TEXTURES + file) == false)
    {
        cout << "Error loading " << TEXTURES + file << endl;
        return false;
    }
    t.setSmooth(true);
    return true;
}

// =====================
// main
// =====================
int main()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
                            "Angry Chiriya");
    window.setFramerateLimit(60);

    // =====================================================
    // load textures
    // =====================================================
    sf::Texture bgTexture;
    sf::Texture shipTexture;
    sf::Texture wavesTexture;
    sf::Texture redBirdTexture;
    sf::Texture blueBirdTexture;
    sf::Texture yellowBirdTexture;
    sf::Texture weakPigTexture;
    sf::Texture strongPigTexture;
    sf::Texture kingPigTexture;
    sf::Texture iceTexture;
    sf::Texture woodTexture;
    sf::Texture stoneTexture;
    sf::Texture crateTexture;
    sf::Texture slingBackTexture;
    sf::Texture slingFrontTexture;

    if (loadTexture(bgTexture, "bg.png") == false) return 1;
    if (loadTexture(shipTexture, "ship.png") == false) return 1;
    if (loadTexture(wavesTexture, "1waves.png") == false) return 1;

    if (loadTexture(redBirdTexture, "bird_red.png") == false) return 1;
    if (loadTexture(blueBirdTexture, "bird_blue.png") == false) return 1;
    if (loadTexture(yellowBirdTexture, "bird_yellow.png") == false) return 1;

    if (loadTexture(weakPigTexture, "pig1.png") == false) return 1;
    if (loadTexture(strongPigTexture, "pig2.png") == false) return 1;
    if (loadTexture(kingPigTexture, "pig3.png") == false) return 1;

    if (loadTexture(iceTexture, "block_ice.png") == false) return 1;
    if (loadTexture(woodTexture, "block_wood.png") == false) return 1;
    if (loadTexture(stoneTexture, "block_stone.png") == false) return 1;

    if (loadTexture(crateTexture, "CrateBird.png") == false) return 1;

    if (loadTexture(slingBackTexture, "slingBackTexture.png") == false) return 1;
    if (loadTexture(slingFrontTexture, "slingFrontTexture.png") == false) return 1;

    // =====================================================
    // hud + txt
    // =====================================================
    sf::Font hudFont;
    if (hudFont.loadFromFile("assets/fonts/arial.ttf") == false)
    {
        cout << "Warning: could not load assets/fonts/arial.ttf" << endl;
    }

    sf::Text hudTextRed;
    sf::Text hudTextBlue;
    sf::Text hudTextYellow;

    sf::Sprite hudRedSprite;
    sf::Sprite hudBlueSprite;
    sf::Sprite hudYellowSprite;

    sf::RectangleShape resetButton;
    sf::Text resetText;

    sf::Text gameOverText;
    sf::Text scoreText;

    if (hudFont.getInfo().family != "")
    {
        hudTextRed.setFont(hudFont);
        hudTextBlue.setFont(hudFont);
        hudTextYellow.setFont(hudFont);

        hudTextRed.setCharacterSize(24);
        hudTextBlue.setCharacterSize(24);
        hudTextYellow.setCharacterSize(24);

        hudTextRed.setFillColor(sf::Color::White);
        hudTextBlue.setFillColor(sf::Color::White);
        hudTextYellow.setFillColor(sf::Color::White);

        hudRedSprite.setTexture(redBirdTexture);
        hudBlueSprite.setTexture(blueBirdTexture);
        hudYellowSprite.setTexture(yellowBirdTexture);

        float S = 0.25f;
        hudRedSprite.setScale(S, S);
        hudBlueSprite.setScale(S, S);
        hudYellowSprite.setScale(S, S);

        hudRedSprite.setPosition(40.0f, 10.0f);
        hudBlueSprite.setPosition(260.0f, 10.0f);
        hudYellowSprite.setPosition(520.0f, 10.0f);

        hudTextRed.setPosition(100.0f, 10.0f);
        hudTextBlue.setPosition(320.0f, 10.0f);
        hudTextYellow.setPosition(580.0f, 10.0f);

        gameOverText.setFont(hudFont);
        gameOverText.setString("GAME OVER");
        gameOverText.setCharacterSize(64);
        gameOverText.setFillColor(sf::Color::White);

        sf::FloatRect goBounds = gameOverText.getLocalBounds();
        gameOverText.setOrigin(goBounds.left + goBounds.width / 2.0f,
                               goBounds.top + goBounds.height / 2.0f);
        gameOverText.setPosition(WINDOW_WIDTH / 2.0f,
                                 WINDOW_HEIGHT / 2.0f - 40.0f);

        scoreText.setFont(hudFont);
        scoreText.setString("Score: 0");
        scoreText.setCharacterSize(36);
        scoreText.setFillColor(sf::Color::White);

        sf::FloatRect scBounds = scoreText.getLocalBounds();
        scoreText.setOrigin(scBounds.left + scBounds.width / 2.0f,
                            scBounds.top + scBounds.height / 2.0f);
        scoreText.setPosition(WINDOW_WIDTH / 2.0f,
                              WINDOW_HEIGHT / 2.0f + 20.0f);

        resetButton.setSize(sf::Vector2f(120.0f, 40.0f));
        resetButton.setFillColor(sf::Color(80, 80, 80));
        resetButton.setOutlineColor(sf::Color::White);
        resetButton.setOutlineThickness(2.0f);
        resetButton.setPosition(WINDOW_WIDTH - 140.0f, 10.0f);

        resetText.setFont(hudFont);
        resetText.setString("Reset");
        resetText.setCharacterSize(22);
        resetText.setFillColor(sf::Color::White);
        resetText.setPosition(WINDOW_WIDTH - 120.0f, 12.0f);
    }

    // =====================================================
    // bg music
    // =====================================================
    sf::Music music;
    if (music.openFromFile(AUDIO + "Angry Birds Theme Song.mp3") == false)
    {
        cout << "Warning: could not load " << AUDIO << "Angry Birds Theme Song.mp3" << endl;
    }
    else
    {
        music.setLoop(true);
        music.play();
    }

    // =====================================================
    // bg setup
    // =====================================================
    sf::Sprite bgSprite;
    bgSprite.setTexture(bgTexture);

    sf::IntRect bgRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    bgSprite.setTextureRect(bgRect);

    float BG_SPEED = 200.0f;
    int bgMaxOffset = (int)bgTexture.getSize().x - WINDOW_WIDTH;
    if (bgMaxOffset < 0)
    {
        bgMaxOffset = 0;
    }
    float bgOffsetX = 0.0f;

    // ------------------------------------------------------------------------------------------
    // ship setup

    sf::Sprite shipSprite;
    shipSprite.setTexture(shipTexture);

    float desiredShipHeight = WINDOW_HEIGHT * 0.3f;
    float shipScale = desiredShipHeight / (float)shipTexture.getSize().y;
    shipSprite.setScale(shipScale, shipScale);

    float shipX = WINDOW_WIDTH * 0.08f;
    float shipY = WINDOW_HEIGHT * 0.65f;
    shipSprite.setPosition(shipX, shipY);

    sf::FloatRect shipBounds = shipSprite.getGlobalBounds();

    // -------------------------------------------------------------------------------
    // slingshot setup (back stick + front piece)
    //  - base sits on the ship
    //  - bird will spawn aligned with this

    sf::Sprite slingBackSprite;
    sf::Sprite slingFrontSprite;

    slingBackSprite.setTexture(slingBackTexture);
    slingFrontSprite.setTexture(slingFrontTexture);

    float slingDesiredHeight = shipBounds.height * 0.45f;
    float slingScale = slingDesiredHeight / (float)slingBackTexture.getSize().y;

    slingBackSprite.setScale(slingScale, slingScale);
    slingFrontSprite.setScale(slingScale, slingScale);

    sf::FloatRect backBounds = slingBackSprite.getLocalBounds();
    sf::FloatRect frontBounds = slingFrontSprite.getLocalBounds();

    slingBackSprite.setOrigin(backBounds.width * 0.5f, backBounds.height);
    slingFrontSprite.setOrigin(frontBounds.width * 0.5f, frontBounds.height);

    // place base on the deck, roughly in the middle of the ship
    float slingX = shipBounds.left + shipBounds.width * 0.20f;
    float slingY = shipBounds.top + shipBounds.height * 0.45f;

    slingBackSprite.setPosition(slingX, slingY);
    slingFrontSprite.setPosition(slingX, slingY);

    // bird "rest" position – slightly above sling base so it sits in the fork
    float birdDeckX = slingX - 25.0;
    float birdDeckY = slingY - 170.0;

    // =====================================================
    // WAVES SETUP
    // =====================================================
    sf::Sprite waves1;
    sf::Sprite waves2;
    waves1.setTexture(wavesTexture);
    waves2.setTexture(wavesTexture);

    float wavesWidth = (float)wavesTexture.getSize().x;
    float wavesHeight = (float)wavesTexture.getSize().y;

    float wavesScaleX = 2.0f;
    float wavesScaleY = 1.5f;

    waves1.setScale(wavesScaleX, wavesScaleY);
    waves2.setScale(wavesScaleX, wavesScaleY);

    float wavesWidthScaled = wavesWidth * wavesScaleX;
    float wavesHeightScaled = wavesHeight * wavesScaleY;

    float wavesY = WINDOW_HEIGHT - wavesHeightScaled;

    waves1.setPosition(0.0f, wavesY);
    waves2.setPosition(wavesWidthScaled, wavesY);

    float WAVES_SPEED = 300.0f;

    // =====================================================
    // pigs n obstacles
    // =====================================================
    vector<Pig *> pigs;
    vector<Obstacle *> obstacles;

    pigs.push_back(new WeakPig(weakPigTexture, sf::Vector2f(1200.0f, 650.0f)));
    pigs.push_back(new StrongPig(strongPigTexture, sf::Vector2f(1500.0f, 650.0f)));
    // later: pigs.push_back(new KingPig(kingPigTexture, sf::Vector2f(...)));

    int i = 0;
    for (i = 0; i < (int)pigs.size(); i++)
    {
        pigs[i]->getSprite().setScale(0.5f, 0.5f);
    }

    obstacles.push_back(new IceObstacle(iceTexture, sf::Vector2f(950.0f, 700.0f)));
    obstacles.push_back(new WoodObstacle(woodTexture, sf::Vector2f(1100.0f, 700.0f)));
    obstacles.push_back(new StoneObstacle(stoneTexture, sf::Vector2f(1350.0f, 700.0f)));

    for (i = 0; i < (int)obstacles.size(); i++)
    {
        obstacles[i]->getSprite().setScale(0.7f, 0.7f);
    }

    // =====================================================
    // crate
    // =====================================================
    sf::Sprite powerUpSprite;
    bool powerUpActive = true;
    powerUpSprite.setTexture(crateTexture);
    powerUpSprite.setScale(0.23f, 0.23f);
    powerUpSprite.setPosition(1300.0f, 450.0f);

    // =====================================================
    // birds
    // =====================================================
    float desiredBirdHeight = shipBounds.height * 0.25f;
    float birdScale = desiredBirdHeight / (float)redBirdTexture.getSize().y;

    Bird * currentBird = 0;

    int redBirdCount = 3;
    int blueBirdCount = 3;
    int yellowBirdCount = 3;

    int currentBirdType = 0;

    // =====================================================
    // GAME STATE
    // =====================================================
    sf::Clock clock;
    bool scrolling = true;
    bool dragging = false;

    sf::Vector2f dragStartPos;
    sf::Vector2f birdVelocity(0.0f, 0.0f);
    bool birdLaunched = false;

    float GRAVITY = 500.0f;

    bool gameOver = false;

    // =====================================================
    // GAME LOOP
    // =====================================================
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f mousePos((float)event.mouseButton.x,
                                      (float)event.mouseButton.y);

                if (resetButton.getGlobalBounds().contains(mousePos))
                {
                    for (i = 0; i < (int)pigs.size(); i++)
                    {
                        delete pigs[i];
                    }
                    pigs.clear();

                    for (i = 0; i < (int)obstacles.size(); i++)
                    {
                        delete obstacles[i];
                    }
                    obstacles.clear();

                    pigs.push_back(new WeakPig(weakPigTexture, sf::Vector2f(1200.0f, 650.0f)));
                    pigs.push_back(new StrongPig(strongPigTexture, sf::Vector2f(1500.0f, 650.0f)));

                    for (i = 0; i < (int)pigs.size(); i++)
                    {
                        pigs[i]->getSprite().setScale(0.5f, 0.5f);
                    }

                    obstacles.push_back(new IceObstacle(iceTexture, sf::Vector2f(950.0f, 700.0f)));
                    obstacles.push_back(new WoodObstacle(woodTexture, sf::Vector2f(1100.0f, 700.0f)));
                    obstacles.push_back(new StoneObstacle(stoneTexture, sf::Vector2f(1350.0f, 700.0f)));

                    for (i = 0; i < (int)obstacles.size(); i++)
                    {
                        obstacles[i]->getSprite().setScale(0.7f, 0.7f);
                    }

                    powerUpActive = true;
                    powerUpSprite.setScale(0.23f, 0.23f);
                    powerUpSprite.setPosition(1300.0f, 450.0f);

                    if (currentBird != 0)
                    {
                        delete currentBird;
                        currentBird = 0;
                    }
                    currentBirdType = 0;
                    birdLaunched = false;
                    dragging = false;
                    birdVelocity.x = 0.0f;
                    birdVelocity.y = 0.0f;

                    redBirdCount = 3;
                    blueBirdCount = 3;
                    yellowBirdCount = 3;

                    bgOffsetX = 0.0f;

                    gameOver = false;
                    scrolling = true;
                    if (music.getStatus() != sf::Music::Playing)
                    {
                        music.play();
                    }

                    continue;
                }
            }

            if (gameOver == true)
            {
                continue;
            }

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Space)
                {
                    scrolling = !scrolling;
                    if (scrolling == true)
                    {
                        music.play();
                    }
                    else
                    {
                        music.pause();
                    }
                }

                if (event.key.code == sf::Keyboard::Num1 ||
                    event.key.code == sf::Keyboard::Num2 ||
                    event.key.code == sf::Keyboard::Num3)
                {
                    int requestedType = 0;
                    if (event.key.code == sf::Keyboard::Num1)
                    {
                        requestedType = 1;
                    }
                    else if (event.key.code == sf::Keyboard::Num2)
                    {
                        requestedType = 2;
                    }
                    else
                    {
                        requestedType = 3;
                    }

                    bool hasBirds = true;
                    if (requestedType == 1 && redBirdCount <= 0)
                    {
                        hasBirds = false;
                    }
                    if (requestedType == 2 && blueBirdCount <= 0)
                    {
                        hasBirds = false;
                    }
                    if (requestedType == 3 && yellowBirdCount <= 0)
                    {
                        hasBirds = false;
                    }

                    if (hasBirds == true)
                    {
                        if (currentBird != 0)
                        {
                            delete currentBird;
                            currentBird = 0;
                        }

                        currentBirdType = requestedType;
                        birdLaunched = false;
                        birdVelocity.x = 0.0f;
                        birdVelocity.y = 0.0f;

                        currentBird = createBird(requestedType,
                                                 redBirdTexture,
                                                 blueBirdTexture,
                                                 yellowBirdTexture,
                                                 birdDeckX, birdDeckY, birdScale);
                    }
                }

                if (event.key.code == sf::Keyboard::Up &&
                    currentBird != 0 &&
                    currentBird->isLaunched() == false)
                {
                    currentBird->lift();
                }
            }

            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f mousePos2((float)event.mouseButton.x,
                                       (float)event.mouseButton.y);

                if (currentBird != 0 &&
                    birdLaunched == false &&
                    currentBird->getSprite().getGlobalBounds().contains(mousePos2))
                {
                    dragging = true;
                    dragStartPos = mousePos2;
                }
                else if (currentBird != 0 && birdLaunched == true)
                {
                    currentBird->specialAbility(birdVelocity);
                }
            }

            if (event.type == sf::Event::MouseButtonReleased &&
                event.mouseButton.button == sf::Mouse::Left &&
                dragging == true)
            {
                dragging = false;

                if (currentBird != 0)
                {
                    birdLaunched = true;
                    currentBird->launch();

                    sf::Vector2f releasePos((float)event.mouseButton.x,
                                            (float)event.mouseButton.y);

                    sf::Vector2f dragVector;
                    dragVector.x = dragStartPos.x - releasePos.x;
                    dragVector.y = dragStartPos.y - releasePos.y;

                    dragVector.y = dragVector.y - 100.0f;

                    float velocityScale = 5.0f;
                    birdVelocity.x = dragVector.x * velocityScale;
                    birdVelocity.y = dragVector.y * velocityScale;

                    float speed = sqrt(birdVelocity.x * birdVelocity.x +
                                       birdVelocity.y * birdVelocity.y);

                    float maxSpeed = 2000.0f;
                    if (speed > maxSpeed)
                    {
                        float scale = maxSpeed / speed;
                        birdVelocity.x = birdVelocity.x * scale;
                        birdVelocity.y = birdVelocity.y * scale;
                    }

                    if (currentBirdType == 1 && redBirdCount > 0)
                    {
                        redBirdCount = redBirdCount - 1;
                    }
                    else if (currentBirdType == 2 && blueBirdCount > 0)
                    {
                        blueBirdCount = blueBirdCount - 1;
                    }
                    else if (currentBirdType == 3 && yellowBirdCount > 0)
                    {
                        yellowBirdCount = yellowBirdCount - 1;
                    }
                }
            }
        }

        float dt = clock.restart().asSeconds();

        if (gameOver == false)
        {
            if (scrolling == true)
            {
                bgOffsetX = bgOffsetX + BG_SPEED * dt;
                if (bgMaxOffset > 0 && bgOffsetX > bgMaxOffset)
                {
                    bgOffsetX = bgOffsetX - bgMaxOffset;
                }

                bgRect.left = (int)bgOffsetX;
                bgSprite.setTextureRect(bgRect);

                waves1.move(-WAVES_SPEED * dt, 0.0f);
                waves2.move(-WAVES_SPEED * dt, 0.0f);

                if (waves1.getPosition().x <= -wavesWidthScaled)
                {
                    waves1.setPosition(waves2.getPosition().x + wavesWidthScaled, wavesY);
                }

                if (waves2.getPosition().x <= -wavesWidthScaled)
                {
                    waves2.setPosition(waves1.getPosition().x + wavesWidthScaled, wavesY);
                }
            }

            for (i = 0; i < (int)pigs.size(); i++)
            {
                pigs[i]->update(dt);
            }

            for (i = 0; i < (int)obstacles.size(); i++)
            {
                obstacles[i]->update(dt);
            }

            if (currentBird != 0)
            {
                if (dragging == true)
                {
                    sf::Vector2i mp = sf::Mouse::getPosition(window);
                    currentBird->getSprite().setPosition((float)mp.x, (float)mp.y);
                }
                else if (birdLaunched == true)
                {
                    birdVelocity.y = birdVelocity.y + GRAVITY * dt;

                    currentBird->update(dt, birdVelocity);

                    const sf::Sprite & birdSprite = currentBird->getSprite();

                    for (i = 0; i < (int)obstacles.size(); i++)
                    {
                        handleBirdObstacleCollision(birdSprite,
                                                    birdVelocity,
                                                    *obstacles[i]);
                    }

                    for (i = 0; i < (int)pigs.size(); i++)
                    {
                        handleBirdPigCollision(birdSprite,
                                               birdVelocity,
                                               *pigs[i]);
                    }

                    if (powerUpActive == true &&
                        checkSpriteCollision(birdSprite, powerUpSprite))
                    {
                        powerUpActive = false;

                        int firstType = 1;
                        int firstCount = redBirdCount;

                        if (blueBirdCount < firstCount)
                        {
                            firstType = 2;
                            firstCount = blueBirdCount;
                        }
                        if (yellowBirdCount < firstCount)
                        {
                            firstType = 3;
                            firstCount = yellowBirdCount;
                        }

                        int secondType;
                        int secondCount;

                        if (firstType == 1)
                        {
                            secondType = 2;
                            secondCount = blueBirdCount;
                            if (yellowBirdCount < secondCount)
                            {
                                secondType = 3;
                                secondCount = yellowBirdCount;
                            }
                        }
                        else if (firstType == 2)
                        {
                            secondType = 1;
                            secondCount = redBirdCount;
                            if (yellowBirdCount < secondCount)
                            {
                                secondType = 3;
                                secondCount = yellowBirdCount;
                            }
                        }
                        else
                        {
                            secondType = 1;
                            secondCount = redBirdCount;
                            if (blueBirdCount < secondCount)
                            {
                                secondType = 2;
                                secondCount = blueBirdCount;
                            }
                        }

                        if (firstType == 1)
                        {
                            redBirdCount = redBirdCount + 1;
                        }
                        else if (firstType == 2)
                        {
                            blueBirdCount = blueBirdCount + 1;
                        }
                        else if (firstType == 3)
                        {
                            yellowBirdCount = yellowBirdCount + 1;
                        }

                        if (secondType == 1)
                        {
                            redBirdCount = redBirdCount + 1;
                        }
                        else if (secondType == 2)
                        {
                            blueBirdCount = blueBirdCount + 1;
                        }
                        else if (secondType == 3)
                        {
                            yellowBirdCount = yellowBirdCount + 1;
                        }
                    }

                    sf::Vector2f pos = currentBird->getSprite().getPosition();
                    if (pos.y > WINDOW_HEIGHT + 300.0f ||
                        pos.x > WINDOW_WIDTH + 300.0f ||
                        pos.x < -300.0f)
                    {
                        delete currentBird;
                        currentBird = 0;
                        currentBirdType = 0;
                        birdLaunched = false;
                        birdVelocity.x = 0.0f;
                        birdVelocity.y = 0.0f;

                        int totalBirds = redBirdCount + blueBirdCount + yellowBirdCount;
                        if (totalBirds <= 0)
                        {
                            gameOver = true;
                            scrolling = false;
                            music.pause();
                        }
                    }
                }
                else
                {
                    currentBird->setDeckPosition(sf::Vector2f(birdDeckX, birdDeckY));
                }
            }

            if (hudFont.getInfo().family != "")
            {
                hudTextRed.setString(std::to_string(redBirdCount));
                hudTextBlue.setString(std::to_string(blueBirdCount));
                hudTextYellow.setString(std::to_string(yellowBirdCount));
            }
        }

        window.clear(sf::Color::Black);

        window.draw(bgSprite);
        window.draw(shipSprite);
        window.draw(waves1);
        window.draw(waves2);

        for (i = 0; i < (int)obstacles.size(); i++)
        {
            window.draw(obstacles[i]->getSprite());
        }

        for (i = 0; i < (int)pigs.size(); i++)
        {
            window.draw(pigs[i]->getSprite());
        }

        if (powerUpActive == true)
        {
            window.draw(powerUpSprite);
        }

        // slingshot layering:
        // back stick (bottom), bird (middle), front piece (top)
        window.draw(slingBackSprite);

        if (currentBird != 0)
        {
            currentBird->draw(window);
        }

        window.draw(slingFrontSprite);

        window.draw(hudRedSprite);
        window.draw(hudBlueSprite);
        window.draw(hudYellowSprite);

        if (hudFont.getInfo().family != "")
        {
            window.draw(hudTextRed);
            window.draw(hudTextBlue);
            window.draw(hudTextYellow);
            window.draw(resetButton);
            window.draw(resetText);
        }

        if (gameOver == true && hudFont.getInfo().family != "")
        {
            window.draw(gameOverText);
            window.draw(scoreText);
        }

        window.display();
    }

    for (i = 0; i < (int)pigs.size(); i++)
    {
        delete pigs[i];
    }
    for (i = 0; i < (int)obstacles.size(); i++)
    {
        delete obstacles[i];
    }

    if (currentBird != 0)
    {
        delete currentBird;
    }

    return 0;
}
