#ifndef __STIME_H
#define __STIME_H

#include <sys/time.h>
#include <string>
#include <stdint.h>

#define TIME_SEP   1
#define DATE_SEP   2
#define MONTH_SEP  4
#define AM_SEP     8
#define PM_SEP     16


#define MON_DAY_YEAR_ORDER  1
#define YEAR_MON_DAY_ORDER  2
#define YEAR_DAY_MON_ORDER  4
#define DAY_MON_YEAR_ORDER  8
#define MON_YEAR_DAY_ORDER  16


#define DATE_FIELDS        7
#define SWAP_POSITION(a,b) {int temp_buf; temp_buf=a; a=b; b=temp_buf;}
#define GIVE_YEAR(year) do { year = year < 30 ? 2000 + year : year < 100 ? 1900 + year : year; } while (0)

#define stringcmp_format(string1, string2, count) __builtin_strncasecmp(string1, string2, count)
#define format_localtime_s(a,b) localtime_r(b,a)
#define format_gmtime_s(a,b) gmtime_r(b,a)
#define format_sprintf_s __builtin_snprintf

#define BASE_YEAR              1900
#define NUM_OF_DAYS_LEAPYEAR   366
#define NUM_OF_DAYS_YEAR       365
#define NUM_OF_DAYS_WEEK       7


#define IF_LEAPYEAR(year) (!((year) % 4) && (((year) % 100) || !((year) % 400)))


#define FORMAT_TIMEZONE timezone

#define FORMAT_DAYLIGHT daylight

size_t format_time_into_string(char *st, size_t max_limit,
		const char* type_format,const struct tm *time_f, const struct timeval* tval);



char *format_time_into_specs(const char *format, const struct tm *t, char *pt, const char *ptlim, const struct timeval* tv);
char *convert_date_time_format(const int32_t n, const char *format, char *pt, const char *ptlim);
char *add_timeformat_to_string(const char *str, char *pt, const char *ptlim);


enum strftime_format {

	NAME_DAY_WEEK_ABB  = 'a',
	FULL_DAY_NAME      = 'A',
	ABBRE_MON_NAME     = 'b',
	FULL_MON_NAME      = 'B',
	DATE_TIME          = 'c',
	CENTURY_NUMBER     = 'C',
	DEC_DATE_MON       = 'd',
	MON_DAY_YEAR       = 'D',
	DAY_AS_DECIMAL_0   = 'e',
	ALTERNATE_ERA      = 'E',
	YEAR_MON_DAY       = 'F',
	TWO_DIG_YEAR       = 'g',
	FOUR_DIG_YEAR      = 'G',
	ABBRE_MON_NAME_2   = 'h',
	HOUR_AS_24_CLK     = 'H',
	HOUR_AS_12_CLK     = 'I',
	DAY_AS_DECIMAL     = 'j',
	HOUR_AS_24_SINGLE  = 'k',
	HOUR_AS_12_SINGLE  = 'l',
	MON_AS_DECIMAL     = 'm',
	MIN_AS_DECIMAL     = 'M',
	NEW_LINE           = 'n',
	MODIFIER           = 'O',
	AM_PM              = 'p',
	am_pm              = 'P',
	TIME_AM_PM         = 'r',
	TIME_HOUR_MIN      = 'R',
	EPOCH_TIME         = 's',
	SECONDS_AS_DEC     = 'S',
	TAB_CHARACTER      = 't',
	TIME_IN_24_HOUR    = 'T',
	DAY_WEEK_AS_DEC    = 'u',
	WEEK_NUM_AS_DEC    = 'U',
	ISO_WEEK_NUM       = 'V',
	DAY_OF_WEEK        = 'w',
	WEEK_NUM_AS_DEC_MON= 'W',
	DATE_WITHOUT_TIME  = 'x',
	TIME_WITHOUT_DATE  = 'X',
	YEAR_WITHOUT_CEN   = 'y',
	YEAR_WIT_CEN       = 'Y',
	HOUR_MIN_OFFSET    = 'z',
	TIMEZONE_NAME      = 'Z',
	DATE_TIME_TZ       = '+',
	LITERAL            = '%',
	RAND_VALUE_1       = 'i',
	RAND_VALUE_2       = 'v',
	MICRO_SEC          = '1',
	MILLI_SEC          = '0',
	NULL_CHAR          = '\0'
};




struct  parse_date
{
	uint32_t field_flags[DATE_FIELDS];
	uint32_t field_parse_flags;
	uint32_t field_values[DATE_FIELDS];
	uint32_t field_count;
};


struct ntp_time_t
{
	uint32_t second;
	uint32_t fraction;
};

class STime
{
public:
    STime()
    {
        *this = Now();
    }

    STime(const STime& a)
    {
        *this = a;
    }

    STime(int32_t sec, int32_t usec)
    {
        m_time.tv_sec = sec;
        m_time.tv_usec = usec;
    }

    STime(int64_t ms)
    {
       set( ms );
    }

    STime(int32_t year, int32_t mon, int32_t day, int32_t hour, int32_t min, int32_t sec, bool isLocal);

    ~STime()
    {
    }

    STime& operator=(const STime& a)
    {
        m_time.tv_sec = a.m_time.tv_sec;
        m_time.tv_usec = a.m_time.tv_usec;

        return *this;
    }

    bool operator==(const STime& a)
    {
        return ((m_time.tv_sec == a.m_time.tv_sec) && (m_time.tv_usec == a.m_time.tv_usec));
    }

    bool operator!=(const STime& a)
    {
        return ((m_time.tv_sec != a.m_time.tv_sec) || (m_time.tv_usec != a.m_time.tv_usec));
    }

    bool operator<(const STime& a)
    {
        if (m_time.tv_sec < a.m_time.tv_sec)
            return true;
        if (m_time.tv_sec == a.m_time.tv_sec)
        {
            if (m_time.tv_usec < a.m_time.tv_usec)
                return true;
        }

        return false;
    }

    bool operator>(const STime& a)
    {
        if (m_time.tv_sec > a.m_time.tv_sec)
            return true;
        if (m_time.tv_sec == a.m_time.tv_sec)
        {
            if (m_time.tv_usec > a.m_time.tv_usec)
                return true;
        }

        return false;
    }

    bool operator>=(const STime& a)
    {
        return !(*this < a);
    }

    bool operator<=(const STime& a)
    {
        return !(*this > a);
    }

    STime operator+(const STime& a)
    {
        return add(a.m_time);
    }

    STime operator+(const timeval& t)
    {
        return add(t);
    }

    STime operator-(const STime& a)
    {
        return subtract(a.m_time);
    }

    STime operator-(const timeval& t)
    {
        return subtract(t);
    }

    STime add(int32_t days, int32_t hrs, int32_t mins, int32_t secs, int32_t usecs)
    {
        timeval t;
        t.tv_sec =
            (days * 86400) +
            (hrs * 3600) +
            (mins * 60) +
            secs;
        t.tv_usec = usecs;

        return add(t);
    }

    STime add(const timeval& t)
    {
        STime nt(*this);

        nt.m_time.tv_sec += t.tv_sec;
        nt.m_time.tv_usec += t.tv_usec;
        if (nt.m_time.tv_usec >= 1000000)
        {
            nt.m_time.tv_usec -= 1000000;
            nt.m_time.tv_sec++;
        }

        return nt;
    }

    STime subtract(int32_t days, int32_t hrs, int32_t mins, int32_t secs, int32_t usecs)
    {
        timeval t;
        t.tv_sec =
            (days * 86400) +
            (hrs * 3600) +
            (mins * 60) +
            secs;
        t.tv_usec = usecs;

        return subtract(t);
    }

    STime subtract(const timeval& t)
    {
        STime nt(*this);

        nt.m_time.tv_sec -= t.tv_sec;
        nt.m_time.tv_usec -= t.tv_usec;
        if (nt.m_time.tv_usec < 0)
        {
            nt.m_time.tv_usec += 1000000;
            nt.m_time.tv_sec--;
        }

        return nt;
    }

    const timeval& getTimeVal()
    {
        return m_time;
    }

    STime &operator=(const timeval &a)
    {
       set(a);
       return *this;
    }

    STime &operator=(int64_t ms)
    {
       set(ms);
       return *this;
    }

    void set(const timeval& a)
    {
        m_time.tv_sec = a.tv_sec;
        m_time.tv_usec = a.tv_usec;
    }

    void set(int64_t ms)
    {
       m_time.tv_sec = ms / 1000;
       m_time.tv_usec = ( ms % 1000 ) * 1000;
    }

    int64_t getCassandraTimestmap()
    {
       return m_time.tv_sec * 1000 + (m_time.tv_usec / 1000);
    }

    void getNTPTime(ntp_time_t &ntp) const;
    void setNTPTime(const ntp_time_t &ntp);

    bool isValid() { return m_time.tv_sec != 0 || m_time.tv_usec != 0; }


//    LongLong operator - (const STime& a)
//    {
//        return ((m_time.tv_sec - a.m_time.tv_sec) * 1000000) + (m_time.tv_usec - a.m_time.tv_usec);
//    }

    int32_t year();
    int32_t month();
    int32_t day();
    int32_t hour();
    int32_t minute();
    int32_t second();

    static STime Now();
    void Format(std::string& dest, const char * fmt, bool local);
    void Format(char * dest, int32_t maxsize, const char * fmt, bool local);
    bool ParseDateTime(const char * pszDate, bool isLocal = true);

private:
    timeval m_time;
};

#endif // #define __STIME_H

