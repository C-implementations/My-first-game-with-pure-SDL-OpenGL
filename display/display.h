#ifndef DISPLAY_H
#define DISPLAY_H

// Third party libraries
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <string>

// C++ standard template library (STL)
#include <iostream>

class Display
{
    private:
        std::string title;
        int screenHeight;
        int screenWidth;

        float uOffset = -2.0f;
        float gRotate = 0.0f;
        float gScale = 0.75f;
        
        bool gQuit;
        
        SDL_Window* gGraphicsApplicationWindow;
        SDL_GLContext gOpenGLContext;

    public:
        Display(std::string title, int width, int height);

        void GetOpenGLVersionInfo();
        void InitializeProgram();
        void CleanUp();
        void Input();

        std::string getScreenTitle() const;
        int getScreenHeight() const;
        int getScreenWidth() const;
        float getUOffset() const;
        float getGRotate() const;
        float getGScale() const;
        bool getGQuit() const;
        SDL_GLContext getOpenGLContext() const;
        SDL_Window* getGraphicsApplicationWindow() const;
};

#endif