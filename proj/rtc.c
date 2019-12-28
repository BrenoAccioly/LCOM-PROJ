#include <lcom/lcf.h>
#include "rtc.h"

int rtc_read_sec(){
  uint32_t sec = 0;
	sys_outb(RTC_ADDR_REG, SEC_ADDR);
	sys_inb(RTC_DATA_REG, &sec);
	return (((sec & 0xF0) >> 4) * 10) + (sec & 0x0F);
}

int rtc_read_min(){
  uint32_t min = 0;
	sys_outb(RTC_ADDR_REG, MIN_ADDR);
	sys_inb(RTC_DATA_REG, &min);
	return (((min & 0xF0) >> 4) * 10) + (min & 0x0F);
}

int rtc_read_hour(){
  uint32_t hour = 0;
	sys_outb(RTC_ADDR_REG, HOUR_ADDR);
	sys_inb(RTC_DATA_REG, &hour);
	return (((hour & 0xF0) >> 4) * 10) + (hour & 0x0F);
}

int rtc_read_dow(){
  uint32_t dow = 0;
	sys_outb(RTC_ADDR_REG, DOW_ADDR);
	sys_inb(RTC_DATA_REG, &dow);
	return (((dow & 0xF0) >> 4) * 10) + (dow & 0x0F);
}

int rtc_read_day(){
  uint32_t day = 0;
	sys_outb(RTC_ADDR_REG, DAY_ADDR);
	sys_inb(RTC_DATA_REG, &day);
	return (((day & 0xF0) >> 4) * 10) + (day & 0x0F);
}

int rtc_read_month(){
  uint32_t month = 0;
	sys_outb(RTC_ADDR_REG, MONTH_ADDR);
	sys_inb(RTC_DATA_REG, &month);
	return (((month & 0xF0) >> 4) * 10) + (month & 0x0F);
}

int rtc_read_year(){
  uint32_t year = 0;
	sys_outb(RTC_ADDR_REG, YEAR_ADDR);
	sys_inb(RTC_DATA_REG, &year);
	return (((year & 0xF0) >> 4) * 10) + (year & 0x0F);
}
