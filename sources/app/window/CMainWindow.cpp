
#include "CMainWindow.hpp"
#include "app/auxiliary/trace.hpp"

#include <iostream>
#include <vector>


CMainWindow::CMainWindow()
    : mWindow(nullptr)
    , mGlContext(nullptr)
    , mFlags(0)
    , mSize(0)
{
}


CMainWindow::~CMainWindow()
{
    if (nullptr != mGlContext)
    {
        SDL_GL_DeleteContext(mGlContext);
    }

    if (nullptr != mWindow)
    {
        SDL_DestroyWindow(mWindow);
    }

    SDL_Quit();
}


void CMainWindow::setTitle(const std::string& value)
{
    mTitle = value;
}


void CMainWindow::setSize(const glm::ivec2& value)
{
    mSize = value;
}


void CMainWindow::setFlags(const Uint32& value)
{
    mFlags = value;
}


void CMainWindow::create()
{
    if (mAttributes.empty())
    {
        spdlog::error("CMainWindow::create : can`t create window, attributes is not set.");
        return;
    }

    SDL_Init(SDL_INIT_VIDEO);

    for (const auto& item : mAttributes)
    {
        SDL_GL_SetAttribute(item.first, item.second);
    }

    mWindow =
        SDL_CreateWindow(mTitle.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, mSize.x, mSize.y, mFlags);

    // mWindow

    // TODO: if window was resized, create new gl context.
    mGlContext = SDL_GL_CreateContext(mWindow);

    if (nullptr == mGlContext)
    {
        throw std::runtime_error("Failed to create gl context!");
    }

    spdlog::debug("glGetString (GL_VERSION) returns {}", glGetString(GL_VERSION));
}

void CMainWindow::swapBuffers()
{
    SDL_GL_SwapWindow(mWindow);
}


void CMainWindow::setAttributes(std::map<SDL_GLattr, int>& attributes)
{
    mAttributes = attributes;
}


Uint32 CMainWindow::getId() const
{
    return SDL_GetWindowID(mWindow);
}


const glm::ivec2& CMainWindow::getSize() const
{
    return mSize;
}
