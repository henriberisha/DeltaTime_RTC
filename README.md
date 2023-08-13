# DeltaTime_RTC

### This is a short script written for ESP32 that keeps track of time using RTC and is able to give the deltatime provided the number of seconds

**Libraries used**

1. [TimeLib](https://github.com/PaulStoffregen/Time/tree/master)
2. [ESP32Time](https://github.com/fbiego/ESP32Time)

**Good reference to use**

> [ESP32Time Example](https://www.theelectronics.co.in/2022/04/how-to-use-internal-rtc-of-esp32.html)

**The objectives of this script are**

1. Keeping track of Real Time Clock (RTC) after initially setting the time
2. Calculating Delta DateTime given deltaSeconds

# In the code

`ESP32Time rtc_UTC;` declaring the object that will keep track of RTC
`rtc_UTC.setTime(...);` initializing it by setting the time

#### Functions

`getDeltaTime_1` --> METHOD #1 to find delta datetime
`getDeltaTime_2` --> METHOD #2 to find delta datetime
`getDeltaTime_OR_TimeNow` --> METHOD #3 to find delta datetim **PREFERRED WAY**

# Output Example

> Now: 2023-08-12T20:38:10.667Z
> getDeltaTime_1: 2023-08-12T21:38:10.668Z
> getDeltaTime_2: 2023-08-12T19:38:10.668Z
> getDeltaTime_OR_TimeNow --> Now: 2023-08-12T20:38:10.679Z
> getDeltaTime_OR_TimeNow --> Delta future: 2023-08-12T20:43:10.679Z
> getDeltaTime_OR_TimeNow --> Delta past: 2023-08-12T20:28:10.690Z
>
> ---
>
> Now: 2023-08-12T20:38:10.701Z
> getDeltaTime_1: 2023-08-12T21:38:10.712Z
> getDeltaTime_2: 2023-08-12T19:38:10.712Z
> getDeltaTime_OR_TimeNow --> Now: 2023-08-12T20:38:10.712Z
> getDeltaTime_OR_TimeNow --> Delta future: 2023-08-12T20:43:10.723Z
> getDeltaTime_OR_TimeNow --> Delta past: 2023-08-12T20:28:10.724Z
>
> ---
