#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
using namespace std;

int main()
{
    const int WINDOW_WIDTH  = 1920;
    const int WINDOW_HEIGHT = 1080;

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),"Scrolling Background with Ship");
    window.setFramerateLimit(60);

    // =====================================================
    // texture loading  --------------------------------------------------------------------------------------
    // =====================================================

    // error Checking
    sf::Texture bgTexture;
    if (!bgTexture.loadFromFile("bg.png"))     // 9600 x 1080
    {
        cout << "Error: could not load bg.png" << endl;
        return 1;
    }

    sf::Texture shipTexture;
    if (!shipTexture.loadFromFile("ship.png"))
    {
        cout << "Error: could not load ship.png" << endl;
        return 1;
    }

    sf::Texture wavesTexture;
    if (!wavesTexture.loadFromFile("1waves.png"))
    {
        cout << "Error: could not load 1waves.png" << endl;
        return 1;
    }

    sf::Texture redBirdTexture;
    if (!redBirdTexture.loadFromFile("bird_red.png"))
    {
        cout << "Error: could not load bird_red.png" << endl;
        return 1;
    }

    sf::Texture blueBirdTexture;
    if (!blueBirdTexture.loadFromFile("bird_blue.png"))
    {
        cout << "Error: could not load bird_blue.png" << endl;
        return 1;
    }

    sf::Texture yellowBirdTexture;
    if (!yellowBirdTexture.loadFromFile("bird_yellow.png"))
    {
        cout << "Error: could not load bird_yellow.png" << endl;
        return 1;
    }

    //pixel smoother
    bgTexture.setSmooth(true);
    shipTexture.setSmooth(true);
    wavesTexture.setSmooth(true);
    redBirdTexture.setSmooth(true);
    blueBirdTexture.setSmooth(true);
    yellowBirdTexture.setSmooth(true);

    // =====================================================
    // bg music -----------------------------------------------------------------------------------
    // =====================================================
    
    sf::Music music;
    if (!music.openFromFile("Angry Birds Theme Song.mp3"))
    {
        cout << "Warning: could not load Angry Birds Theme Song.mp3" << endl;
    }
    else
    {
        music.setLoop(true);
        music.play();
    }

    // =====================================================
    // bg scroll setup ---------------------------------------------------------------------------
    // =====================================================
    
    sf::Sprite bgSprite(bgTexture);

    // this rectangle defines the visible 1920x1080 window
    sf::IntRect bgRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    bgSprite.setTextureRect(bgRect);

    // no scaling now – use 1:1 pixels
    // bgSprite.setScale(1.0, 1.0);

    const float BG_SPEED = 200.0;

    // maximum we can shift to the right inside the 9600 texture
    const int BG_MAX_OFFSET = bgTexture.getSize().x - WINDOW_WIDTH;
    float   bgOffsetX     = 0.0;

    // =====================================================
    // ship -------------------------------------------------------------------------------------------
    // =====================================================
    
    sf::Sprite shipSprite(shipTexture);

    float desiredShipHeight = WINDOW_HEIGHT * 0.3;
    float shipScale = desiredShipHeight / (shipTexture.getSize().y);
    shipSprite.setScale(shipScale, shipScale);

    float shipX = WINDOW_WIDTH  * 0.08;
    float shipY = WINDOW_HEIGHT * 0.65;
    shipSprite.setPosition(shipX, shipY);

    sf::FloatRect shipBounds = shipSprite.getGlobalBounds();

    float birdDeckX = shipBounds.left + shipBounds.width  * 0.45;
    float birdDeckY = shipBounds.top  + shipBounds.height * 0.25;

    // =====================================================
    // wave setup loop ---------------------------------------------------------------------------------
    // =====================================================
    
    sf::Sprite waves1(wavesTexture);
    sf::Sprite waves2(wavesTexture);

    // original texture size
    float wavesWidth  = (wavesTexture.getSize().x);
    float wavesHeight = (wavesTexture.getSize().y);

    // make waves bigger
    float wavesScaleX = 2.0;   // scale this is rate 2x
    float wavesScaleY = 1.5;   

    waves1.setScale(wavesScaleX, wavesScaleY);
    waves2.setScale(wavesScaleX, wavesScaleY);

    // size after scaling
    float wavesWidthScaled  = wavesWidth  * wavesScaleX;
    float wavesHeightScaled = wavesHeight * wavesScaleY;

    // place so bottom of scaled waves touches bottom of screen
    float wavesY = WINDOW_HEIGHT - wavesHeightScaled;

    waves1.setPosition(0.0, wavesY);
    waves2.setPosition(wavesWidthScaled, wavesY);

    const float WAVES_SPEED = 300.0;


    // =====================================================
    // birds setup --------------------------------------------------------------------------------------
    // =====================================================
    
    sf:: Sprite redBirdSprite(redBirdTexture);
    sf::Sprite blueBirdSprite(blueBirdTexture);
    sf::Sprite yellowBirdSprite(yellowBirdTexture);

    float desiredBirdHeight = shipBounds.height * 0.25;

    float redScale  = desiredBirdHeight / (redBirdTexture.getSize().y);
    float blueScale = desiredBirdHeight / (blueBirdTexture.getSize().y);
    float yellowScale = desiredBirdHeight / (yellowBirdTexture.getSize().y);

    redBirdSprite.setScale(redScale, redScale);
    blueBirdSprite.setScale(blueScale, blueScale);
    yellowBirdSprite.setScale(yellowScale, yellowScale);

    //keyboard shortcuts
    const int NO_BIRD     = 0;
    const int RED_BIRD    = 1;
    const int BLUE_BIRD   = 2;
    const int YELLOW_BIRD = 3;

    int currentBird = NO_BIRD;

    // =====================================================
    // time, scroll and loop ---------------------------------------------------------------------------
    // =====================================================
    sf::Clock clock;
    bool scrolling = true;

    while (window.isOpen())
    {
// -------------------------------------------------
// loop
// -------------------------------------------------
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Space)
                {
                    // toggle scrolling
                    scrolling = !scrolling;

                    // ALSO toggle music
                    if (scrolling)
                    {
                        music.play();
                    }
                    else
                    {
                        music.pause();
                    }
                }
                else if (event.key.code == sf::Keyboard::Num1)
                {
                    currentBird = RED_BIRD;
                }
                else if (event.key.code == sf::Keyboard::Num2)
                {
                    currentBird = BLUE_BIRD;
                }
                else if (event.key.code == sf::Keyboard::Num3)
                {
                    currentBird = YELLOW_BIRD;
                }
            }
        }

        // -------------------------------------------------
        // update ---------------------------------------------------------------------------------
        // -------------------------------------------------
        float dt = clock.restart().asSeconds();

        if (scrolling)
        {
            // move the background "window" to the right along the big texture
            bgOffsetX += BG_SPEED * dt;

            // loop after reaching the end of the 9600px texture
            if (bgOffsetX > BG_MAX_OFFSET)
            {
                bgOffsetX -= BG_MAX_OFFSET;  // wrap back (or set to 0)
            }

            bgRect.left = (bgOffsetX);
            bgSprite.setTextureRect(bgRect);

            // waves (using 2 sprites for loop)
            waves1.move(-WAVES_SPEED * dt, 0.0);
            waves2.move(-WAVES_SPEED * dt, 0.0);

            if (waves1.getPosition().x <= -wavesWidthScaled)
            {
                waves1.setPosition(waves2.getPosition().x + wavesWidthScaled, wavesY);
            }

            if (waves2.getPosition().x <= -wavesWidthScaled)
            {
                waves2.setPosition(waves1.getPosition().x + wavesWidthScaled, wavesY);
            }

        }

        if (currentBird == RED_BIRD)
        {
            redBirdSprite.setPosition(birdDeckX, birdDeckY);
        }
        else if (currentBird == BLUE_BIRD)
        {
            blueBirdSprite.setPosition(birdDeckX, birdDeckY);
        }
        else if (currentBird == YELLOW_BIRD)
        {
            yellowBirdSprite.setPosition(birdDeckX, birdDeckY);
        }

        // -------------------------------------------------
        // obj draw ------------------------------------------------------------------------------------
        // -------------------------------------------------
        window.clear(sf::Color::Black);

        // single bg sprite using moving textureRect
        window.draw(bgSprite);

        window.draw(shipSprite);

        if (currentBird == RED_BIRD)
        {
            window.draw(redBirdSprite);
        }
        else if (currentBird == BLUE_BIRD)
        {
            window.draw(blueBirdSprite);
        }
        else if (currentBird == YELLOW_BIRD)
        {
            window.draw(yellowBirdSprite);
        }

        window.draw(waves1);
        window.draw(waves2);

        window.display();
    }

    return 0;
}