#include <pi/base/TimeZone.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>

using namespace pi::TimeZone;

struct tm getTm(int year, int month, int day,
                int hour, int minute, int seconds)
{
    struct tm gmt;
    bzero(&gmt, sizeof gmt);
    gmt.tm_year = year - 1900;
    gmt.tm_mon = month - 1;
    gmt.tm_mday = day;
    gmt.tm_hour = hour;
    gmt.tm_min = minute;
    gmt.tm_sec = seconds;
    return gmt;
}

struct tm getTm(const char* str)
{
    struct tm gmt;
    bzero(&gmt, sizeof gmt);
    strptime(str, "%F %T", &gmt);
    return gmt;
}

time_t getGmt(int year, int month, int day, int hour, int minute, int seconds)
{
    struct tm gmt = getTm(year, month, day, hour, minute, seconds);
    return timegm(&gmt);
}

time_t getGmt(const char* str)
{
    struct tm gmt = gmtTm(str);
    return timegm(&gmt);
}

struct TestCase
{
    const char* gmt;
    const char* local;
    bool isdst;
};
int main()
{
    testNewYork();
}
