#ifndef RTC_H_INCLUDED
#define RTC_H_INCLUDED

#define SEC_ADDR     0X00 
#define MIN_ADDR     0X02
#define HOUR_ADDR    0X04
#define DOW_ADDR     0X06
#define DAY_ADDR     0X07
#define MONTH_ADDR   0X08
#define YEAR_ADDR    0X09

#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71

int rtc_read_sec();

int rtc_read_min();

int rtc_read_hour();

int rtc_read_dow();

int rtc_read_day();

int rtc_read_month();

int rtc_read_year();

#endif
