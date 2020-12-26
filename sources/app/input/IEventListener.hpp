
#ifndef FOSS_IEVENTLISTENER_HPP
#define FOSS_IEVENTLISTENER_HPP

class CInputEvent;


class IEventListener
{
public:
    virtual ~IEventListener() = default;
    virtual bool isAcceptable(int type) = 0;
    virtual void accept(const CInputEvent & event) = 0;
};

#endif //FOSS_IEVENTLISTENER_HPP
