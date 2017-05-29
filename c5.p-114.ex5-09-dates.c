/*
 * Exercise 5-9. Rewrite the routines day_of_year and month_day with pointers
 * instead of indexing.
 */
#include <stdio.h>
#include <stdint.h>

/*
 * Array of days in the month with boolean value for leap year status.
 * The third array are the correction for each month for the day.
 */
static char daytab[3][13] = {
	{ 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
	{ 0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
	{-1,  0,  3,  3,  6,  1,  4,  6,  2,  5,  0,  3,  5 }
};

/*
 * Returns boolean, is/isnot leap year.
 */
static int leap(const register int_fast16_t year)
{
	return (year%4 == 0 && year%100 != 0) || year%400 == 0;
}

/*
 * Returns 1 if day is valid for given month, 0 if there is a problem.
 */
static int check_day(		const int_fast16_t year,
				const uint_fast8_t month,
				const uint_fast16_t day)
{
	if (day <= daytab[leap(year)][month] && day > 0)
		return 0;
	return -1;
}

/*
 * Century table, gives Gregorian correction for the given year, input data
 * required is the century not the year.
 */
static int century(int cent)
{
	static int correction[2][9] = {
		{  0,  1,  2,  3 },
		{  0,  5,  3,  1 }
	};

	return correction[1][cent%4];
}

/*
 * Returns a pointer to the name of the month.
 */
static char *month_name(const register uint_fast8_t month)
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
static char *the_day(const uint_fast8_t day)
{
	static char *name[] = {
		"Saturday", "Sunday", "Monday", "Tuesday",
		"Wednesday", "Thursday", "Friday"
	};

	return name[day];
}

/*
 * Returns the name of the day for the given date.
 */
static char *day_name(		register int_fast16_t year,
				register uint_fast8_t month,
				register uint_fast16_t day)
{
	register uint_fast8_t value; /* Used to store the running calculation value */

	/*
	 * 1) Add the Day and the value of the Month from daytab[][]. If the
	 * resulting number is greater than 6, subtract the highest multiple of
	 * 7 in it.  Hold this number till step 3. 
	 */
	day = (day + daytab[2][month]) % 7;

	/*
	 * 2) Subtract from the (last two digits of the) Year, the greatest
	 * multiple of 28 within it. Add to the resulting number, the number
	 * that you get when you divide it by 4 and round down droping the
	 * decimal (floor). Now add the value for the Century from the Century
	 * Table. If the Month is Jan. or Feb. and the Year is a leap year,
	 * subtract 1. 
	 */
	value = year%100;

	if (value == 0)
		value = 101;

	value = value%28 + value/4;
	value += century(year/100);
	if (month < 3)
		value -= leap(year);

	/*
	 * 3) Add together the results from steps 1 and 2. If the resulting
	 * number is greater than 6, subtract the highest multiple of 7 in it.
	 * Using the resulting number, look up the Day-of-week in the
	 * Weekday-Table.
	 */
	return the_day((value + day) % 7);
}

/*
 * day_of_year:	set day of year from a givem month & day number.
 */
static int day_of_year(		const register int_fast16_t year,
				const register uint_fast8_t month,
				register uint_fast16_t day)
{
	register uint_fast8_t i;

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
static int month_day(		const register int_fast16_t year,
				register uint_fast16_t yearday,
				int *pmonth, int *pday)
{
	register uint_fast16_t i;

	if (year > INT16_MAX || year < INT16_MIN)
		return -1;

	if (yearday < 1 || yearday > 365+leap(year))
		return -1;

	for (i = 0; yearday > daytab[leap(year)][i]; i++)
		yearday -= daytab[leap(year)][i];

	*pmonth = i;
	*pday = yearday;

	return 0;
}

static int input_date(int year, int month, int day)
{
	int day_number;
	int is_date;
	int pmonth, pday;
	char *p_month_name;
	char *p_day_name;

	/*
	 * Get the numeric value for the day, out of all of the days in the
	 * year.
	 */
	day_number = day_of_year(year, month, day);

	if (day_number == -1) {
		printf("day_of_year: incorrect format.\n");
		return 1;
	}

	/*
	 * Translate the year and day number into the month and day.
	 */
	is_date = month_day(year, day_number, &pmonth, &pday);

	if (is_date == -1) {
		printf("month_day: incorrect format.\n");
		return 1;
	}

	p_day_name = day_name(year, month, day);
	p_month_name = month_name(month);

	/*
	 * Print out the day out of 365/6, the number of the day in the month
	 * and the month. Followed by the yeat and the name of the day.
	 */
	printf("%d %s %d\t-> %s day %d.\n", pday, p_month_name, year, p_day_name, day_number);

	return 0;
}

static void run_tests(void)
{
	input_date(1800, 2, 28);
	input_date(1804, 2, 29);
	input_date(1996, 2, 29);
	input_date(2000, 2, 29);
	input_date(2003, 2, 28);
	input_date(2004, 2, 29);
	input_date(2160, 3, 17);
	input_date(2200, 2, 28);
	input_date(2400, 2, 29);
	input_date(2800, 2, 29);
}

int main(int argc, char* argv[])
{
	run_tests();
		
	return 0;
}

