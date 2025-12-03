Compile Command

& "D:\softwares\msys2\mingw64\bin\g++.exe" `
     main1.cpp `
     src\Bird.cpp src\Dev1.cpp src\Graphics.cpp `
     -Iinclude `
     -ID:\softwares\SFML\include `
     -LD:\softwares\SFML\lib `
     -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio `
    -o angry_chiriya.exe

.\angry_chiriya.exe


Currently we are not using graphics.cpp hence we have commented that out in Graphics.hpp. All the implementation, graohic implementation is directly done in main1.cpp.

Game Features:

Different Birds:
pressing 1 = Red Bird
pressing 2 = Blue Bird
pressing 3 = Yellow Bird

Special Abilities:
Red Bird : None.
Blue Bird : When Airborne, if clicked anywhere on the screen, the bird splits into three sub particles which cause less damage then the unsplit blue bird.
Yellow Bird: When Airborne, if clicked anywhere on the screen, the bird speeds up by a certaiun amount. 

Game Reset Button added. 

Different types of pigs have different health. (dies on multiple hits)

different obstacles reduce velocity by a different amount (running the game would show one single wood piece since we are using the same sprite for all three obstacles but implementation works)

5 distinct levels

all the assets have been made by me :) on photoshop

