#pragma once

#include <chrono>


namespace util
{
namespace time
{
    using namespace std::chrono;

    using Time = system_clock::time_point;
    using SteadyTime = steady_clock::time_point;

    using h = hours;
    using s = seconds;
    using ms = milliseconds;
    using ns = nanoseconds;

    template<typename StruT>
    inline double now()
    {
        return duration_cast<duration<double>>(StruT::now()).count();
    }

    template<typename StruT, typename PrecT>
    inline double elapsed(StruT rec)
    {
        return (duration_cast<PrecT>(StruT::now() - rec).count());
    }
}   // namespace time
}   // namespace util