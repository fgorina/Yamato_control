#ifndef NET_NTP_TIME_H
#define NET_NTP_TIME_H

#ifdef __cplusplus
extern "C"
{
#endif

  const char *ntpServer = "pool.ntp.org";
  const long utcOffset_sec = 0;
  const int daylightOffset_sec = 0; // 3600;

  void setupTime()
  {
    struct tm timeinfo;
    if (getLocalTime(&timeinfo))
    {

      Serial.println("Updating with Local Time");
      RTCdate.Year = timeinfo.tm_year;
      RTCdate.Month = timeinfo.tm_mon;
      RTCdate.Date = timeinfo.tm_mday;
      M5.Rtc.SetDate(&RTCdate);

      RTCtime.Hours = timeinfo.tm_hour;
      RTCtime.Minutes = timeinfo.tm_min;
      RTCtime.Seconds = timeinfo.tm_sec;
      M5.Rtc.SetTime(&RTCtime); // writes the  time to the (RTC) real time clock.
    }
  }

  void setupTimeGPS(String datetime)
  {
    Serial.println(datetime);
    RTCdate.Year = atoi(datetime.substring(0, 4).c_str());
    RTCdate.Month = atoi(datetime.substring(5, 7).c_str());
    RTCdate.Date = atoi(datetime.substring(8, 10).c_str());
    M5.Rtc.SetDate(&RTCdate);

    RTCtime.Hours = atoi(datetime.substring(11, 13).c_str());
    RTCtime.Minutes = atoi(datetime.substring(14, 16).c_str());
    RTCtime.Seconds = atoi(datetime.substring(17, 19).c_str());
    M5.Rtc.SetTime(&RTCtime); // writes the  time to the (RTC) real time clock.
  }

  void init_dateTime()
  {
    configTime(utcOffset_sec, daylightOffset_sec, ntpServer);
    setupTime();
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
