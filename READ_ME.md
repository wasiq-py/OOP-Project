Compile Command

& "D:\softwares\msys2\mingw64\bin\g++.exe" `
    main.cpp `
    src\Bird.cpp `
    src\Dev1.cpp `
    src\Graphics.cpp `
    -Iinclude `
    -ID:\softwares\SFML\include `
    -LD:\softwares\SFML\lib `
    -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio `
    -o angry_chiriya.exe


(You can omit Graphics.cpp if you’re not using runGraphicsDemo() anywhere.)