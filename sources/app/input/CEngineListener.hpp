
#ifndef FOSS_APP_INPUT_CENGINELISTENER_HPP
#define FOSS_APP_INPUT_CENGINELISTENER_HPP

#include "base/input/IEventListener.hpp"

#include <set>


class CEngine;

class CEngineListener : public IEventListener
{
public:
    explicit CEngineListener(CEngine & engine);

    bool isAcceptable(int type) override;
    void accept(const CInputEvent & event) override;

private:
    CEngine & mEngine;
    std::set<int> mAcceptable;

private:
    void onQuit(const CInputEvent & event);
};

#endif //FOSS_APP_INPUT_CENGINELISTENER_HPP
