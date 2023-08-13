# DeltaTime_RTC

### This is a short script written for ESP32 that keeps track of time using RTC and is able to give the deltatime provided the number of seconds

**Libraries used**

1. [TimeLib](https://github.com/PaulStoffregen/Time/tree/master)
2. [ESP32Time](https://github.com/fbiego/ESP32Time)

**Good reference to use**

> [ESP32Time Example](https://www.theelectronics.co.in/2022/04/how-to-use-internal-rtc-of-esp32.html)

**The objectives of this script is to do the following**

1. Keep track of Real Time Clock (RTC) after initially setting the time
2. Calculate Delta DateTime given deltaSeconds
