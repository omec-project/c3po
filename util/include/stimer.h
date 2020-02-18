/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef __STIMER_H
#define __STIMER_H

typedef long long int stime_t;


class STimerElapsed
{
public:
    STimerElapsed();
    STimerElapsed(STimerElapsed &a);
    STimerElapsed(stime_t t);
    ~STimerElapsed();

    void Start();
    void Stop();
    void Set(stime_t a);
    stime_t MilliSeconds(bool bRestart = false);
    stime_t MicroSeconds(bool bRestart = false);

    STimerElapsed &operator = (STimerElapsed &a);
    STimerElapsed &operator = (stime_t t);

    operator stime_t() { return _time; }

private:
    stime_t _time;
    stime_t _endtime;
};

#endif // #define __STIMER_H
