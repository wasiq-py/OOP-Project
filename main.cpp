#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <optional>    // for std::optional used by pollEvent
using namespace std;

int main()
{
    const int WINDOW_WIDTH  = 1920;
    const int WINDOW_HEIGHT = 1080;

    // SFML 3: VideoMode now takes a Vector2u
    sf::RenderWindow window(
        sf::VideoMode({static_cast<unsigned int>(WINDOW_WIDTH),
                       static_cast<unsigned int>(WINDOW_HEIGHT)}),
        "Scrolling Background with Ship"
    );
    window.setFramerateLimit(60);

    // =====================================================
    // LOAD TEXTURES
    // =====================================================
    sf::Texture bgTexture;
    if (!bgTexture.loadFromFile("bg1.png"))     // 9600 x 1080
    {
        cout << "Error: could not load bg1.png" << endl;
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

    // Bird textures (change filenames if your images are named differently)
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

    bgTexture.setSmooth(true);
    shipTexture.setSmooth(true);
    wavesTexture.setSmooth(true);
    redBirdTexture.setSmooth(true);
    blueBirdTexture.setSmooth(true);
    yellowBirdTexture.setSmooth(true);

    // =====================================================
    // MUSIC (BACKGROUND THEME)
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
    // BACKGROUND SETUP (LOOPING)
    // =====================================================
    sf::Sprite bgSprite1(bgTexture);
    sf::Sprite bgSprite2(bgTexture);

    // Scale background to fit 1366x768-ish look if you want;
    // right now we scale based on original idea you had.
    float bgScaleX = static_cast<float>(1366) / bgTexture.getSize().x;
    float bgScaleY = static_cast<float>(768)  / bgTexture.getSize().y;

    bgSprite1.setScale({bgScaleX, bgScaleY});
    bgSprite2.setScale({bgScaleX, bgScaleY});

    // Effective width of one background after scaling
    float bgWidthScaled = bgTexture.getSize().x * bgScaleX;

    // Place second background immediately after the first
    bgSprite1.setPosition({0.0f, 125.0f});
    bgSprite2.setPosition({bgWidthScaled, 125.0f});

    const float BG_SPEED = 200.0f;

    // =====================================================
    // SHIP SETUP
    // =====================================================
    sf::Sprite shipSprite(shipTexture);

    float desiredShipHeight = WINDOW_HEIGHT * 0.3f;
    float shipScale = desiredShipHeight / static_cast<float>(shipTexture.getSize().y);
    shipSprite.setScale({shipScale, shipScale});

    float shipX = WINDOW_WIDTH * 0.18f;
    float shipY = WINDOW_HEIGHT * 0.5f;
    shipSprite.setPosition({shipX, shipY});

    // Get ship bounds after scaling, to place birds on deck
    sf::FloatRect shipBounds = shipSprite.getGlobalBounds();

    // In SFML 3: FloatRect has position + size instead of left/top/width/height
    float birdDeckX = shipBounds.position.x + shipBounds.size.x * 0.45f;  // middle-ish of ship
    float birdDeckY = shipBounds.position.y + shipBounds.size.y * 0.25f;  // a bit above the center

    // =====================================================
    // WAVES SETUP (BOTTOM, LOOPING)
    // =====================================================
    sf::Sprite waves1(wavesTexture);
    sf::Sprite waves2(wavesTexture);

    float wavesWidth  = static_cast<float>(wavesTexture.getSize().x);
    float wavesHeight = static_cast<float>(wavesTexture.getSize().y);

    // Place the waves so they sit near the bottom
    float wavesY = WINDOW_HEIGHT - wavesHeight - 180.0f;

    waves1.setPosition({0.0f, wavesY});
    waves2.setPosition({wavesWidth, wavesY});

    const float WAVES_SPEED = 300.0f;

    // =====================================================
    // BIRD SPRITES SETUP
    // =====================================================
    sf::Sprite redBirdSprite(redBirdTexture);
    sf::Sprite blueBirdSprite(blueBirdTexture);
    sf::Sprite yellowBirdSprite(yellowBirdTexture);

    // Scale birds so they look nice on the ship
    float desiredBirdHeight = shipBounds.size.y * 0.25f;

    float redScale    = desiredBirdHeight / static_cast<float>(redBirdTexture.getSize().y);
    float blueScale   = desiredBirdHeight / static_cast<float>(blueBirdTexture.getSize().y);
    float yellowScale = desiredBirdHeight / static_cast<float>(yellowBirdTexture.getSize().y);

    redBirdSprite.setScale({redScale, redScale});
    blueBirdSprite.setScale({blueScale, blueScale});
    yellowBirdSprite.setScale({yellowScale, yellowScale});

    // Enumeration to track which bird is currently active
    const int NO_BIRD     = 0;
    const int RED_BIRD    = 1;
    const int BLUE_BIRD   = 2;
    const int YELLOW_BIRD = 3;

    int currentBird = NO_BIRD;

    // =====================================================
    // TIME & SCROLL STATE
    // =====================================================
    sf::Clock clock;
    bool scrolling = true;

    while (window.isOpen())
    {
        // -----------------------------------------------------
        // EVENT LOOP  (SFML 3 style)
        // -----------------------------------------------------
        while (const std::optional<sf::Event> eventOpt = window.pollEvent())
        {
            const sf::Event& event = *eventOpt;

            // Window closed
            if (event.is<sf::Event::Closed>())
            {
                window.close();
            }
            // Key pressed
            else if (const auto* keyEvent = event.getIf<sf::Event::KeyPressed>())
            {
                if (keyEvent->code == sf::Keyboard::Key::Space)
                {
                    // Pause / resume background + waves
                    scrolling = !scrolling;
                }
                else if (keyEvent->code == sf::Keyboard::Key::Num1)
                {
                    // Show red bird on deck
                    currentBird = RED_BIRD;
                }
                else if (keyEvent->code == sf::Keyboard::Key::Num2)
                {
                    // Show blue bird on deck
                    currentBird = BLUE_BIRD;
                }
                else if (keyEvent->code == sf::Keyboard::Key::Num3)
                {
                    // Show yellow bird on deck
                    currentBird = YELLOW_BIRD;
                }
            }
        }

        // -----------------------------------------------------
        // UPDATE LOGIC
        // -----------------------------------------------------
        float dt = clock.restart().asSeconds();

        if (scrolling)
        {
            // Move both backgrounds left
            bgSprite1.move({-BG_SPEED * dt, 0.0f});
            bgSprite2.move({-BG_SPEED * dt, 0.0f});

            // Loop background when it goes off-screen (use scaled width)
            if (bgSprite1.getPosition().x <= -bgWidthScaled)
            {
                bgSprite1.setPosition({bgSprite2.getPosition().x + bgWidthScaled, 125.0f});
            }

            if (bgSprite2.getPosition().x <= -bgWidthScaled)
            {
                bgSprite2.setPosition({bgSprite1.getPosition().x + bgWidthScaled, 125.0f});
            }

            // Move waves (foreground)
            waves1.move({-WAVES_SPEED * dt, 0.0f});
            waves2.move({-WAVES_SPEED * dt, 0.0f});

            // Loop waves
            if (waves1.getPosition().x <= -wavesWidth)
            {
                waves1.setPosition({waves2.getPosition().x + wavesWidth, wavesY});
            }

            if (waves2.getPosition().x <= -wavesWidth)
            {
                waves2.setPosition({waves1.getPosition().x + wavesWidth, wavesY});
            }
        }

        // Keep bird position locked to the deck (even if ship ever moves later)
        if (currentBird == RED_BIRD)
        {
            redBirdSprite.setPosition({birdDeckX, birdDeckY});
        }
        else if (currentBird == BLUE_BIRD)
        {
            blueBirdSprite.setPosition({birdDeckX, birdDeckY});
        }
        else if (currentBird == YELLOW_BIRD)
        {
            yellowBirdSprite.setPosition({birdDeckX, birdDeckY});
        }

        // -----------------------------------------------------
        // DRAW
        // -----------------------------------------------------
        window.clear(sf::Color::Black);

        // Background
        window.draw(bgSprite1);
        window.draw(bgSprite2);

        // Ship
        window.draw(shipSprite);

        // Active bird (if any)
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

        // Waves in front
        window.draw(waves1);
        window.draw(waves2);

        window.display();
    }

    return 0;
}
