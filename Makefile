all:
	g++ -Iinclude -Iinclude/SDL2 -Iinclude/headers -Llib -o Connect4 src/*.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -mwindows