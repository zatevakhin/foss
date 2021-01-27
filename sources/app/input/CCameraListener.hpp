#pragma once

#include "IEventListener.hpp"
#include "app/scene/CFreeCamera.hpp"
#include <set>

class CCameraListener : public IEventListener
{
public:
    explicit CCameraListener(const TFreeCameraPtr& camera);
    bool isAcceptable(int type) override;
    void accept(const CInputEvent& event) override;

private:
    TFreeCameraPtr mCamera;
    std::set<int> mAcceptable;

private:
    void onMouseMotion(const CInputEvent& event);
    void onKeyDown(const CInputEvent& event);
    void onKeyUp(const CInputEvent& event);

    void onMouseButtonDown(const CInputEvent& event);
    void onMouseButtonUp(const CInputEvent& event);
};
