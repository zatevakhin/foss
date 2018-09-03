
/*!
 * @file
 * @brief File which is contains chronometer implementation.
 * */

#ifndef FOSS_CORE_GEARS_CCHRONOMETER_HPP
#define FOSS_CORE_GEARS_CCHRONOMETER_HPP

#include <chrono>
#include <thread>


/*!
 * @brief Class which is implements chronometer
 *
 * used to account for the time between frames switching and stop execution of
 * the current thread if the time delta is less that \b framePeriod.
 * */
class CChronometer
{
public:
    /*!
     * @brief The default time behind which must be executed frame
     *
     * this is a frame length in milliseconds.
     * */
    constexpr static unsigned long DEFAULT_FRAME_PERIOD = 16UL;

public:

    /*!
     * @brief Constructor initialize the \b mLastTimePoint with last point of time.
     * */
    explicit CChronometer();

    /*!
     * @brief Returns diff betwen current point of time and previous.
     * @return time delta.
     *
     * And setting up current time as last point of time.
     * */
    double getDelta();

    /*!
     * @brief Stopping current thread if the passed time is less than \b framePeriod.
     * @param framePeriod - frame length in milliseconds.
     * */
    void wait(const long& framePeriod = DEFAULT_FRAME_PERIOD);

private:
    /*!
     * @brief Contains stored previous point of time.
     * */
    std::chrono::system_clock::time_point mLastTimePoint;

};

#endif //FOSS_CORE_GEARS_CCHRONOMETER_HPP
