#pragma once
#include <chrono>

class Timer
{
public:
    Timer();
    ~Timer();

    // タイマーをリセット
    void Reset();

    // 経過秒を返す
    float GetElapsedSec() const;

    // 一定時間経過したか
    bool IsOver(float seconds) const;

    // ポーズ
    void Pause();
    void Resume();

private:

    std::chrono::steady_clock::time_point startTime_;
    std::chrono::steady_clock::time_point pauseStart_;  // ポーズ開始時刻
    std::chrono::steady_clock::duration pausedTime_;    // 累積ポーズ時間

    bool isPaused_ = false;
};