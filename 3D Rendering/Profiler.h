#ifndef PROFILER_H
#define PROFILER_H

//#ifdef _DEBUG
#define DEBUG_BUILD
//#endif

#include <deque>
#include <map>
#include <ctime>
#include <string>

#define CONCAT(a, b) a ## b
#define UNIQUE_VAR(tag) CONCAT(tag, __LINE__)

#ifdef DEBUG_BUILD
#define DEBUG_ONLY(what) what
#else
#define DEBUG_ONLY(what) do { } while (0)
#endif
 
#define PROFILE(name) DEBUG_ONLY(Profiler UNIQUE_VAR(profile) (name))
#define PROFILE_FUNC DEBUG_ONLY(Profiler UNIQUE_VAR(profile) (__FUNSIG__))
#define PROFILE_MAIN_LOOP DEBUG_ONLY(Profiler::tick(); Profiler UNIQUE_VAR(profile) ("MAIN LOOP"))
#define PROFILE_DUMP(stream) DEBUG_ONLY(Profiler::dump(stream))

#ifdef DEBUG_BUILD

class Profiler
{
protected:
	static std::map<std::string, double> frameTimes, minTimes, maxTimes, totalTimes;
	static double frameCount;
	std::string name;

	clock_t startTime, totalTime;

public:
	Profiler(std::string entry)
	{
		name = entry;
		startTime = clock();
	}
	~Profiler()
	{
		totalTime = clock() - startTime;

		frameTimes[name] += totalTime;
	}

	static void dump(std::ostream &s)
	{
		double time = 0;
		s << "**********************" << std::endl;
		s << "Profile Log:" << std::endl;
		for (auto it = totalTimes.begin(); it != totalTimes.end(); ++it)
		{
			s << it->first << std::endl;
			s << "\tAverage time: " << it->second / frameCount / (CLOCKS_PER_SEC / 1000) << " milliseconds" << std::endl;
			if (it->first == "MAIN LOOP")
			{
				time = (it->second / frameCount);
			}
		}
		s << "**********************" << std::endl;
		s << "Milliseconds per Frame: " << time / (CLOCKS_PER_SEC / 1000) << " FPS: " << CLOCKS_PER_SEC / time << std::endl;
		s << "**********************" << std::endl;
	}

	static void tick()
	{
		++frameCount;
		for (auto &it = frameTimes.begin(); it != frameTimes.end(); ++it)
		{
			totalTimes[it->first] += it->second;
			it->second = 0;
		}
	}
};

#endif

#endif