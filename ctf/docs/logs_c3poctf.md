---
# C3POCTF Log Files
---

The following sections describe the purpose and location of the log
files to which this component writes, as well messages that can appear
in those logs. Use this information to determine status and processing
as part of testing and troubleshooting.

## Configure the Logs

Today, C3POCTF does not implement a log mechanism based on level.
C3POCTF is logging statistic metrics, configurable by frequency.

The following steps show how to configure the frequency of statistics
generation

1. Open the json configuration file c3poctf/conf/ctf.json.

2. Set or modify the value for the key “statsfreq” (The value is in
milliseconds).

3. Restart the application.

This component writes to 1 log files.

**syslog**

Location: /var/log/syslog

This log file tracks the number of ACR messages sent to CDF for a given
period, the number of messages received from CDF for a given period, the
average lapse of time between the time the ACR is sent and the
corresponding ACA is received for a given period, the timespan
corresponding to the log generation in UTC, and the total time in seconds
that C3POCTF is up.

Typical messages that appear include:

{"msg\_handle\_avg":"2","nbacr\_received":"150","nbacr\_sent":"150","time\_utc":"2017-10-13
13:18:16.722”}

{"msg\_handle\_avg":"2","nbacr\_received":"150","nbacr\_sent":"150","time\_utc":"2017-10-13
13:18:16.722”}

{"msg\_handle\_avg":"2","nbacr\_received":"150","nbacr\_sent":"150","time\_utc":"2017-10-13
13:18:16.722”}

{"time\_utc":"2017-10-13 13:18:17.911","uptime":"11"}

"msg\_handle\_avg":"2" The average lapse of time between the time the
ACR is sent and the corresponding ACA is received for a given period

"nbacr\_received":"150" Number of ACR messages received from CDF for a
given period

"nbacr\_sent":"150" Number of ACR messages sent to CDF for a given
period

"time\_utc":"2017-10-13 13:18:16.722" the timespan corresponding to the
log generation in UTC

"uptime":"11" he total time in seconds that C3POCTF is up in seconds
