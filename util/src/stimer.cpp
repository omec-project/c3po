/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

//#include <sys/time.h>
#include <time.h>
#include <signal.h>
#include <string.h>

#include "stimer.h"

STimerElapsed &STimerElapsed::operator = (STimerElapsed &a)
{
    _time = a._time;
    _endtime = a._endtime;
    return  *this;
}

STimerElapsed &STimerElapsed::operator = (stime_t t)
{
    _time = t;
    _endtime = -1;
    return  *this;
}

STimerElapsed::STimerElapsed()
{
    Start();
}

STimerElapsed::STimerElapsed(STimerElapsed &a)
{
    _endtime =  - 1;
    _time = a._time;
}

STimerElapsed::STimerElapsed(stime_t t)
{
    _endtime =  - 1;
    _time = t;
}

STimerElapsed::~STimerElapsed(){}

void STimerElapsed::Start()
{
    struct timespec ts;
    if (clock_gettime(CLOCK_REALTIME, &ts))
        _time = 0;
    else
        _time = (((stime_t)ts.tv_sec) * 1000000000) + ((stime_t)ts.tv_nsec);
    _endtime =  - 1;
}

void STimerElapsed::Stop()
{
    struct timespec ts;
    stime_t t;
    if (clock_gettime(CLOCK_REALTIME, &ts))
        t = 0;
    else
        t = (((stime_t)ts.tv_sec) * 1000000000) + ((stime_t)ts.tv_nsec);

    _endtime = t - _time;
}

void STimerElapsed::Set(stime_t a)
{
    _time = a;
    _endtime =  - 1;
}

stime_t STimerElapsed::MilliSeconds(bool bRestart)
{
    if (_endtime ==  - 1)
    {
        struct timespec ts;
        stime_t t;
        if (clock_gettime(CLOCK_REALTIME, &ts))
            t = 0;
        else
            t = (((stime_t)ts.tv_sec) * 1000000000) + ((stime_t)ts.tv_nsec);

        stime_t r = t - _time;
        if (bRestart)
            _time = t;
        return r / 1000000;
    }

    return _endtime / 1000000;
}

stime_t STimerElapsed::MicroSeconds(bool bRestart)
{
    if (_endtime ==  - 1)
    {
        struct timespec ts;
        stime_t t;
        if (clock_gettime(CLOCK_REALTIME, &ts))
            t = 0;
        else
            t = (((stime_t)ts.tv_sec) * 1000000000) + ((stime_t)ts.tv_nsec);
        stime_t r = t - _time;
        if (bRestart)
            _time = t;
        return r / 1000;
    }

    return _endtime / 1000;
}
