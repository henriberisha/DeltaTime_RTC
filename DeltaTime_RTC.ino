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

myTimeInfo getDeltaTime_1(myTimeInfo *timeInfo, time_t deltaSeconds = 0)
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
String getDeltaTime_2(myTimeInfo *timeInfo, time_t deltaSeconds = 0)
{
	tmElements_t myElements = {timeInfo->my_sec, timeInfo->my_min, timeInfo->my_hour, timeInfo->my_weekDay, timeInfo->my_day, timeInfo->my_month, timeInfo->my_year - 1970};
	time_t myTime = makeTime(myElements);

	time_t deltaTime = myTime + deltaSeconds;

	setTime(deltaTime);

	String sec = padd(second()), min = padd(minute()), hr = padd(hour()), d = padd(day()), mt = padd(month()), yr = String(year());

	return String(yr + "-" + mt + "-" + d +
				  "T" + hr + ":" + min + ":" + sec + "." + padd(timeInfo->my_millis) + "Z");
}

String getDeltaTime_OR_TimeNow(ESP32Time *timeObj, time_t deltaSeconds = 0)
{
	// This returns TimeNow as there is no deltaseconds
	if (deltaSeconds == 0)
	{
		return String(timeObj->getTime("%FT%T") + "." + padd(timeObj->getMillis()) + "Z");
	}

	// This returns DeltaTime depending on deltaseconds (can be +/-)
	else
	{
		myTimeInfo info = {timeObj->getSecond(), timeObj->getMinute(), timeObj->getHour(true), timeObj->getDayofWeek() + 1, timeObj->getDay(), timeObj->getMonth() + 1, timeObj->getYear(), timeObj->getMillis()};
		tmElements_t nowElements = {info.my_sec, info.my_min, info.my_hour, info.my_weekDay, info.my_day, info.my_month, info.my_year - 1970};
		time_t nowTime = makeTime(nowElements);

		time_t deltaTime = nowTime + deltaSeconds;

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
	rtc_UTC.setTime(43, 37, 20, 12, 8, 2023); // 2023-08-12  20:37:43
}

// Loop will print time, a demonstration of RTC
void loop()
{
	Serial.println("Now: \t\t\t\t\t\t" + rtc_UTC.getTime("%FT%T") + "." + padd(rtc_UTC.getMillis()) + "Z");

	myTimeInfo info = {rtc_UTC.getSecond(), rtc_UTC.getMinute(), rtc_UTC.getHour(true), rtc_UTC.getDayofWeek() + 1, rtc_UTC.getDay(), rtc_UTC.getMonth() + 1, rtc_UTC.getYear(), rtc_UTC.getMillis()};

	myTimeInfo delta = getDeltaTime_1(&info, 3600);
	String deltaTime_1 = String(delta.my_year) + "-" + padd(delta.my_month) + "-" + padd(delta.my_day) +
						 "T" + padd(delta.my_hour) + ":" + padd(delta.my_min) + ":" + padd(delta.my_sec) + "." + padd(delta.my_millis) + "Z";
	Serial.println("getDeltaTime_1: \t\t\t\t" + deltaTime_1);

	String deltaTime_2 = getDeltaTime_2(&info, -3600);
	Serial.println("getDeltaTime_2: \t\t\t\t" + deltaTime_2);

	time_t ds3 = 300;  // Set delta seconds for a future datetime after 300 seconds(5 mins) have passed
	time_t dS3 = -600; // Set delta seconds to a past datetime in 600 seconds(10 mins)

	String deltaTime_3 = getDeltaTime_OR_TimeNow(&rtc_UTC);
	Serial.println("getDeltaTime_OR_TimeNow --> Now: \t\t" + deltaTime_3);

	deltaTime_3 = getDeltaTime_OR_TimeNow(&rtc_UTC, ds3);
	Serial.println("getDeltaTime_OR_TimeNow --> Delta future: \t" + deltaTime_3);

	deltaTime_3 = getDeltaTime_OR_TimeNow(&rtc_UTC, dS3);
	Serial.println("getDeltaTime_OR_TimeNow --> Delta past: \t" + deltaTime_3);
	Serial.println("******************************************************************************************************************************\n\n");

	// delay(5000);
}