#include <TimeLib.h>   // Use this for getting a time delta
#include <ESP32Time.h> // Use this for keeping track of the time

typedef struct TimeInfo
{
	int my_sec;
	int my_min;
	int my_hour;
	int my_weekDay;
	int my_day;
	int my_month;
	int my_year;
	int my_millis;
} myTimeInfo;

String padd(int value)
{
	if (value < 10)
		return "0" + String(value);
	else
		return String(value);
}

myTimeInfo getStaleTime(myTimeInfo *timeInfo, time_t deltaSeconds = 0.0)
{
	tmElements_t myElements = {timeInfo->my_sec, timeInfo->my_min, timeInfo->my_hour, timeInfo->my_weekDay, timeInfo->my_day, timeInfo->my_month, timeInfo->my_year - 1970};
	time_t myTime = makeTime(myElements);

	time_t deltaTime = myTime + deltaSeconds;

	setTime(deltaTime);

	myTimeInfo time =
		{
			second(),
			minute(),
			hour(),
			weekday(),
			day(),
			month(),
			year(),
			timeInfo->my_millis};

	return time;
}

// Same as the above function but returns time as timeISO8601 format
String getTimeDelta(myTimeInfo *timeInfo, time_t deltaSeconds = 0.0)
{
	tmElements_t myElements = {timeInfo->my_sec, timeInfo->my_min, timeInfo->my_hour, timeInfo->my_weekDay, timeInfo->my_day, timeInfo->my_month, timeInfo->my_year - 1970};
	time_t myTime = makeTime(myElements);

	time_t deltaTime = myTime + deltaSeconds;

	setTime(deltaTime);

	String sec = padd(second()), min = padd(minute()), hr = padd(hour()), d = padd(day()), mt = padd(month()), yr = String(year());

	return String(yr + "-" + mt + "-" + d +
				  "T" + hr + ":" + min + ":" + sec + "." + padd(timeInfo->my_millis) + "Z");
}

String takTime(ESP32Time *timeObj, long deltaSeconds = 0)
{
	if (deltaSeconds == 0)
	{
		return String(timeObj->getTime("%FT%T") + "." + padd(timeObj->getMillis()) + "Z");
	}
	else
	{
		myTimeInfo info = {timeObj->getSecond(), timeObj->getMinute(), timeObj->getHour(true), timeObj->getDayofWeek() + 1, timeObj->getDay(), timeObj->getMonth() + 1, timeObj->getYear(), timeObj->getMillis()};
		tmElements_t nowElements = {info.my_sec, info.my_min, info.my_hour, info.my_weekDay, info.my_day, info.my_month, info.my_year - 1970};
		time_t nowTime = makeTime(nowElements);

		time_t deltaTime = nowTime + time_t(deltaSeconds);

		setTime(deltaTime);

		String deltaSec = padd(second()), deltaMin = padd(minute()), deltaHr = padd(hour()), deltaDay = padd(day()), deltaMonth = padd(month()), deltaYr = String(year());

		return String(deltaYr + "-" + deltaMonth + "-" + deltaDay + "T" + deltaHr + ":" + deltaMin + ":" + deltaSec + "." + padd(info.my_millis) + "Z");
	}
}

ESP32Time rtc_UTC;

void setup()
{
	Serial.begin(115200);
	delay(2000);

	// Set initial time. This is just example, prefered setting would be from NTP, NetworkTime, GPS time
	rtc_UTC.setTime(43, 37, 20, 12, 8, 2023);
}

void loop()
{
	Serial.println("Now: " + rtc_UTC.getTime("%FT%T") + "." + padd(rtc_UTC.getMillis()) + "Z");

	myTimeInfo info = {rtc_UTC.getSecond(), rtc_UTC.getMinute(), rtc_UTC.getHour(true), rtc_UTC.getDayofWeek() + 1, rtc_UTC.getDay(), rtc_UTC.getMonth() + 1, rtc_UTC.getYear(), rtc_UTC.getMillis()};

	/*
	myTimeInfo stale = getStaleTime(&info, 0);

	String staleTime = String(stale.my_year) + "-" + String(stale.my_month) + "-" + String(stale.my_day) +
					   "T" + String(stale.my_hour) + ":" + String(stale.my_min) + ":" + String(stale.my_sec) + "." + String(stale.my_millis) + "Z";

	Serial.println("Stale: " + staleTime);
	*/

	time_t deltaSeconds = 3600; // Set delta seconds
	// time_t deltaSeconds = -300; // Set delta seconds

	Serial.println("Stale: " + getTimeDelta(&info, deltaSeconds));

	delay(5000);

	Serial.println(takTime(&rtc_UTC, 0L));
}