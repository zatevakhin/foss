
#include "CMainWindow.hpp"

#include <iostream>


CMainWindow::CMainWindow(const std::string& title, const glm::ivec2& size, const Uint32& flags)
    : mWindow(nullptr)
    , mGlContext(nullptr)
{
    SDL_Init(SDL_INIT_VIDEO);
    mWindow = SDL_CreateWindow(title.c_str(), 0, 0, size.x, size.y, flags);
}


CMainWindow::~CMainWindow()
{
    SDL_GL_DeleteContext(mGlContext);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}


void CMainWindow::swapBuffers()
{
    SDL_GL_SwapWindow(mWindow);
}


void CMainWindow::setGlAttribute(const SDL_GLattr& attribute, const int& value)
{
    SDL_GL_SetAttribute(attribute, value);
}


void CMainWindow::createGlContext()
{
    if (nullptr != mGlContext)
    {
        SDL_GL_DeleteContext(mGlContext);
    }

    mGlContext = SDL_GL_CreateContext(mWindow);
}


Uint32 CMainWindow::getId() const
{
    return SDL_GetWindowID(mWindow);
}



