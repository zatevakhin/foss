
/*!
 * @file
 * @brief The file which is contains a \b main window implementation.
 * */

#ifndef FOSS_CORE_WINDOW_CMAINWINDOW_HPP
#define FOSS_CORE_WINDOW_CMAINWINDOW_HPP

#include "core/gears/CChronometer.hpp"

#include "core/auxiliary/opengl.hpp"
#include "core/auxiliary/glm.hpp"
#include "core/auxiliary/sdl.hpp"

#include <memory>


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
    CMainWindow(const std::string& title,
                const glm::ivec2& size,const Uint32& flags);

    /*!
     * @brief Destructor closes SDL2 OpenGL context and delete SDL2 window
     * */
    ~CMainWindow();

    /*!
     * @brief Recreates or create new OpenGL context using setted OpenGL attributes
     * */
    void createGlContext();

    /*!
     * @brief Swap OpenGL window buffers
     * */
    void swapBuffers();

    /*!
     * @brief Returns id of current OpenGL window
     * @return Window id
     * */
    Uint32 getId() const;

    /*!
     * @brief Set SDL2 window attribute value
     * @param attribute - SDL2 attribute
     * @param value - value of attribute
     * */
    static void setGlAttribute(const SDL_GLattr& attribute, const int& value);

private:
    /*!
* @brief The SDL2 window used for OpenGL context creation and buffer switching
     * */
    SDL_Window* mWindow;

    /*!
     * @brief An handle used for access to the OpenGL
     * */
    SDL_GLContext mGlContext;
};

#endif //FOSS_CORE_WINDOW_CMAINWINDOW_HPP
