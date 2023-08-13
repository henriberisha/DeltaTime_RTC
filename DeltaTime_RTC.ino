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

/*!
	@brief  padd value with zero in front and return as String
	@param  value
			i.e. millis, sec, min, hr, day, month
*/
String padd(int value)
{
	if (value < 10)
		return "0" + String(value);
	else
		return String(value);
}

/*!
	@brief  function to calculate delta time || METHOD #1
	@param  timeInfo
			user defined struct to hold datetime components (pass by reference)
	@param  deltaSeconds
			(optional) seconds that have passed to calculate delta. by default = 0 will return time NOW
	@returns 'myTimeInfo' struct holding the datetime components needed to reconstruct the deltatime
*/
myTimeInfo getDeltaTime_1(myTimeInfo *timeInfo, time_t deltaSeconds = 0)
{
	// Making the deltatime using functions provided by 'TimeLib.h'
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

/*!
	@brief  function to calculate delta time || METHOD #2
	@param  timeInfo
			user defined struct to hold datetime components (pass by reference)
	@param  deltaSeconds
			(optional) seconds that have passed to calculate delta. by default = 0 will return time NOW
	@returns String representation of datetime in ISO 8601 format i.e. YYYY-MM-DDThh:mm:ss.sssZ
*/
String getDeltaTime_2(myTimeInfo *timeInfo, time_t deltaSeconds = 0)
{
	// Making the deltatime using functions provided by 'TimeLib.h'
	tmElements_t myElements = {timeInfo->my_sec, timeInfo->my_min, timeInfo->my_hour, timeInfo->my_weekDay, timeInfo->my_day, timeInfo->my_month, timeInfo->my_year - 1970};
	time_t myTime = makeTime(myElements);

	time_t deltaTime = myTime + deltaSeconds;
	setTime(deltaTime);

	String sec = padd(second()), min = padd(minute()), hr = padd(hour()), d = padd(day()), mt = padd(month()), yr = String(year());

	return String(yr + "-" + mt + "-" + d +
				  "T" + hr + ":" + min + ":" + sec + "." + padd(timeInfo->my_millis) + "Z");
}

/*!
	@brief  function to calculate delta time
	@param  timeObj
			ESP32Time object that is keeping track of RTC (pass by reference)
	@param  deltaSeconds
			(optional) seconds that have passed to calculate delta. by default = 0 will return time NOW
	@returns String representation of datetime in ISO 8601 format i.e. YYYY-MM-DDThh:mm:ss.sssZ
*/
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
		// Making the deltatime using functions provided by 'TimeLib.h'
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
	// Print time NOW using the RTC capability of ESP32Time library
	Serial.println("Now: \t\t\t\t\t\t" + rtc_UTC.getTime("%FT%T") + "." + padd(rtc_UTC.getMillis()) + "Z");

	// METHOD #1 Calculate and print deltatime with "getDeltaTime_1()"
	myTimeInfo info = {rtc_UTC.getSecond(), rtc_UTC.getMinute(), rtc_UTC.getHour(true), rtc_UTC.getDayofWeek() + 1, rtc_UTC.getDay(), rtc_UTC.getMonth() + 1, rtc_UTC.getYear(), rtc_UTC.getMillis()};

	myTimeInfo delta = getDeltaTime_1(&info, 3600); // Deltatime 1 hour in the future
	String deltaTime_1 = String(delta.my_year) + "-" + padd(delta.my_month) + "-" + padd(delta.my_day) +
						 "T" + padd(delta.my_hour) + ":" + padd(delta.my_min) + ":" + padd(delta.my_sec) + "." + padd(delta.my_millis) + "Z";
	Serial.println("getDeltaTime_1: \t\t\t\t" + deltaTime_1);
	// -- END METHOD #1 --

	// METHOD #2 Calculate and print deltatime with "getDeltaTime_2()"
	String deltaTime_2 = getDeltaTime_2(&info, -3600); // Deltatime 1 hour in the past
	Serial.println("getDeltaTime_2: \t\t\t\t" + deltaTime_2);
	// -- END METHOD #2 --

	// METHOD #3 - THIS IS OPTIMAL AND PREFERRED || Calculate and print NOW / deltatime with getDeltaTime_OR_TimeNow()
	time_t future_ds = 300; // Set delta seconds for a future datetime after 300 seconds(5 mins) have passed
	time_t past_ds = -600;	// Set delta seconds to a past datetime in 600 seconds(10 mins)

	String deltaTime_3 = getDeltaTime_OR_TimeNow(&rtc_UTC); // Time Now as deltaseconds = 0
	Serial.println("getDeltaTime_OR_TimeNow --> Now: \t\t" + deltaTime_3);

	deltaTime_3 = getDeltaTime_OR_TimeNow(&rtc_UTC, future_ds); // Deltatime 5 mins in the future
	Serial.println("getDeltaTime_OR_TimeNow --> Delta future: \t" + deltaTime_3);

	deltaTime_3 = getDeltaTime_OR_TimeNow(&rtc_UTC, past_ds); // Deltatime 10 mins in the past
	Serial.println("getDeltaTime_OR_TimeNow --> Delta past: \t" + deltaTime_3);
	Serial.println("******************************************************************************************************************************\n\n");
	// -- END METHOD #3 --

	// delay(5000);

	/*
		After every iteration of the loop when serial has printed, you will notice discrepancies in the millis component of the datetimes
		This is because it will take some time to calculate deltatimes after each different METHOD is called
		However they will be refering to the same NOW TIME that 'rtc_UTC' object is keeping track of
	*/
}