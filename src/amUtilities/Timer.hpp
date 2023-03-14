#pragma once

#include <chrono>

using namespace std::chrono;

#define FORCED_USE_OF_DEPRECATED

template <class T>
class Timer
{
public:
	Timer() = default;

	bool PassedTime(const T duration)
	{
		if (!m_isActive)
		{
			m_isActive = true;
			ResetTime();
			return false;
		}

		if (TimePassedValue(duration))
		{
			m_isActive = false;
			return true;
		}

		return false;
	}

#if defined(USE_DEPRECATED) || defined(FORCED_USE_OF_DEPRECATED)
	bool TimePassedValue(T duration) { return this->GetTimeElaspsed() > duration; }
#endif

	T GetTimeElaspsed()
	{
		return (T)duration_cast<milliseconds>(steady_clock::now() - m_startPoint).count() / (T)1000;
	}
	void ResetTimer()
	{
		m_isActive = false;
		ResetTime();
	}

private:
	void ResetTime() { m_startPoint = steady_clock::now(); }

private:
	steady_clock::time_point m_startPoint = steady_clock::now();
	bool m_isActive = false;
};

typedef Timer<float> FTimer;
typedef Timer<double> DTimer;

#undef FORCED_USE_OF_DEPRECATED