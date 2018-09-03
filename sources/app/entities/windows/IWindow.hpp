#ifndef FOSS_APP_ENTITIES_IWINDOW_HPP
#define FOSS_APP_ENTITIES_IWINDOW_HPP

#include <memory>


class IWindow
{
public:
    virtual void draw() = 0;
};

using CWindowPtr = std::shared_ptr<IWindow>;


#endif //FOSS_APP_ENTITIES_IWINDOW_HPP