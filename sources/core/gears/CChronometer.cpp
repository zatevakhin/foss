
#include "CChronometer.hpp"


CChronometer::CChronometer()
    : mLastTimePoint(std::chrono::system_clock::now())
{
}

double CChronometer::getDelta()
{
    auto t = std::chrono::system_clock::now();
    auto timePassed = std::chrono::duration_cast<std::chrono::milliseconds>(
        t - mLastTimePoint);
    mLastTimePoint = t;
    return 0.001 * double(timePassed.count());
};

void CChronometer::wait(const unsigned long & framePeriod)
{
    auto t = mLastTimePoint + std::chrono::milliseconds(framePeriod);
    std::this_thread::sleep_until(t);
}
