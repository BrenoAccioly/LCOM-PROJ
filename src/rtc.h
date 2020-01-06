#ifndef RTC_H_INCLUDED
#define RTC_H_INCLUDED

/** @defgroup RTC RTC
 * @{
 *
 * Functions and constants for the Real Time Clock.
 */

#define SEC_ADDR     0X00 
#define MIN_ADDR     0X02
#define HOUR_ADDR    0X04
#define DOW_ADDR     0X06
#define DAY_ADDR     0X07
#define MONTH_ADDR   0X08
#define YEAR_ADDR    0X09

#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71

/**
 * @return the seconds
 */
int rtc_read_sec();

/**
 * @return the minutes 
 */
int rtc_read_min();

/**
 * @return the hour
 */
int rtc_read_hour();

/**
 * @return the day of the week
 */
int rtc_read_dow();

/**
 * @return the day 
 */
int rtc_read_day();

/**
 * @return the month 
 */
int rtc_read_month();

/**
 * @return the year 
 */
int rtc_read_year();

#endif
