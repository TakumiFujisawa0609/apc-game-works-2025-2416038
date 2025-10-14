#include "Timer.h"

Timer::Timer()
{
    Reset();
}

Timer::~Timer()
{
}

void Timer::Reset()
{
    startTime_ = std::chrono::steady_clock::now();
    pausedTime_ = std::chrono::steady_clock::duration::zero();
    isPaused_ = false;
}

float Timer::GetElapsedSec() const
{
    // ポーズ中かどうかで現在時間を決める
    auto endTime = isPaused_ ? pauseStart_ : std::chrono::steady_clock::now();

    // ポーズしていた時間を引く
    auto elapsed = endTime - startTime_ - pausedTime_;

    return std::chrono::duration_cast<std::chrono::duration<float>>(elapsed).count();
}

bool Timer::IsOver(float seconds) const
{
    return GetElapsedSec() >= seconds;
}

void Timer::Pause()
{
    if (!isPaused_)
    {
        pauseStart_ = std::chrono::steady_clock::now();
        isPaused_ = true;
    }
}

void Timer::Resume()
{
    if (isPaused_)
    {
        // ポーズしてた時間を差し引く
        pausedTime_ += std::chrono::steady_clock::now() - pauseStart_;
        isPaused_ = false;
    }
}
