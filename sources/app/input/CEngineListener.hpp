#pragma once


#include "IEventListener.hpp"

#include <set>


class CEngine;

class CEngineListener : public IEventListener
{
public:
    explicit CEngineListener(CEngine& engine);

    bool isAcceptable(int type) override;
    void accept(const CInputEvent& event) override;

private:
    CEngine& mEngine;
    std::set<int> mAcceptable;

private:
    void onQuit(const CInputEvent& event);
};
