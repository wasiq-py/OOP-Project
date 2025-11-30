Compile Command

& "D:\softwares\msys2\mingw64\bin\g++.exe" main.cpp Bird.cpp Dev1.cpp Graphics.cpp `
 -ID:\softwares\SFML\include `
 -LD:\softwares\SFML\lib `
 -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio `
 -o angry_chiriya.exe


(You can omit Graphics.cpp if you’re not using runGraphicsDemo() anywhere.)