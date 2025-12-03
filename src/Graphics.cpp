#include "Graphics.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>

using namespace std;

// Correct folder paths
static const std::string TEXTURES = "assets/textures/";
static const std::string AUDIO    = "assets/audio/";

void runGraphicsDemo()
{
    cout << "runGraphicsDemo() started" << endl;

    sf::RenderWindow window(
        sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
        "Scrolling Background with Ship"
    );
    window.setFramerateLimit(60);

// error hadling textures 

    bool assetsOk = true;

    sf::Texture bgTexture;
    if (!bgTexture.loadFromFile(TEXTURES + "bg.png"))
    {
        cout << "Error: could not load " << TEXTURES + "bg.png" << endl;
        assetsOk = false;
    }

    sf::Texture shipTexture;
    if (!shipTexture.loadFromFile(TEXTURES + "ship.png"))
    {
        cout << "Error: could not load " << TEXTURES + "ship.png" << endl;
        assetsOk = false;
    }

    sf::Texture wavesTexture;
    if (!wavesTexture.loadFromFile(TEXTURES + "1waves.png"))
    {
        cout << "Error: could not load " << TEXTURES + "1waves.png" << endl;
        assetsOk = false;
    }

    sf::Texture redBirdTexture;
    if (!redBirdTexture.loadFromFile(TEXTURES + "bird_red.png"))
    {
        cout << "Error: could not load " << TEXTURES + "bird_red.png" << endl;
        assetsOk = false;
    }

    sf::Texture blueBirdTexture;
    if (!blueBirdTexture.loadFromFile(TEXTURES + "bird_blue.png"))
    {
        cout << "Error: could not load " << TEXTURES + "bird_blue.png" << endl;
        assetsOk = false;
    }

    sf::Texture yellowBirdTexture;
    if (!yellowBirdTexture.loadFromFile(TEXTURES + "bird_yellow.png"))
    {
        cout << "Error: could not load " << TEXTURES + "bird_yellow.png" << endl;
        assetsOk = false;
    }

    if (!assetsOk)
    {
        cout << "Some textures failed to load. Continuing anyway with empty textures..." << endl;
    }

    // Smooth textures 
    bgTexture.setSmooth(true);
    shipTexture.setSmooth(true);
    wavesTexture.setSmooth(true);
    redBirdTexture.setSmooth(true);
    blueBirdTexture.setSmooth(true);
    yellowBirdTexture.setSmooth(true);

// bg music ----------------------------------------------
    sf::Music music;
    if (!music.openFromFile(AUDIO + "Angry Birds Theme Song.mp3"))
    {
        cout << "Warning: could not load "
             << AUDIO + "Angry Birds Theme Song.mp3" << endl;
    }
    else
    {
        music.setLoop(true);
        music.play();
    }

// bg scroll 

    sf::Sprite bgSprite(bgTexture);
    sf::IntRect bgRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    bgSprite.setTextureRect(bgRect);

    const float BG_SPEED = 200.0f;
    const int BG_MAX_OFFSET = static_cast<int>(bgTexture.getSize().x) - WINDOW_WIDTH;
    float bgOffsetX = 0.0f;

//ship sprite 

    sf::Sprite shipSprite(shipTexture);

    float desiredShipHeight = WINDOW_HEIGHT * 0.3f;
    float shipScale = desiredShipHeight / static_cast<float>(shipTexture.getSize().y);
    shipSprite.setScale(shipScale, shipScale);

    float shipX = WINDOW_WIDTH  * 0.08f;
    float shipY = WINDOW_HEIGHT * 0.65f;
    shipSprite.setPosition(shipX, shipY);

    sf::FloatRect shipBounds = shipSprite.getGlobalBounds();
    float birdDeckX = shipBounds.left + shipBounds.width  * 0.45f;
    float birdDeckY = shipBounds.top  + shipBounds.height * 0.25f;

//waves sprite

    sf::Sprite waves1(wavesTexture);
    sf::Sprite waves2(wavesTexture);

    float wavesWidth  = static_cast<float>(wavesTexture.getSize().x);
    float wavesHeight = static_cast<float>(wavesTexture.getSize().y);

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

//bird sprite

    sf::Sprite redBirdSprite(redBirdTexture);
    sf::Sprite blueBirdSprite(blueBirdTexture);
    sf::Sprite yellowBirdSprite(yellowBirdTexture);

    float desiredBirdHeight = shipBounds.height * 0.25f;
    float redScale    = desiredBirdHeight / static_cast<float>(redBirdTexture.getSize().y);
    float blueScale   = desiredBirdHeight / static_cast<float>(blueBirdTexture.getSize().y);
    float yellowScale = desiredBirdHeight / static_cast<float>(yellowBirdTexture.getSize().y);

    redBirdSprite.setScale(redScale, redScale);
    blueBirdSprite.setScale(blueScale, blueScale);
    yellowBirdSprite.setScale(yellowScale, yellowScale);

    const int NO_BIRD     = 0;
    const int RED_BIRD    = 1;
    const int BLUE_BIRD   = 2;
    const int YELLOW_BIRD = 3;

    int currentBird = NO_BIRD;

    sf::Clock clock;
    bool scrolling = true;

// main loop

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Space)
                {
                    scrolling = !scrolling;
                    if (scrolling) music.play();
                    else music.pause();
                }

                if (event.key.code == sf::Keyboard::Num1) currentBird = RED_BIRD;
                if (event.key.code == sf::Keyboard::Num2) currentBird = BLUE_BIRD;
                if (event.key.code == sf::Keyboard::Num3) currentBird = YELLOW_BIRD;
            }
        }

        float dt = clock.restart().asSeconds();

        // scrolling background
        if (scrolling)
        {
            bgOffsetX += BG_SPEED * dt;
            if (BG_MAX_OFFSET > 0 && bgOffsetX > BG_MAX_OFFSET)
                bgOffsetX -= BG_MAX_OFFSET;

            bgRect.left = static_cast<int>(bgOffsetX);
            bgSprite.setTextureRect(bgRect);

            waves1.move(-WAVES_SPEED * dt, 0.0f);
            waves2.move(-WAVES_SPEED * dt, 0.0f);

            if (waves1.getPosition().x <= -wavesWidthScaled)
                waves1.setPosition(waves2.getPosition().x + wavesWidthScaled, wavesY);

            if (waves2.getPosition().x <= -wavesWidthScaled)
                waves2.setPosition(waves1.getPosition().x + wavesWidthScaled, wavesY);
        }

        // update bird positions on ship
        if (currentBird == RED_BIRD)
            redBirdSprite.setPosition(birdDeckX, birdDeckY);

        if (currentBird == BLUE_BIRD)
            blueBirdSprite.setPosition(birdDeckX, birdDeckY);

        if (currentBird == YELLOW_BIRD)
            yellowBirdSprite.setPosition(birdDeckX, birdDeckY);

        // draw everything
        window.clear(sf::Color::Black);

        window.draw(bgSprite);
        window.draw(shipSprite);

        if (currentBird == RED_BIRD) window.draw(redBirdSprite);
        if (currentBird == BLUE_BIRD) window.draw(blueBirdSprite);
        if (currentBird == YELLOW_BIRD) window.draw(yellowBirdSprite);

        window.draw(waves1);
        window.draw(waves2);

        window.display();
    }
}

