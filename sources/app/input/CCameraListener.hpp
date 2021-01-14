#pragma once

#include "IEventListener.hpp"

#include <set>


class CCamera;

class CCameraListener : public IEventListener
{
public:
    explicit CCameraListener(CCamera* camera);
    bool isAcceptable(int type) override;
    void accept(const CInputEvent& event) override;

private:
    CCamera* mCamera;
    std::set<int> mAcceptable;

private:
    void onMouseMotion(const CInputEvent& event);
    void onKeyDown(const CInputEvent& event);
    void onKeyUp(const CInputEvent& event);

    void onMouseButtonDown(const CInputEvent& event);
    void onMouseButtonUp(const CInputEvent& event);
};
