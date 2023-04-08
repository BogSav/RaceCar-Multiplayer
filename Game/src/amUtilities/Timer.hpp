#pragma once

#include <chrono>

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

	T GetTimeElaspsed()
	{
		return (T)std::chrono::duration_cast<std::chrono::milliseconds>(
				   std::chrono::steady_clock::now() - m_startPoint)
				   .count()
			/ (T)1000;
	}
	void ResetTimer()
	{
		m_isActive = false;
		ResetTime();
	}

private:
	void ResetTime() { m_startPoint = std::chrono::steady_clock::now(); }

#if defined(USE_DEPRECATED) || defined(FORCED_USE_OF_DEPRECATED)
	bool TimePassedValue(T duration) { return this->GetTimeElaspsed() > duration; }
#endif

private:
	std::chrono::steady_clock::time_point m_startPoint = std::chrono::steady_clock::now();
	bool m_isActive = false;
};

typedef Timer<float> FTimer;
typedef Timer<double> DTimer;

class ForcedFramerateTimer
{
public:
	ForcedFramerateTimer(const float& framesPerSecond = 0) {}

	void SuspendThreadToSyncFramerate()
	{
		std::chrono::time_point<std::chrono::high_resolution_clock> currentTime =
			std::chrono::high_resolution_clock::now();

		std::chrono::nanoseconds elapsedTime =
			std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - lastFrameTime);

		if (elapsedTime < frameTime)
		{
			std::this_thread::sleep_for(std::chrono::nanoseconds{16'666'667});
		}

		lastFrameTime = currentTime;
	}

	void SuspendThreadToSyncFramerate(float& deltaTime)
	{
		std::chrono::nanoseconds elapsedTime{static_cast<long long>(deltaTime * 1e+9)};

		if (elapsedTime < frameTime)
		{
			std::this_thread::sleep_for(frameTime - elapsedTime);
		}
	}

private:
	std::chrono::nanoseconds frameTime{166'666'667};
	std::chrono::time_point<std::chrono::high_resolution_clock> lastFrameTime =
		std::chrono::high_resolution_clock::now();
};

#undef FORCED_USE_OF_DEPRECATED