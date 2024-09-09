#include "display.h"
#include <Camera.hpp>

Display::Display(std::string title, int width, int height)
{
    SDL_WarpMouseInWindow(gGraphicsApplicationWindow, getScreenWidth()/2, getScreenHeight()/2);

    title = title;
    screenWidth = width;
    screenHeight = height;
    gGraphicsApplicationWindow = nullptr;
    gOpenGLContext = nullptr;
    gQuit = false;

    Display::InitializeProgram();
}

void Display::InitializeProgram()
{
    /* Initialize SDL */
    if ( SDL_Init(SDL_INIT_VIDEO) < 0 )
    {
        std::cout << "SDL2 could not initialize video subsystem!" << std::endl;
        exit(1);
    }

    /* Setup OpenGL context */
    /* Use OpenGL 4.1 core or greater */
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    /* We want to request a double buffer for smooth updating */
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    /* Create an application window using OpenGL that supports SDL. */
    gGraphicsApplicationWindow = SDL_CreateWindow(title.c_str(), 30, 30, screenWidth, screenHeight, 
            SDL_WINDOW_OPENGL);

    if ( gGraphicsApplicationWindow == nullptr )
    {
        std::cout << "SDL_Window was not able to be created!" << std::endl;
        exit(1);
    }

    /* Create an OpenGL Graphics Context */
    gOpenGLContext = SDL_GL_CreateContext(gGraphicsApplicationWindow);

    if (  gOpenGLContext == nullptr )
    {
        std::cout << "OpenGL context not available!" << std::endl;
    }

    /* Initialize GLAD library */
    if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
    {
        std::cout << "Glad was not initialized" << std::endl;
        exit(1);
    }
    
    Display::GetOpenGLVersionInfo();
}

void Display::GetOpenGLVersionInfo()
{
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Render: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Shading Language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

void Display::CleanUp()
{
    SDL_DestroyWindow(gGraphicsApplicationWindow);
    SDL_Quit();
}

void Display::Input(Camera* camera)
{
    static int mouseX = getScreenWidth()/2;
    static int mouseY = getScreenHeight()/2;

    SDL_Event e;

    while ( SDL_PollEvent(&e) != 0 )
    {
        if ( e.type == SDL_QUIT )
        {
            std::cout << "Goodbye!" << std::endl;
            gQuit = true;
        }
        
        if (e.type == SDL_MOUSEMOTION)
        {
            mouseX += e.motion.xrel;
            mouseY += e.motion.yrel;
            camera->MouseLook(mouseX, mouseY);
        }
    }

    gRotate += 0.001f;

    // Retrieve keyboard state
    const Uint8 *state = SDL_GetKeyboardState(NULL);

    if (state[SDL_SCANCODE_UP]) {
        camera->MoveForward(speed);
    }

    if (state[SDL_SCANCODE_RIGHT]) {
        camera->MoveRight(speed);
    }

    if (state[SDL_SCANCODE_LEFT]) {
        camera->MoveLeft(speed);
    }

    if (state[SDL_SCANCODE_DOWN]) {
        camera->MoveBackward(speed);
    }
}

std::string Display::getScreenTitle() const
{
    return title;
}

int Display::getScreenHeight() const
{
    return screenHeight;
}

int Display::getScreenWidth() const
{
    return screenWidth;
}

float Display::getGRotate() const {
    return gRotate;
}

float Display::getUOffset() const {
    return uOffset;
}

float Display::getGScale() const {
    return gScale;
}

bool Display::getGQuit() const
{
    return gQuit;
}

SDL_GLContext Display::getOpenGLContext() const
{
    return gOpenGLContext;
}

SDL_Window* Display::getGraphicsApplicationWindow() const
{
    return gGraphicsApplicationWindow;
}