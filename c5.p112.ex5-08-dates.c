/*
 * Exercise 5-8. There is no error checking in day_of_year or month_day. Remedy
 * this defect.
 */
#include <stdio.h>

static char daytab[2][13] = {
	{ 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
	{ 0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }
};

/*
 * Returns 1 if day is valid for given month, 0 if there is a problem.
 */
static int check_day(const int leap, const int month, const int day)
{
	if (day <= daytab[leap][month] && day > 0)
		return 0;
	return -1;
}

/*
 * day_of_year:	set day of year from month & day.
 */
static int day_of_year(const int year, const int month, const int day)
{
	int i, leap;

	if (year == 0 || month < 1 || month > 12)
		return -1;

	leap = year%4 == 0 && year%100 != 0 || year%400 == 0;

	if (check_day(leap, month, day))
		return -1;

	for (i = 1; i < month; i++)
		day += daytab[leap][i];

	return day;
}

/*
 * month_day:	set month, day from day of year.
 */
static int month_day(const int year, const int yearday, int *pmonth, int *pday)
{
	int i, leap;

	leap = year%4 == 0 && year%100 != 0 || year%400 == 0;

	if (yearday < 1 || yearday > 365+leap)
		return -1;

	for (i = 0; yearday > daytab[leap][i]; i++)
		yearday -= daytab[leap][i];
	*pmonth = i;
	*pday = yearday;

	return 0;
}

int main(void)
{
	int day;
	int is_date;
	int pmonth, pday;

	day = day_of_year(1974, 9, 10);

	if (day == -1) {
		printf("error: day_of_year incorrect format.\n");
		return 1;
	} else
		printf("Day out of 365 : %d\n", day);

	is_date = month_day(1974, 253, &pmonth, &pday);

	if (is_date == -1) {
		printf("error: month_day incorrect format.\n");
		return 1;
	} else
		printf("Month : %d Day : %d\n", pmonth, pday);

	return 0;
}

