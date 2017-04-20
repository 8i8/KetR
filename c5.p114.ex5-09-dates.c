/*
 * Exercise 5-9. Rewrite the routines day_of_year and month_day with pointers
 * instead of indexing.
 *
 * http://www.wikihow.com/Calculate-the-Day-of-the-Week
 */
#include <stdio.h>

static char daytab[2][13] = {
	{ 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
	{ 0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }
};

/*
 * Returns boolean, is/isnot leap year.
 */
static int leap(const int year)
{
	return (year%4 == 0 && year%100 != 0) || year%400 == 0;
}

/*
 * Returns 1 if day is valid for given month, 0 if there is a problem.
 */
static int check_day(const int year, const int month, const int day)
{
	if (day <= daytab[leap(year)][month] && day > 0)
		return 0;
	return -1;
}

/*
 * Returns a pointer to the name of the month.
 */
static char *month_name(const int month)
{
	static char *name[] = {
		"Illeagal month",
		"January", "Febuary", "March",
		"April", "May", "June",
		"July", "August", "September",
		"October", "November", "December"
	};

	return (month < 1 || month > 12) ? name[0] : name[month];
}

/*
 * Returns a pointer to the name of the day.
 */
static char *day_name(int day, int year)
{
	static char *name[] = {
		"Sunday", "Monday", "Tuesday", "Wednesday",
		"Thursday", "Friday", "Saturday"
	};

	return name[day];
}

/*
 * day_of_year:	set day of year from month & day.
 */
static int day_of_year(const int year, const int month, int day)
{
	int i;

	if (year == 0 || month < 1 || month > 12)
		return -1;

	if (check_day(year, month, day))
		return -1;

	for (i = 1; i < month; i++)
		day += daytab[leap(year)][i];

	return day;
}

/*
 * month_day:	set month, day from day of year.
 */
static int month_day(const int year, int yearday, int *pmonth, int *pday)
{
	int i;

	if (yearday < 1 || yearday > 365+leap(year))
		return -1;

	for (i = 0; yearday > daytab[leap(year)][i]; i++)
		yearday -= daytab[leap(year)][i];

	*pmonth = i;
	*pday = yearday;

	return 0;
}

int main(void)
{
	int day;
	int is_date;
	int pmonth, pday;
	char *pmonth_name;

	day = day_of_year(1974, 9, 10);

	if (day == -1) {
		printf("error: day_of_year incorrect format.\n");
		return 1;
	} else
		printf("Day out of 365 : %d\n", day);

	is_date = month_day(1974, 253, &pmonth, &pday);
	pmonth_name = month_name(pmonth);

	if (is_date == -1) {
		printf("error: month_day incorrect format.\n");
		return 1;
	} else
		printf("%s %d\n", pmonth_name, pday);

	return 0;
}

