all:
	g++ -I glad/include -I src/include -L src/lib -o main main.cpp glad.c -l mingw32 -l SDL2main -l SDL2