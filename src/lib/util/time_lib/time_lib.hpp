#pragma once

#include <chrono>


namespace util
{
namespace time
{
    using namespace std::chrono;

    using Clock = system_clock;
    using SteadyClock = steady_clock;

    using Time = system_clock::time_point;
    using SteadyTime = steady_clock::time_point;

    using h = hours;
    using s = seconds;
    using ms = milliseconds;
    using ns = nanoseconds;

    template<typename ClockT, typename PrecT>
    inline double elapsed(typename ClockT::time_point rec)
    {
        return (duration_cast<PrecT>(ClockT::now() - rec).count());
    }
}   // namespace time
}   // namespace util