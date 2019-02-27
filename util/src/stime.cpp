#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include "stime.h"


static const char *g_day_names[] = {
	"Sunday",
	"Monday",
	"Tuesday",
	"Wednesday",
	"Thursday",
	"Friday",
	"Saturday"
};

static const char *g_day_names_short[] = {
	"Sun",
	"Mon",
	"Tue",
	"Wed",
	"Thu",
	"Fri",
	"Sat"
};

static const char *g_mnth_names[] = {
	"January",
	"February",
	"March",
	"April",
	"May",
	"June",
	"July",
	"August",
	"September",
	"October",
	"November",
	"December"
};

static const char *g_mnth_names_short[] = {
	"Jan",
	"Feb",
	"Mar",
	"Apr",
	"May",
	"Jun",
	"Jul",
	"Aug",
	"Sep",
	"Oct",
	"Nov",
	"Dec"
};

void cal_year_week(int32_t *year, int32_t *w, const struct tm *t)
{

	int32_t  yday;
	int32_t  wday;

	*year = t->tm_year + BASE_YEAR;
	yday = t->tm_yday;
	wday = t->tm_wday;

	while (1) {

		int32_t  length;
		int32_t  bottom;
		int32_t  top;

		if(IF_LEAPYEAR(*year))
			length = NUM_OF_DAYS_LEAPYEAR;
		else
			length = NUM_OF_DAYS_YEAR;


		bottom = ((yday + 11 - wday) % NUM_OF_DAYS_WEEK) - 3;
		top = bottom - (length % NUM_OF_DAYS_WEEK);

		if (top < -3) top += NUM_OF_DAYS_WEEK;

		top += length;
		if (yday >= top) {
			++(*year);
			*w = 1;
			break;
		}

		if (yday >= bottom) {
			*w = 1 + ((yday - bottom) / NUM_OF_DAYS_WEEK);
			break;
		}

		--*year;
		yday += IF_LEAPYEAR(*year) ? NUM_OF_DAYS_LEAPYEAR : NUM_OF_DAYS_YEAR;
	}

}


size_t format_time_into_string(char *st, size_t max_limit, const char* type_format,
			struct tm *time_f, const struct timeval* tval)
{
	char *ptr = NULL;

	if (NULL == type_format) {
		ptr = format_time_into_specs("%c", time_f, (char *)st, st + max_limit, tval);
	} else {
		ptr = format_time_into_specs(type_format,time_f,st,st + max_limit, tval);
	}

	if (ptr == (st + max_limit))
		return 0;

	*ptr = '\0';

	return ptr-st;
}

char *
format_time_into_specs(const char *time_format, const struct tm *t, char *dest,
		const char *max_limit, const struct timeval* p_timeval)
{
	int32_t tim_sec = 0;
	int i = 0;

	while(time_format[i] != '\0') {

		if (time_format[i] == LITERAL) {

			if (time_format[i] == ALTERNATE_ERA)
				i++;
			else if (time_format[i] == MODIFIER)
				i++;

		char temp = time_format[i +1];
		switch (temp) {

			case FULL_DAY_NAME:
				if ((t->tm_wday < 0 || t->tm_wday > 6))
					dest = add_timeformat_to_string("?", dest, max_limit);
				else
					dest = add_timeformat_to_string(g_day_names[t->tm_wday], dest, max_limit);

				i++;
				continue;

			case NAME_DAY_WEEK_ABB:
				if ((t->tm_wday < 0 || t->tm_wday > 6))
				    dest = add_timeformat_to_string("?", dest, max_limit);
				else
					dest = add_timeformat_to_string( g_day_names_short[t->tm_wday], dest, max_limit);

				i++;
				continue;


			case NULL_CHAR:
				 --i;
				break;

			case MILLI_SEC:
				tim_sec = p_timeval->tv_usec /1000;
				dest = convert_date_time_format(tim_sec, "%03d", dest, max_limit);
				i++;
				continue;

			case MICRO_SEC:
				tim_sec = p_timeval->tv_usec;
				dest = convert_date_time_format(tim_sec, "%06d", dest, max_limit);
				i++;
				continue;

			case FULL_MON_NAME:
				if ((t->tm_mon < 0 || t->tm_mon > 11))
					dest = add_timeformat_to_string("?", dest, max_limit);
				else
					dest = add_timeformat_to_string(g_mnth_names[t->tm_mon], dest, max_limit);

				i++;
				continue;

			case ABBRE_MON_NAME:
				/*fallback to next case*/

			case ABBRE_MON_NAME_2:

				if(t->tm_mon < 0 || t->tm_mon > 11)
					dest = add_timeformat_to_string("?", dest, max_limit);
				else
					dest = add_timeformat_to_string(g_mnth_names_short[t->tm_mon], dest, max_limit);

				i++;
				continue;

			case DATE_TIME:

				dest = format_time_into_specs("%a %b %e %H:%M:%S %Y", t, dest, max_limit, p_timeval);
				i++;
				continue;

			case MON_DAY_YEAR:
				dest = format_time_into_specs("%m/%d/%y", t, dest, max_limit, p_timeval);
				i++;
				continue;

			case CENTURY_NUMBER:
				tim_sec = (t->tm_year + BASE_YEAR) / 100 ;
				dest = convert_date_time_format(tim_sec, "%02d", dest, max_limit);
				i++;
				continue;


			case DEC_DATE_MON:
				dest = convert_date_time_format(t->tm_mday, "%02d", dest, max_limit);
				i++;
				continue;

			case YEAR_MON_DAY:
				dest = format_time_into_specs("%Y-%m-%d", t, dest, max_limit, p_timeval);
				i++;
				continue;

			case DAY_AS_DECIMAL_0:
				dest = convert_date_time_format(t->tm_mday, "%2d", dest, max_limit);
				i++;
				continue;

			case HOUR_AS_24_CLK:
				dest = convert_date_time_format(t->tm_hour, "%02d", dest, max_limit);
				i++;
				continue;

			case RAND_VALUE_1:
				dest = format_time_into_specs("%Y-%m-%dT%H:%M:%S.%0", t, dest, max_limit, p_timeval);
				i++;
				continue;

			case HOUR_AS_12_CLK:

				if((t->tm_hour % 12) == 0)
					dest = convert_date_time_format(12, "%02d", dest, max_limit);
				else
					dest = convert_date_time_format((t->tm_hour % 12), "%02d", dest, max_limit);

				i++;
				continue;

			case HOUR_AS_24_SINGLE:
				dest = convert_date_time_format(t->tm_hour, "%2d", dest, max_limit);
				i++;
				continue;

			case HOUR_AS_12_SINGLE:
				dest = convert_date_time_format((t->tm_hour % 12) ? (t->tm_hour % 12) : 12, "%2d", dest, max_limit);
				i++;
				continue;

			case DAY_AS_DECIMAL:
				dest = convert_date_time_format(t->tm_yday + 1, "%03d", dest, max_limit);
				i++;
				continue;

			case MIN_AS_DECIMAL:
				dest = convert_date_time_format(t->tm_min, "%02d", dest, max_limit);
				i++;
				continue;

			case MON_AS_DECIMAL:
				dest = convert_date_time_format(t->tm_mon + 1, "%02d", dest, max_limit);
				i++;
				continue;

			case AM_PM:
				if ((t->tm_hour >= 12))
					dest = add_timeformat_to_string("pm", dest, max_limit);
				else
					dest = add_timeformat_to_string("am", dest, max_limit);

				i++;
				continue;

			case NEW_LINE:
				dest = add_timeformat_to_string("\n", dest, max_limit);

				i++;
				continue;

			case TIME_HOUR_MIN:
				dest = format_time_into_specs("%H:%M", t, dest, max_limit, p_timeval);
				i++;
				continue;

			case TIME_AM_PM:
				dest = format_time_into_specs("%I:%M:%S %p", t, dest, max_limit, p_timeval);

				i++;
				continue;

			case SECONDS_AS_DEC:
				 dest = convert_date_time_format(t->tm_sec, "%02d", dest, max_limit);

				 i++;
				 continue;

			case EPOCH_TIME:
				{

					struct tm tm_val = *t;
					char buffer[32] = {0};
					format_sprintf_s(buffer, sizeof(buffer), "%lu", mktime(&tm_val));
					dest = add_timeformat_to_string(buffer, dest, max_limit);

					i++;
					continue;
				}

			case TAB_CHARACTER:
					dest = add_timeformat_to_string("\t", dest, max_limit);
					i++;
					continue;

			case TIME_IN_24_HOUR:
					dest = format_time_into_specs("%H:%M:%S", t, dest, max_limit, p_timeval);
					i++;
					continue;

			case DAY_WEEK_AS_DEC:
					if(t->tm_wday == 0)
						dest = convert_date_time_format(7, "%d", dest, max_limit);
					else
						dest = convert_date_time_format(t->tm_wday, "%d", dest, max_limit);
					i++;
					continue;

			case WEEK_NUM_AS_DEC:
					tim_sec = ((t->tm_yday + 7) - (t->tm_wday)) / 7;
					dest = convert_date_time_format(tim_sec, "%02d", dest, max_limit);
					i++;
					continue;


			case ISO_WEEK_NUM:
			case FOUR_DIG_YEAR:
			case TWO_DIG_YEAR :
				{

					int32_t yr;
					int32_t wk;
					cal_year_week(&yr, &wk, t);
					if (time_format[i] == ISO_WEEK_NUM)
						dest = convert_date_time_format(wk, "%02d", dest, max_limit);
					else if (time_format[i] == TWO_DIG_YEAR)
						dest = convert_date_time_format(yr % 100, "%02d", dest, max_limit);
					else
						dest = convert_date_time_format(yr, "%04d", dest, max_limit);

					i++;
					continue;
				}

			case RAND_VALUE_2:

				dest = format_time_into_specs("%e-%b-%Y", t, dest, max_limit, p_timeval);
				i++;
				continue;

			case DAY_OF_WEEK:
				dest = convert_date_time_format(t->tm_wday, "%d", dest, max_limit);
				i++;
				continue;

			case WEEK_NUM_AS_DEC_MON:

				dest = convert_date_time_format((t->tm_yday + 7 - (t->tm_wday ? (t->tm_wday - 1) : 6)) / 7, "%02d", dest, max_limit);
				i++;
				continue;

			case DATE_WITHOUT_TIME:
				dest = format_time_into_specs("%m/%d/%y", t, dest, max_limit, p_timeval);
				i++;
				continue;

			case TIME_WITHOUT_DATE:
				dest = format_time_into_specs("%H:%M:%S", t, dest, max_limit, p_timeval);
				i++;
				continue;

			case YEAR_WIT_CEN:
				tim_sec = t->tm_year + BASE_YEAR;
				dest = convert_date_time_format(tim_sec, "%04d", dest, max_limit);
				i++;
				continue;

			case YEAR_WITHOUT_CEN:
				tim_sec = (t->tm_year + BASE_YEAR) % 100;
				dest = convert_date_time_format(tim_sec, "%02d", dest, max_limit);
				i++;
				continue;

			case TIMEZONE_NAME:
				dest = add_timeformat_to_string("?", dest, max_limit);
				i++;
				continue;

			case HOUR_MIN_OFFSET:
				{

					long temp_var = 0;

					((temp_var = FORMAT_TIMEZONE)  >= 0) ? (dest = add_timeformat_to_string("+", dest, max_limit))
												 : (dest = add_timeformat_to_string("-", dest, max_limit));

					dest = convert_date_time_format(temp_var / 3600, "%02d", dest, max_limit);
					dest = convert_date_time_format((temp_var % 3600) / 60, "%02d", dest, max_limit);

					i++;
					continue;
				}

			case DATE_TIME_TZ:
				dest = format_time_into_specs("%a, %d %b %Y %H:%M:%S %z", t, dest, max_limit, p_timeval);
				i++;
				continue;

			case LITERAL:
			default:
			break;
			}
		}

		if (dest == max_limit) break;
			*dest++ = time_format[i+1];
			i = i+2;

	}

	return dest;
}

char *convert_date_time_format(const int32_t n, const char* date_time_format,
									char* dest_buffer, const char *max_dest_limit)
{
	char  buffer[32] = {0};
	format_sprintf_s(buffer, sizeof(buffer), date_time_format, n);

	return add_timeformat_to_string(buffer, dest_buffer, max_dest_limit);
}


char *add_timeformat_to_string(const char *buffer, char *dest_buffer,
									  const char *max_dest_limit)
{

	for(; (dest_buffer < max_dest_limit && (*dest_buffer = *buffer++) != '\0') ; ++dest_buffer);

	return dest_buffer;
}




time_t timegm(struct tm *t)
{
    time_t tl, tb;
    struct tm *tg;

    tl = mktime (t);
    if (tl == -1)
    {
        t->tm_hour--;
        tl = mktime (t);
        if (tl == -1)
            return -1; /* can't deal with output from strptime */
        tl += 3600;
    }
    tg = gmtime (&tl);
    tg->tm_isdst = 0;
    tb = mktime (tg);
    if (tb == -1)
    {
        tg->tm_hour--;
        tb = mktime (tg);
        if (tb == -1)
            return -1; /* can't deal with output from gmtime */
        tb += 3600;
    }
    return (tl - (tb - tl));
}

static bool validate_date(uint32_t day, uint32_t month, uint32_t year) {
	if (month < 1 || month > 12) {
		return false;
	}
	if (day < 1) {
		return false;
	}

	uint32_t numg_day_names = 31;
	if (month == 2) {
		numg_day_names = 28;
		if (IF_LEAPYEAR(year)) {
			numg_day_names = 29;
		}
	} else if (month == 4 || month == 6 || month == 9 || month == 11) {
		numg_day_names = 30;
	}

	if (day > numg_day_names) {
		return false;
	}

	return true;
}

static bool  create_date(parse_date *pd, struct tm *time_ptr,
						int day, int month, int year)
{
	if (time_ptr->tm_hour > 23 || time_ptr->tm_min > 59 || time_ptr->tm_sec > 59)
		return false;

	if (time_ptr->tm_hour < 12 && (pd->field_parse_flags & PM_SEP))
		time_ptr->tm_hour += 12;
	else if (time_ptr->tm_hour == 12 && (pd->field_parse_flags & AM_SEP))
	time_ptr->tm_hour = 0;

	time_ptr->tm_mday = day;
	time_ptr->tm_mon = month;
	GIVE_YEAR(year);
	time_ptr->tm_year = year;
	return true;
}

static bool initiate_and_create_date(parse_date *pd, struct tm *time_ptr,
			uint32_t date, uint32_t date_order, int day, int month, int year)
{
	uint32_t day_count = 0;
	uint32_t day_try = 0;

	switch (date_order) {
		case 0:  day_try = date_order & MON_DAY_YEAR_ORDER; break;
	    case 1:  day_try = date_order & DAY_MON_YEAR_ORDER; break;
	    default: day_try = date_order & YEAR_MON_DAY_ORDER; break;
	}

	for( uint32_t i =0; i< date_order; i++, day_count++) {

		if (1 == day_count) {

			if(0==date) {
				day_try = date_order & ~(DAY_MON_YEAR_ORDER|YEAR_DAY_MON_ORDER);

		    } else if (1 == date) {
				day_try = date_order & ~(MON_DAY_YEAR_ORDER|YEAR_MON_DAY_ORDER|MON_YEAR_DAY_ORDER);
			 } else
				 day_try = date_order & ~(DAY_MON_YEAR_ORDER|YEAR_DAY_MON_ORDER);

		} else
			day_try = date_order;


		if (!day_try)
			continue;


		if (day_try & MON_DAY_YEAR_ORDER) {

			 if (validate_date(month,day,year)) {

				SWAP_POSITION(day,month);
				return create_date(pd, time_ptr, day, month, year);
			 }
			 date_order &= ~MON_DAY_YEAR_ORDER;
		} else if (day_try & YEAR_MON_DAY_ORDER) {

			if (validate_date(year,month,day)){
	                SWAP_POSITION(day,year);
	                return create_date(pd, time_ptr, day, month, year);
	        }

		    date_order &= ~YEAR_MON_DAY_ORDER;
		} else if (day_try & YEAR_DAY_MON_ORDER) {

			if (validate_date(month,year,day)) {
	                SWAP_POSITION(day,month);
	                SWAP_POSITION(month,year);
	                return create_date(pd, time_ptr, day, month, year);
	        }
	            date_order &= ~YEAR_DAY_MON_ORDER;
		} else if (day_try & DAY_MON_YEAR_ORDER) {

			if (validate_date(day,month,year))
				return create_date(pd, time_ptr, day, month, year);

	            date_order &= ~DAY_MON_YEAR_ORDER;
	    } else if (day_try & MON_YEAR_DAY_ORDER ) {
				if(validate_date(year, day, month)) {

					SWAP_POSITION(day,year);
					SWAP_POSITION(month,year);
					return create_date(pd, time_ptr, day, month, year);
				}
				date_order &= ~MON_YEAR_DAY_ORDER;
		}

	} //for

	if (2 == pd->field_count ) {
		day_count = 0;
	    year = 1;
	    pd->field_count = 0;
	    date_order = YEAR_MON_DAY_ORDER|MON_YEAR_DAY_ORDER;
		return initiate_and_create_date(pd, time_ptr, date, date_order, day, month, year);
	}

	return false;

}

static bool determine_date(parse_date *pd, uint32_t date,
		uint32_t seek, struct tm *timer_ptr)
{
	int  day =30, month = 12, year = 1899;
	uint32_t date_order;

	if (0 == pd->field_count ) {
	         return create_date(pd, timer_ptr, day, month, year);
	}

	day = pd->field_values[seek + 0];
	month = pd->field_values[seek + 1];

	if (2 == pd->field_count) {
		struct tm current;
		time_t t = time(NULL);
		format_gmtime_s(&current, &t);

		year = current.tm_year + 1900;

	}
	else
		year = pd->field_values[seek + 2];


	if (pd->field_flags[seek + 0] & MONTH_SEP) {
		date_order = MON_DAY_YEAR_ORDER;
	}  else if (pd->field_flags[seek + 1] & MONTH_SEP) {
		date_order = DAY_MON_YEAR_ORDER;
		if (pd->field_count > 2)
			date_order |= YEAR_MON_DAY_ORDER;
	} else if (pd->field_count > 2 && (pd->field_flags[seek + 2] & MONTH_SEP)) {

		date_order = YEAR_DAY_MON_ORDER;
	} else {
		date_order = MON_DAY_YEAR_ORDER|DAY_MON_YEAR_ORDER;
		if (pd->field_count > 2)
			date_order |= (YEAR_MON_DAY_ORDER|YEAR_DAY_MON_ORDER);
	}

	return initiate_and_create_date(pd, timer_ptr, date, date_order, day, month, year);
	return false;

}


bool DateFromStr(const char * strIn, struct timeval* ptvout, bool isLocal)
{
    static const char * tokens[] =
    {
        "January",      // LOCALE_SMONTHNAME1
        "February",     // LOCALE_SMONTHNAME2
        "March",        // LOCALE_SMONTHNAME3
        "April",        // LOCALE_SMONTHNAME4
        "May",          // LOCALE_SMONTHNAME5
        "June",         // LOCALE_SMONTHNAME6
        "July",         // LOCALE_SMONTHNAME7
        "August",       // LOCALE_SMONTHNAME8
        "September",    // LOCALE_SMONTHNAME9
        "October",      // LOCALE_SMONTHNAME10
        "November",     // LOCALE_SMONTHNAME11
        "December",     // LOCALE_SMONTHNAME12
        "",             // LOCALE_SMONTHNAME13
        "Jan",          // LOCALE_SABBREVMONTHNAME1
        "Feb",          // LOCALE_SABBREVMONTHNAME2
        "Mar",          // LOCALE_SABBREVMONTHNAME3
        "Apr",          // LOCALE_SABBREVMONTHNAME4
        "May",          // LOCALE_SABBREVMONTHNAME5
        "Jun",          // LOCALE_SABBREVMONTHNAME6
        "Jul",          // LOCALE_SABBREVMONTHNAME7
        "Aug",          // LOCALE_SABBREVMONTHNAME8
        "Sep",          // LOCALE_SABBREVMONTHNAME9
        "Oct",          // LOCALE_SABBREVMONTHNAME10
        "Nov",          // LOCALE_SABBREVMONTHNAME11
        "Dec",          // LOCALE_SABBREVMONTHNAME12
        "",             // LOCALE_SABBREVMONTHNAME13
        "Monday",       // LOCALE_SDAYNAME1
        "Tuesday",      // LOCALE_SDAYNAME2
        "Wednesday",    // LOCALE_SDAYNAME3
        "Thursday",     // LOCALE_SDAYNAME4
        "Friday",       // LOCALE_SDAYNAME5
        "Saturday",     // LOCALE_SDAYNAME6
        "Sunday",       // LOCALE_SDAYNAME7
        "Mon",          // LOCALE_SABBREVDAYNAME1
        "Tue",          // LOCALE_SABBREVDAYNAME2
        "Wed",          // LOCALE_SABBREVDAYNAME3
        "Thu",          // LOCALE_SABBREVDAYNAME4
        "Fri",          // LOCALE_SABBREVDAYNAME5
        "Sat",          // LOCALE_SABBREVDAYNAME6
        "Sun",          // LOCALE_SABBREVDAYNAME7
        "AM",           // LOCALE_S1159
        "PM"            // LOCALE_S2359
    };

    static const unsigned char ParseDateMonths[] =
    {
        1,2,3,4,5,6,7,8,9,10,11,12,13,
        1,2,3,4,5,6,7,8,9,10,11,12,13
    };

    unsigned int i;

    parse_date dp;
    uint32_t dwDateSeps = 0, iDate = 0;

    if (!strIn)
        return false;

    ptvout->tv_sec = 0;
    ptvout->tv_usec = 0;

    memset(&dp, 0, sizeof(dp));

    /* Parse the string into our structure */
    while (*strIn)
    {
        if (dp.field_count >= 7)
            break;

        if (isdigit(*strIn))
        {
            dp.field_values[dp.field_count] = strtoul(strIn, (char**)&strIn, 10);
            dp.field_count++;
            strIn--;
        }
        else if (isalpha(*strIn))
        {
            bool bFound = false;

            for (i = 0; i < sizeof(tokens)/sizeof(tokens[0]); i++)
            {
                size_t dwLen = strlen(tokens[i]);
                if (dwLen && !(stringcmp_format(strIn, tokens[i], dwLen)))
                {
                    if (i <= 25)
                    {
                        dp.field_values[dp.field_count] = ParseDateMonths[i];
                        dp.field_flags[dp.field_count] |= (MONTH_SEP|DATE_SEP);
                        dp.field_count++;
                    }
                    else if (i > 39)
                    {
                        if (!dp.field_count || dp.field_parse_flags & (AM_SEP|PM_SEP))
                            return false; //hRet = DISP_E_TYPEMISMATCH;
                        else
                        {
                            dp.field_flags[dp.field_count - 1] |= (i == 40 ? AM_SEP : PM_SEP);
                            dp.field_parse_flags |= (i == 40 ? AM_SEP : PM_SEP);
                        }
                    }
                    strIn += (dwLen - 1);
                    bFound = true;
                    break;
                }
            }

            if (!bFound)
            {
                if ((*strIn == 'a' || *strIn == 'A' || *strIn == 'p' || *strIn == 'P') &&
                    (dp.field_count && !(dp.field_parse_flags & (AM_SEP|PM_SEP))))
                {
                    /* Special case - 'a' and 'p' are recognised as short for am/pm */
                    if (*strIn == 'a' || *strIn == 'A')
                    {
                        dp.field_flags[dp.field_count - 1] |= AM_SEP;
                        dp.field_parse_flags |=  AM_SEP;
                    }
                    else
                    {
                        dp.field_flags[dp.field_count - 1] |= PM_SEP;
                        dp.field_parse_flags |=  PM_SEP;
                    }
                    strIn++;
                }
                else if (*strIn == 'T')
                {
                   //ignore
                }
                else
                {
                    //TRACE("No matching token for %s\n", debugstr_w(strIn));
                    return false; //hRet = DISP_E_TYPEMISMATCH;
                    //break;
                }
            }
        }
        else if (*strIn == ':' ||  *strIn == '.')
        {
            if (!dp.field_count || !strIn[1])
                return false; //hRet = DISP_E_TYPEMISMATCH;
            else
                dp.field_flags[dp.field_count - 1] |= TIME_SEP;
        }
        else if (*strIn == '-' || *strIn == '/')
        {
            dwDateSeps++;
            if (dwDateSeps > 2 || !dp.field_count || !strIn[1])
                return false; //hRet = DISP_E_TYPEMISMATCH;
            else
                dp.field_flags[dp.field_count - 1] |= DATE_SEP;
        }
        else if (*strIn == ',' || isspace(*strIn))
        {
            if (*strIn == ',' && !strIn[1])
                return false; //hRet = DISP_E_TYPEMISMATCH;
        }
        else
        {
            return false; //hRet = DISP_E_TYPEMISMATCH;
        }
        strIn++;
    }

    if (!dp.field_count || dp.field_count > 7 ||
        (dp.field_count == 1 && !(dp.field_parse_flags & (AM_SEP|PM_SEP))))
        return false; //hRet = DISP_E_TYPEMISMATCH;

    struct tm t;
    int32_t milliseconds;
    uint32_t dwOffset = 0; /* Start of date fields in dp.field_values */

    t.tm_wday = t.tm_hour = t.tm_min = t.tm_sec = milliseconds = 0;
    //st.wDayOfWeek = t.tm_hour = t.tm_min = t.tm_sec = st.wMilliseconds = 0;

    /* Figure out which numbers correspond to which fields.
     *
     * This switch statement works based on the fact that native interprets any
     * fields that are not joined with a time separator ('.' or ':') as date
     * fields. Thus we construct a value from 0-32 where each set bit indicates
     * a time field. This encapsulates the hundreds of permutations of 2-6 fields.
     * For valid permutations, we set dwOffset to point32_t to the first date field
     * and shorten dp.field_count by the number of time fields found. The real
     * magic here occurs in MakeDate() above, where we determine what
     * each date number must represent in the context of iDate.
     */
    //TRACE("0x%08x\n", TIMEFLAG(0)|TIMEFLAG(1)|TIMEFLAG(2)|TIMEFLAG(3)|TIMEFLAG(4));
	int val = ((dp.field_flags[0] & TIME_SEP) << 0) | ((dp.field_flags[1] & TIME_SEP) << 1) |
			  ((dp.field_flags[2] & TIME_SEP) << 2) | ((dp.field_flags[3] & TIME_SEP) << 3) |
			  ((dp.field_flags[4] & TIME_SEP) << 4);

	switch (val)
    {
        case 0x1: /* TT TTDD TTDDD */
            if (dp.field_count > 3 &&
                ((dp.field_flags[2] & (AM_SEP|PM_SEP)) || (dp.field_flags[3] & (AM_SEP|PM_SEP)) ||
                (dp.field_flags[4] & (AM_SEP|PM_SEP))))
                return false; //hRet = DISP_E_TYPEMISMATCH;
            else if (dp.field_count != 2 && dp.field_count != 4 && dp.field_count != 5)
                return false; //hRet = DISP_E_TYPEMISMATCH;
            t.tm_hour = dp.field_values[0];
            t.tm_min  = dp.field_values[1];
            dp.field_count -= 2;
            dwOffset = 2;
            break;

        case 0x3: /* TTT TTTDD TTTDDD */
            if (dp.field_count > 4 &&
                ((dp.field_flags[3] & (AM_SEP|PM_SEP)) || (dp.field_flags[4] & (AM_SEP|PM_SEP)) ||
                (dp.field_flags[5] & (AM_SEP|PM_SEP))))
                return false; //hRet = DISP_E_TYPEMISMATCH;
            else if (dp.field_count != 3 && dp.field_count != 5 && dp.field_count != 6)
                return false; //hRet = DISP_E_TYPEMISMATCH;
            t.tm_hour   = dp.field_values[0];
            t.tm_min = dp.field_values[1];
            t.tm_sec = dp.field_values[2];
            milliseconds = dp.field_values[3];
            dwOffset = 3;
            dp.field_count -= 3;
            break;

        case 0x4: /* DDTT */
            if (dp.field_count != 4 ||
                (dp.field_flags[0] & (AM_SEP|PM_SEP)) || (dp.field_flags[1] & (AM_SEP|PM_SEP)))
                return false; //hRet = DISP_E_TYPEMISMATCH;
            t.tm_hour = dp.field_values[2];
            t.tm_min  = dp.field_values[3];
            dp.field_count -= 2;
            break;

        case 0x0: /* T DD DDD TDDD TDDD */
            if (dp.field_count == 1 && (dp.field_parse_flags & (AM_SEP|PM_SEP)))
            {
                t.tm_hour = dp.field_values[0]; /* T */
                dp.field_count = 0;
                break;
            }
            else if (dp.field_count > 4 || (dp.field_count < 3 && dp.field_parse_flags & (AM_SEP|PM_SEP)))
            {
                return false; //hRet = DISP_E_TYPEMISMATCH;
            }
            else if (dp.field_count == 3)
            {
                if (dp.field_flags[0] & (AM_SEP|PM_SEP)) /* TDD */
                {
                    dp.field_count = 2;
                    t.tm_hour = dp.field_values[0];
                    dwOffset = 1;
                    break;
                }
                if (dp.field_flags[2] & (AM_SEP|PM_SEP)) /* DDT */
                {
                    dp.field_count = 2;
                    t.tm_hour = dp.field_values[2];
                    break;
                }
                else if (dp.field_parse_flags & (AM_SEP|PM_SEP))
                    return false; //hRet = DISP_E_TYPEMISMATCH;
            }
            else if (dp.field_count == 4)
            {
                dp.field_count = 3;
                if (dp.field_flags[0] & (AM_SEP|PM_SEP)) /* TDDD */
                {
                    t.tm_hour = dp.field_values[0];
                    dwOffset = 1;
                }
                else if (dp.field_flags[3] & (AM_SEP|PM_SEP)) /* DDDT */
                {
                    t.tm_hour = dp.field_values[3];
                }
                else
                    return false; //hRet = DISP_E_TYPEMISMATCH;
                break;
            }
            /* .. fall through .. */

        case 0x8: /* DDDTT */
            if ((dp.field_count == 2 && (dp.field_parse_flags & (AM_SEP|PM_SEP))) ||
                (dp.field_count == 5 && ((dp.field_flags[0] & (AM_SEP|PM_SEP)) ||
                (dp.field_flags[1] & (AM_SEP|PM_SEP)) || (dp.field_flags[2] & (AM_SEP|PM_SEP)))) ||
                dp.field_count == 4 || dp.field_count == 6)
                return false; //hRet = DISP_E_TYPEMISMATCH;
            t.tm_hour   = dp.field_values[3];
            t.tm_min = dp.field_values[4];
            if (dp.field_count == 5)
                dp.field_count -= 2;
            break;

        case 0xC: /* DDTTT */
            if (dp.field_count != 5 ||
                (dp.field_flags[0] & (AM_SEP|PM_SEP)) || (dp.field_flags[1] & (AM_SEP|PM_SEP)))
                return false; //hRet = DISP_E_TYPEMISMATCH;
            t.tm_hour   = dp.field_values[2];
            t.tm_min = dp.field_values[3];
            t.tm_sec = dp.field_values[4];
            milliseconds = dp.field_values[5];
            dp.field_count -= 3;
            break;

        case 0x18: /* DDDTTT */
            if ((dp.field_flags[0] & (AM_SEP|PM_SEP)) || (dp.field_flags[1] & (AM_SEP|PM_SEP)) ||
                (dp.field_flags[2] & (AM_SEP|PM_SEP)))
                return false; //hRet = DISP_E_TYPEMISMATCH;
            t.tm_hour   = dp.field_values[3];
            t.tm_min = dp.field_values[4];
            t.tm_sec = dp.field_values[5];
            milliseconds = dp.field_values[6];
            dp.field_count -= 3;
            break;

        default:
            return false; //hRet = DISP_E_TYPEMISMATCH;
            break;
    }

    if (determine_date(&dp, iDate, dwOffset, &t))
    {
        if (t.tm_year > 2037)
        {
            t.tm_year = 2037;
            t.tm_mon = 12;
            t.tm_hour = 23;
            t.tm_min = 59;
            t.tm_sec = 59;
            milliseconds = 0;
        }
        t.tm_year -= 1900;
        t.tm_mon--;
        t.tm_wday = 0;
        t.tm_yday = 0;
        if (isLocal)
        {
            t.tm_isdst = -1;
            ptvout->tv_sec = (long)mktime(&t);
        }
        else
        {
            t.tm_isdst = 0;
            ptvout->tv_sec = (long)timegm(&t);
        }
        ptvout->tv_usec = (long)(milliseconds * 1000);
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

STime::STime(int32_t year, int32_t mon, int32_t day, int32_t hour, int32_t min, int32_t sec, bool isLocal)
{
    timeval tv;
    struct tm t;

    memset(&t, 0, sizeof(t));

    t.tm_year = year - 1900;
    t.tm_mon = mon - 1;
    t.tm_mday = day;
    t.tm_hour = hour;
    t.tm_min = min;
    t.tm_sec = sec;

    if (isLocal)
    {
        t.tm_isdst = -1;
        tv.tv_sec = (long)mktime(&t);
    }
    else
    {
        t.tm_isdst = 0;
        tv.tv_sec = (long)timegm(&t);
    }

    tv.tv_usec = 0;

    set(tv);
}

int32_t STime::year()
{
   struct tm tms;
   time_t tv_sec  = (time_t)m_time.tv_sec;
    format_localtime_s(&tms, &tv_sec);
    return tms.tm_year + 1900;
}

int32_t STime::month()
{
   struct tm tms;
   time_t tv_sec  = (time_t)m_time.tv_sec;
    format_localtime_s(&tms, &tv_sec);
    return tms.tm_mon + 1;
}

int32_t STime::day()
{
   struct tm tms;
   time_t tv_sec  = (time_t)m_time.tv_sec;
    format_localtime_s(&tms, &tv_sec);
    return tms.tm_mday;
}

int32_t STime::hour()
{
   struct tm tms;
   time_t tv_sec  = (time_t)m_time.tv_sec;
    format_localtime_s(&tms, &tv_sec);
    return tms.tm_hour;
}

int32_t STime::minute()
{
   struct tm tms;
   time_t tv_sec  = (time_t)m_time.tv_sec;
    format_localtime_s(&tms, &tv_sec);
    return tms.tm_min;
}

int32_t STime::second()
{
   struct tm tms;
   time_t tv_sec  = (time_t)m_time.tv_sec;
    format_localtime_s(&tms, &tv_sec);
    return tms.tm_sec;
}

STime STime::Now()
{
    struct timeval tv;
    struct timezone tz;

    gettimeofday(&tv, &tz);

    return STime(tv.tv_sec, tv.tv_usec);
}

void STime::Format(std::string& dest, const char * fmt, bool local)
{
    char buf[2048];
    Format(buf, sizeof(buf), fmt, local);
    dest.assign(buf);
}

void STime::Format(char * dest, int32_t maxsize, const char * fmt, bool local)
{
    struct tm ts;
    time_t t = m_time.tv_sec;

    if (local)
        format_localtime_s(&ts, &t);
    else
        format_gmtime_s(&ts, &t);

    format_time_into_string(dest, maxsize, fmt, &ts, &m_time);
}

bool STime::ParseDateTime(const char * pszDate, bool isLocal)
{
    return DateFromStr(pszDate, &m_time, isLocal);
}

void STime::getNTPTime(ntp_time_t &ntp) const
{
    ntp.second = m_time.tv_sec + 0x83AA7E80;
    ntp.fraction = (uint32_t)((double)(m_time.tv_usec+1) * (double)(1LL<<32) * 1.0e-6);
}

void STime::setNTPTime(const ntp_time_t &ntp)
{
    m_time.tv_sec = ntp.second - 0x83AA7E80;
    m_time.tv_usec = (uint32_t)((double)ntp.fraction * 1.0e6 / (double)(1LL<<32));
}

