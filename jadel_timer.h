#pragma once
#include "jadel_defs.h"
#include <chrono>

typedef std::chrono::steady_clock::time_point TimePoint;

namespace jadel
{

struct Timer
{
    TimePoint startTime;
    TimePoint lastTime;

    inline void start()
	{
		startTime = std::chrono::high_resolution_clock::now();
        lastTime = startTime;
	}

	inline uint32 getMillisSinceStart() const
	{
        TimePoint currentTime = std::chrono::high_resolution_clock::now();

        uint32 result =  (uint32)std::chrono::duration_cast<std::chrono::milliseconds>
            (currentTime - startTime).count();
        return result;
	}
    
	inline uint32 getMillisSinceLastCheck()
	{
        TimePoint currentTime = std::chrono::high_resolution_clock::now();
		uint32 result =  (uint32)std::chrono::duration_cast<std::chrono::milliseconds>
            (currentTime - lastTime).count();
            lastTime = currentTime;
        return result;
    }
};
}