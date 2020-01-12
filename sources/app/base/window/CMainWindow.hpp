
/*!
 * @file
 * @brief The file which is contains a \b main window implementation.
 * */

#ifndef FOSS_CORE_WINDOW_CMAINWINDOW_HPP
#define FOSS_CORE_WINDOW_CMAINWINDOW_HPP

#include "app/base/gears/CChronometer.hpp"

#include "app/base/auxiliary/opengl.hpp"
#include "app/base/auxiliary/glm.hpp"
#include "app/base/auxiliary/sdl.hpp"

#include <memory>
#include <map>


/*!
 * @brief Class which is implementing \b main window
 * */
class CMainWindow
{

public:
    /**
     * @brief Constructor which is create \b SDL2 window
     * @param title - Window title
     * @param size - Window size
     * @param flags - Window flags which is specify window type
     * */
    CMainWindow();

    /*!
     * @brief Destructor closes SDL2 OpenGL context and delete SDL2 window
     * */
    ~CMainWindow();

    void setTitle(const std::string & value);

    void setSize(const glm::ivec2 & value);

    void setFlags(const Uint32 & value);

    void create();

    /*!
     * @brief Swap OpenGL window buffers
     * */
    void swapBuffers();

    /*!
     * @brief Returns id of current OpenGL window
     * @return Window id
     * */
    Uint32 getId() const;

    const glm::ivec2 & getSize() const;

    /*!
     * @brief Set SDL2 window attribute value
     * @param attribute - SDL2 attribute
     * @param value - value of attribute
     * */
    void setAttributes(std::map<SDL_GLattr, int> & attributes);

private:
    /*!
* @brief The SDL2 window used for OpenGL context creation and buffer switching
     * */
    SDL_Window* mWindow;

    /*!
     * @brief An handle used for access to the OpenGL
     * */
    SDL_GLContext mGlContext;

    std::map<SDL_GLattr, int> mAttributes;

    std::string mTitle;

    glm::ivec2 mSize;

    Uint32 mFlags;
};

#endif //FOSS_CORE_WINDOW_CMAINWINDOW_HPP
