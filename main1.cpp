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

// --------------------------------------------------------------------
// Folder paths
// --------------------------------------------------------------------
const string TEXTURES = "assets/textures/";
const string AUDIO    = "assets/audio/";

// --------------------------------------------------------------------
// Simple AABB collision helper (for power-up crate etc.)
// --------------------------------------------------------------------
bool checkSpriteCollision(const sf::Sprite & a, const sf::Sprite & b)
{
    return a.getGlobalBounds().intersects(b.getGlobalBounds());
}

// --------------------------------------------------------------------
// Texture load helper
// --------------------------------------------------------------------
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

// --------------------------------------------------------------------
// Helpers to clear and build levels (pigs + obstacles layouts)
// These are taken from main1.cpp and adapted to your current code.
// --------------------------------------------------------------------
void clearLevelVectors(vector<Pig*> & pigs,
                       vector<Obstacle*> & obstacles)
{
    int i = 0;
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
}

// Build the pigs + obstacles layout for one level (1..5)
// Also positions the power-up crate for that level.
void buildLevel(int level,
                vector<Pig*> & pigs,
                vector<Obstacle*> & obstacles,
                const sf::Texture & weakPigTexture,
                const sf::Texture & strongPigTexture,
                const sf::Texture & kingPigTexture,
                const sf::Texture & iceTexture,
                const sf::Texture & woodTexture,
                const sf::Texture & stoneTexture,
                sf::Sprite & crateSprite,
                bool & powerUpActive)
{
    clearLevelVectors(pigs, obstacles);

    // Baseline and "ground" used for layouts (from your layout.pdf)
    float BASELINE_X = WINDOW_WIDTH * 0.7f;
    float GROUND_Y   = WINDOW_HEIGHT * 0.65f;

    powerUpActive = true;

    // --------------- LEVEL 1 ---------------
    if (level == 1)
    {
        float xRight = BASELINE_X - 80.0f;

        // stone block
        obstacles.push_back(
            new StoneObstacle(stoneTexture,
                              sf::Vector2f(xRight - 40.0f, GROUND_Y - 130.0f)));

        // ice plank near bottom
        obstacles.push_back(
            new IceObstacle(iceTexture,
                            sf::Vector2f(xRight + 20.0f, GROUND_Y - 20.0f)));

        // weak pigs
        pigs.push_back(
            new WeakPig(weakPigTexture, sf::Vector2f(xRight + 80.0f, GROUND_Y - 90.0f)));

        pigs.push_back(
            new WeakPig(weakPigTexture, sf::Vector2f(xRight - 120.0f, GROUND_Y - 230.0f)));

        // crate somewhere above the structure
        crateSprite.setPosition(xRight, GROUND_Y - 260.0f);
    }

    // --------------- LEVEL 2 ---------------
    else if (level == 2)
    {
        float columnX = BASELINE_X - 80.0f;

        // tall stone column
        obstacles.push_back(
            new StoneObstacle(stoneTexture,
                              sf::Vector2f(columnX, GROUND_Y - 130.0f)));

        // mid wood plank
        obstacles.push_back(
            new WoodObstacle(woodTexture,
                             sf::Vector2f(columnX + 100.0f, GROUND_Y - 120.0f)));

        // bottom ice plank
        obstacles.push_back(
            new IceObstacle(iceTexture,
                            sf::Vector2f(columnX + 120.0f, GROUND_Y - 40.0f)));

        // stone block on left as small launcher
        obstacles.push_back(
            new StoneObstacle(stoneTexture,
                              sf::Vector2f(columnX - 80.0f, GROUND_Y - 80.0f)));

        // two weak pigs on right (stacked)
        pigs.push_back(
            new WeakPig(weakPigTexture,
                        sf::Vector2f(columnX + 150.0f, GROUND_Y - 180.0f)));

        pigs.push_back(
            new WeakPig(weakPigTexture,
                        sf::Vector2f(columnX + 150.0f, GROUND_Y - 80.0f)));

        // crate slightly above and to the right
        crateSprite.setPosition(columnX + 200.0f, GROUND_Y - 220.0f);
    }

    // --------------- LEVEL 3 ---------------
    else if (level == 3)
    {
        float baseX = BASELINE_X - 200.0f;

        // two stone platforms (bottom + mid)
        obstacles.push_back(
            new StoneObstacle(stoneTexture,
                              sf::Vector2f(baseX, GROUND_Y - 20.0f)));

        obstacles.push_back(
            new StoneObstacle(stoneTexture,
                              sf::Vector2f(baseX, GROUND_Y - 90.0f)));

        // launcher stone block on left
        obstacles.push_back(
            new StoneObstacle(stoneTexture,
                              sf::Vector2f(baseX - 260.0f, GROUND_Y - 60.0f)));

        float towerX = BASELINE_X - 40.0f;

        // bottom weak pig
        pigs.push_back(
            new WeakPig(weakPigTexture,
                        sf::Vector2f(towerX, GROUND_Y - 50.0f)));

        // mid wood plank
        obstacles.push_back(
            new WoodObstacle(woodTexture,
                             sf::Vector2f(towerX, GROUND_Y - 110.0f)));

        // mid weak pig
        pigs.push_back(
            new WeakPig(weakPigTexture,
                        sf::Vector2f(towerX, GROUND_Y - 160.0f)));

        // upper wood plank
        obstacles.push_back(
            new WoodObstacle(woodTexture,
                             sf::Vector2f(towerX, GROUND_Y - 210.0f)));

        // strong pig at top
        pigs.push_back(
            new StrongPig(strongPigTexture,
                          sf::Vector2f(towerX, GROUND_Y - 280.0f)));

        // crate above the right tower
        crateSprite.setPosition(towerX + 60.0f, GROUND_Y - 320.0f);
    }

    // --------------- LEVEL 4 ---------------
    else if (level == 4)
    {
        float houseCenterX = BASELINE_X - 120.0f;

        // bottom wood beams
        obstacles.push_back(
            new WoodObstacle(woodTexture,
                             sf::Vector2f(houseCenterX - 80.0f, GROUND_Y - 40.0f)));

        obstacles.push_back(
            new WoodObstacle(woodTexture,
                             sf::Vector2f(houseCenterX + 80.0f, GROUND_Y - 40.0f)));

        // inner stone "walls"
        obstacles.push_back(
            new StoneObstacle(stoneTexture,
                              sf::Vector2f(houseCenterX - 40.0f, GROUND_Y - 150.0f)));

        obstacles.push_back(
            new StoneObstacle(stoneTexture,
                              sf::Vector2f(houseCenterX + 40.0f, GROUND_Y - 150.0f)));

        // ice roof
        obstacles.push_back(
            new IceObstacle(iceTexture,
                            sf::Vector2f(houseCenterX - 70.0f, GROUND_Y - 210.0f)));

        obstacles.push_back(
            new IceObstacle(iceTexture,
                            sf::Vector2f(houseCenterX + 70.0f, GROUND_Y - 210.0f)));

        // strong pig inside "house"
        pigs.push_back(
            new StrongPig(strongPigTexture,
                          sf::Vector2f(houseCenterX, GROUND_Y - 130.0f)));

        // weak pig outside on right
        pigs.push_back(
            new WeakPig(weakPigTexture,
                        sf::Vector2f(BASELINE_X - 10.0f, GROUND_Y - 180.0f)));

        // crate on roof
        crateSprite.setPosition(houseCenterX, GROUND_Y - 260.0f);
    }

    // --------------- LEVEL 5 ---------------
    else
    {
        // Final "castle" + KING pig
        float leftWallX = BASELINE_X - 360.0f;

        // left vertical stone wall
        obstacles.push_back(
            new StoneObstacle(stoneTexture,
                              sf::Vector2f(leftWallX, GROUND_Y - 140.0f)));

        // top ceiling
        obstacles.push_back(
            new StoneObstacle(stoneTexture,
                              sf::Vector2f(BASELINE_X - 80.0f, GROUND_Y - 280.0f)));

        // middle horizontal stones
        obstacles.push_back(
            new StoneObstacle(stoneTexture,
                              sf::Vector2f(BASELINE_X - 90.0f, GROUND_Y - 180.0f)));
        obstacles.push_back(
            new StoneObstacle(stoneTexture,
                              sf::Vector2f(BASELINE_X - 90.0f, GROUND_Y - 80.0f)));

        // central wood column
        obstacles.push_back(
            new WoodObstacle(woodTexture,
                             sf::Vector2f(BASELINE_X - 190.0f, GROUND_Y - 110.0f)));

        // inner ice "L" core
        obstacles.push_back(
            new IceObstacle(iceTexture,
                            sf::Vector2f(BASELINE_X - 120.0f, GROUND_Y - 120.0f)));
        obstacles.push_back(
            new IceObstacle(iceTexture,
                            sf::Vector2f(BASELINE_X - 40.0f, GROUND_Y - 120.0f)));

        // inner floor stone
        obstacles.push_back(
            new StoneObstacle(stoneTexture,
                              sf::Vector2f(BASELINE_X - 80.0f, GROUND_Y - 20.0f)));

        // KING pig at top right (now using kingPigTexture / pig3.png)
        pigs.push_back(
            new KingPig(kingPigTexture,
                        sf::Vector2f(BASELINE_X - 20.0f, GROUND_Y - 260.0f)));

        // weak pigs inside
        pigs.push_back(
            new WeakPig(weakPigTexture,
                        sf::Vector2f(BASELINE_X - 250.0f, GROUND_Y - 190.0f)));
        pigs.push_back(
            new WeakPig(weakPigTexture,
                        sf::Vector2f(BASELINE_X - 140.0f, GROUND_Y - 150.0f)));
        pigs.push_back(
            new WeakPig(weakPigTexture,
                        sf::Vector2f(BASELINE_X - 40.0f, GROUND_Y - 60.0f)));
        pigs.push_back(
            new WeakPig(weakPigTexture,
                        sf::Vector2f(BASELINE_X - 200.0f, GROUND_Y - 40.0f)));

        // crate floating above castle
        crateSprite.setPosition(BASELINE_X - 80.0f, GROUND_Y - 330.0f);
    }

    // scale as in your old main
    // scale pigs smaller based on type
    int i = 0;
    for (i = 0; i < (int)pigs.size(); i++)
    {
        string t = pigs[i]->getType();

        if (t == "weak")
        {
            pigs[i]->getSprite().setScale(0.08f, 0.08f);
        }
        else if (t == "strong")
        {
            pigs[i]->getSprite().setScale(0.1f, 0.1f);
        }
        else // "king"
        {
            pigs[i]->getSprite().setScale(0.15f, 0.15f);
        }
    }

    // obstacles stay the same
    for (i = 0; i < (int)obstacles.size(); i++)
    {
        obstacles[i]->getSprite().setScale(0.7f, 0.7f);
    }

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

    if (loadTexture(bgTexture, "bg.png") == false) return 1;
    if (loadTexture(shipTexture, "ship.png") == false) return 1;
    if (loadTexture(wavesTexture, "1waves.png") == false) return 1;

    if (loadTexture(redBirdTexture, "bird_red.png") == false) return 1;
    if (loadTexture(blueBirdTexture, "bird_blue.png") == false) return 1;
    if (loadTexture(yellowBirdTexture, "bird_yellow.png") == false) return 1;

    // correct pig sprites
    if (loadTexture(weakPigTexture, "pig1.png") == false) return 1;   // weak
    if (loadTexture(strongPigTexture, "pig2.png") == false) return 1; // strong
    if (loadTexture(kingPigTexture,  "pig3.png") == false) return 1;  // king

    if (loadTexture(iceTexture, "wood-block.png") == false) return 1;
    if (loadTexture(woodTexture, "wood-block.png") == false) return 1;
    if (loadTexture(stoneTexture, "wood-block.png") == false) return 1;

    if (loadTexture(crateTexture, "CrateBird.png") == false)
    {
        // power-up is optional, so no return
    }

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
        // HUD number text
        hudTextRed.setFont(hudFont);
        hudTextBlue.setFont(hudFont);
        hudTextYellow.setFont(hudFont);

        hudTextRed.setCharacterSize(24);
        hudTextBlue.setCharacterSize(24);
        hudTextYellow.setCharacterSize(24);

        hudTextRed.setFillColor(sf::Color::White);
        hudTextBlue.setFillColor(sf::Color::White);
        hudTextYellow.setFillColor(sf::Color::White);

        // HUD bird icons
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

        // place text a bit to the right of each icon
        hudTextRed.setPosition(100.0f, 10.0f);
        hudTextBlue.setPosition(320.0f, 10.0f);
        hudTextYellow.setPosition(580.0f, 10.0f);

        // game Over text setup
        gameOverText.setFont(hudFont);
        gameOverText.setString("GAME OVER");
        gameOverText.setCharacterSize(64);
        gameOverText.setFillColor(sf::Color::White);

        sf::FloatRect goBounds = gameOverText.getLocalBounds();
        gameOverText.setOrigin(goBounds.left + goBounds.width / 2.0f,
                               goBounds.top + goBounds.height / 2.0f);
        gameOverText.setPosition(WINDOW_WIDTH / 2.0f,
                                 WINDOW_HEIGHT / 2.0f - 40.0f);

        // score text 
        scoreText.setFont(hudFont);
        scoreText.setString("Score: 0");
        scoreText.setCharacterSize(36);
        scoreText.setFillColor(sf::Color::White);

        sf::FloatRect scBounds = scoreText.getLocalBounds();
        scoreText.setOrigin(scBounds.left + scBounds.width / 2.0f,
                            scBounds.top + scBounds.height / 2.0f);
        scoreText.setPosition(WINDOW_WIDTH / 2.0f,
                              WINDOW_HEIGHT / 2.0f + 20.0f);

        // reset button
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
    // bg setup (5-island big image)
    // =====================================================
    sf::Sprite bgSprite;
    bgSprite.setTexture(bgTexture);

    sf::IntRect bgRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    bgSprite.setTextureRect(bgRect);

    int bgMaxOffset = (int)bgTexture.getSize().x - WINDOW_WIDTH;
    if (bgMaxOffset < 0)
    {
        bgMaxOffset = 0;
    }
    float bgOffsetX = 0.0f;

    const int MAX_LEVELS = 5;
    int currentLevel = 1;

    bool transitioningLevel = false;
    float levelScrollTime = 0.0f;
    float levelScrollDuration = 1.5f;
    float scrollStartOffset = 0.0f;
    float scrollTargetOffset = 0.0f;

    // ship =--------------------------------------
    sf::Sprite shipSprite;
    shipSprite.setTexture(shipTexture);

    float desiredShipHeight = WINDOW_HEIGHT * 0.3f;
    float shipTexH = (float)(shipTexture.getSize().y == 0 ? 1 : shipTexture.getSize().y);
    float shipScale = desiredShipHeight / shipTexH;
    shipSprite.setScale(shipScale, shipScale);

    float shipX = WINDOW_WIDTH * 0.08f;
    float shipY = WINDOW_HEIGHT * 0.65f;
    shipSprite.setPosition(shipX, shipY);

    sf::FloatRect shipBounds = shipSprite.getGlobalBounds();
    float birdDeckX = shipBounds.left + shipBounds.width * 0.45f;
    float birdDeckY = shipBounds.top + shipBounds.height * 0.25f;

    // waves -------------------------------------------
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

    // pigs n obstacles (per-level using layouts) ---------------------------------------------
    vector<Pig *> pigs;
    vector<Obstacle *> obstacles;

    // crate (power-up)
    sf::Sprite powerUpSprite;
    powerUpSprite.setTexture(crateTexture);
    powerUpSprite.setScale(0.23f, 0.23f);
    bool powerUpActive = true;

    // build level 1 initially
    buildLevel(currentLevel, pigs, obstacles,
               weakPigTexture, strongPigTexture, kingPigTexture,
               iceTexture, woodTexture, stoneTexture,
               powerUpSprite, powerUpActive);

    // birds -------------------------------------------------------
    float desiredBirdHeight = shipBounds.height * 0.25f;
    float birdScale = desiredBirdHeight /
                      (float)(redBirdTexture.getSize().y == 0 ? 1 : redBirdTexture.getSize().y);

    Bird * currentBird = 0;

    int redBirdCount = 3;
    int blueBirdCount = 3;
    int yellowBirdCount = 3;

    int currentBirdType = 0;

    // game state --------------------------------------------
    sf::Clock clock;
    bool scrolling = true;   // used as a pause flag
    bool dragging = false;

    sf::Vector2f dragStartPos;
    sf::Vector2f birdVelocity(0.0f, 0.0f);
    bool birdLaunched = false;

    float GRAVITY = 500.0f;

    bool gameOver = false;


    // game loop    --------------------------------------------
    while (window.isOpen())
    {
        // ------------------- EVENTS -----------------------
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            // reset button (always clickable, even on gameOver)
            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f mousePos((float)event.mouseButton.x,
                                      (float)event.mouseButton.y);

                if (resetButton.getGlobalBounds().contains(mousePos))
                {
                    // ===== reset everything to level 1 =====
                    clearLevelVectors(pigs, obstacles);

                    currentLevel = 1;
                    buildLevel(currentLevel, pigs, obstacles,
                               weakPigTexture, strongPigTexture, kingPigTexture,
                               iceTexture, woodTexture, stoneTexture,
                               powerUpSprite, powerUpActive);

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
                    transitioningLevel = false;
                    levelScrollTime = 0.0f;

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

            // during island scroll we ignore gameplay input
            if (transitioningLevel == true)
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

                    // consume one bird of this type on launch
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
        if (dt > 0.05f)
        {
            dt = 0.05f;
        }

        // ------------------ update bg 
        if (transitioningLevel == true)
        {
            levelScrollTime = levelScrollTime + dt;
            float t = levelScrollTime / levelScrollDuration;
            if (t > 1.0f)
            {
                t = 1.0f;
            }

            bgOffsetX = scrollStartOffset +
                        (scrollTargetOffset - scrollStartOffset) * t;

            if (bgOffsetX < 0.0f)
            {
                bgOffsetX = 0.0f;
            }
            if (bgOffsetX > (float)bgMaxOffset)
            {
                bgOffsetX = (float)bgMaxOffset;
            }

            bgRect.left = (int)bgOffsetX;
            bgSprite.setTextureRect(bgRect);

            if (t >= 1.0f)
            {
                transitioningLevel = false;
                currentLevel = currentLevel + 1;
                if (currentLevel > MAX_LEVELS)
                {
                    currentLevel = MAX_LEVELS;
                }

                // build new level
                buildLevel(currentLevel, pigs, obstacles,
                           weakPigTexture, strongPigTexture, kingPigTexture,
                           iceTexture, woodTexture, stoneTexture,
                           powerUpSprite, powerUpActive);

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

                bgOffsetX = (float)(currentLevel - 1) * WINDOW_WIDTH;
                if (bgOffsetX < 0.0f)
                {
                    bgOffsetX = 0.0f;
                }
                if (bgOffsetX > (float)bgMaxOffset)
                {
                    bgOffsetX = (float)bgMaxOffset;
                }

                bgRect.left = (int)bgOffsetX;
                bgSprite.setTextureRect(bgRect);
            }
        }
        else
        {
            // keep background aligned to the current island
            bgOffsetX = (float)(currentLevel - 1) * WINDOW_WIDTH;
            if (bgOffsetX < 0.0f)
            {
                bgOffsetX = 0.0f;
            }
            if (bgOffsetX > (float)bgMaxOffset)
            {
                bgOffsetX = (float)bgMaxOffset;
            }
            bgRect.left = (int)bgOffsetX;
            bgSprite.setTextureRect(bgRect);
        }

        // waves always animate only when scrolling == true
        if (scrolling == true)
        {
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

        // ------------------ update gameplay ------------------
        if (gameOver == false && transitioningLevel == false && scrolling == true)
        {
            int i = 0;

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
                        handleBirdObstacleCollision(birdSprite, birdVelocity,*obstacles[i]);
                    }

                    for (i = 0; i < (int)pigs.size(); i++)
                    {
                        handleBirdPigCollision(birdSprite,birdVelocity,*pigs[i]);
                    }

                    if (powerUpActive == true && checkSpriteCollision(birdSprite, powerUpSprite))
                    {
                        powerUpActive = false;

                        int c1 = redBirdCount;
                        int c2 = blueBirdCount;
                        int c3 = yellowBirdCount;

                        int firstType = 1;
                        int firstCount = c1;
                        if (c2 < firstCount)
                        {
                            firstType = 2;
                            firstCount = c2;
                        }
                        if (c3 < firstCount)
                        {
                            firstType = 3;
                            firstCount = c3;
                        }

                        int secondType = 1;
                        int secondCount = 999999;

                        if (firstType != 1 && c1 < secondCount)
                        {
                            secondType = 1;
                            secondCount = c1;
                        }
                        if (firstType != 2 && c2 < secondCount)
                        {
                            secondType = 2;
                            secondCount = c2;
                        }
                        if (firstType != 3 && c3 < secondCount)
                        {
                            secondType = 3;
                            secondCount = c3;
                        }

                        if (firstType == 1) redBirdCount++;
                        else if (firstType == 2) blueBirdCount++;
                        else yellowBirdCount++;

                        if (secondType == 1) redBirdCount++;
                        else if (secondType == 2) blueBirdCount++;
                        else yellowBirdCount++;
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
                            bool anyPigAlive = false;
                            for (i = 0; i < (int)pigs.size(); i++)
                            {
                                if (pigs[i]->isAlive())
                                {
                                    anyPigAlive = true;
                                }
                            }
                            if (anyPigAlive == true)
                            {
                                gameOver = true;
                                music.pause();
                            }
                        }
                    }
                }
                else
                {
                    currentBird->setDeckPosition(sf::Vector2f(birdDeckX, birdDeckY));
                }
            }

            // check if current level cleared
            bool anyPigAlive2 = false;
            int j = 0;
            for (j = 0; j < (int)pigs.size(); j++)
            {
                if (pigs[j]->isAlive())
                {
                    anyPigAlive2 = true;
                }
            }

            if (anyPigAlive2 == false)
            {
                if (currentLevel < MAX_LEVELS)
                {
                    // start smooth transition to next island
                    transitioningLevel = true;
                    levelScrollTime = 0.0f;
                    scrollStartOffset = (float)(currentLevel - 1) * WINDOW_WIDTH;
                    scrollTargetOffset = (float)currentLevel * WINDOW_WIDTH;

                    powerUpActive = false; // old crate disappears
                }
                else
                {
                    // last level cleared 
                    gameOver = true;
                    music.pause();
                }
            }

            if (hudFont.getInfo().family != "")
            {
                hudTextRed.setString(std::to_string(redBirdCount));
                hudTextBlue.setString(std::to_string(blueBirdCount));
                hudTextYellow.setString(std::to_string(yellowBirdCount));
            }
        }

        // ------------------ draw 
        window.clear(sf::Color::Black);

        window.draw(bgSprite);
        window.draw(shipSprite);
        window.draw(waves1);
        window.draw(waves2);

        int k = 0;
        for (k = 0; k < (int)obstacles.size(); k++)
        {
            if (obstacles[k]->isDestroyed() == false)
            {
                window.draw(obstacles[k]->getSprite());
            }
        }

        for (k = 0; k < (int)pigs.size(); k++)
        {
            if (pigs[k]->isAlive() == true)
            {
                window.draw(pigs[k]->getSprite());
            }
        }

        if (powerUpActive == true)
        {
            window.draw(powerUpSprite);
        }

        if (currentBird != 0)
        {
            currentBird->draw(window);
        }

        // HUD icons + counts
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

    clearLevelVectors(pigs, obstacles);

    if (currentBird != 0)
    {
        delete currentBird;
    }

    return 0;
}
