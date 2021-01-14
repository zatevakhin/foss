#pragma once

class CInputEvent;

class IEventListener
{
public:
    virtual ~IEventListener() = default;
    virtual bool isAcceptable(int type) = 0;
    virtual void accept(const CInputEvent& event) = 0;
};
