/*
* Copyright (c) 2017 Sprint
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*    http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
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
