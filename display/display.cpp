#include "display.h"

Display::Display(std::string title, int width, int height)
{
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

void Display::Input()
{
    SDL_Event e;

    while ( SDL_PollEvent(&e) != 0 )
    {
        if ( e.type == SDL_QUIT )
        {
            std::cout << "Goodbye!" << std::endl;
            gQuit = true;
        }
    }

    // Retrieve keyboard state
    const Uint8 *state = SDL_GetKeyboardState(NULL);

    if (state[SDL_SCANCODE_UP]) {
        uOffset += 0.001f;
        std::cout << uOffset << std::endl;
    }

    if (state[SDL_SCANCODE_DOWN]) {
        uOffset -= 0.001f;
        std::cout << uOffset << std::endl;
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

float Display::getUOffset() const {
    return uOffset;
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