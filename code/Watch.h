#pragma once
#include <chrono>
#include <vector>

using namespace std::chrono;

class Watch {
    // Секундомер, может вести единичные замеры
    // или собирать статистику из нескольких интервалов времени
    using clock = high_resolution_clock;
    duration<long, nano> _session;
    unsigned _intervals_count;
    time_point<clock> _start;
    time_point<clock> _stop;

public:
    void startSession() { _session = {}; _intervals_count = 0; }

    void start() { _start = clock::now(); }

    void stop() {
        _stop = clock::now();
        _session += _stop - _start;
        _intervals_count++;
    }

    unsigned int getIntervalsCount() { return _intervals_count; }

    template<class T = milliseconds>
    long double getSessionLength() { return duration_cast<T>(_session).count(); }

    template<class T = milliseconds>
    long double diff() {
        return duration_cast<T>(_stop - _start).count();
    }
};
