#pragma once
#include <chrono>
using Clock = std::chrono::steady_clock;
using mili = std::chrono::milliseconds;
using micro = std::chrono::microseconds;
using nano = std::chrono::nanoseconds;

class Timer {
    using timep_t = typename Clock::time_point;
    timep_t _start = Clock::now(), _end = {};

public:
    void reset() {
        _end = timep_t{};
        _start = Clock::now();
    }

    const Timer& tick() {
        _end = Clock::now();
        return *this;
    }

    auto ms() const { return std::chrono::duration_cast<mili>(_end - _start); }

    auto us() const { return std::chrono::duration_cast<micro>(_end - _start); }

    auto ns() const { return std::chrono::duration_cast<nano>(_end - _start); }
};