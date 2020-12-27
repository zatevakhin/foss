#pragma once

#include <memory>


class IWindow
{
public:
    virtual void draw() = 0;
};

using CWindowPtr = std::shared_ptr<IWindow>;
