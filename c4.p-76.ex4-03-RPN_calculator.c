/*
 * Input should be entered on one line as the carrage return will pop the
 * stack.
 *
 * Exercise 4-3. Given the basic framework, it's straightforward to extend the
 * calculator. Add the modulus (%) operator and provisions for negative
 * numbers.
 */

#include <stdio.h>
#include <stdlib.h>	/* For atof */
#include <float.h>	/* For DBL_EPSILON, the precision limit of double */
#include <math.h>	/* fabs() the absolute floating point value of input */
			/* fmod() for the remainder of two doubles devided */

#define MAXOP	100
#define NUMBER	'0'	/* A signal that a number was found. */

static char getop(char []);
static void push(double);
static double pop(void);

int main(void)
{
	char type;
	char s[MAXOP];
	double op2;

	while ((type = getop(s)) != EOF) {
		switch (type) {
			case NUMBER:
				push(atof(s));
				break;
			case '+':
				push(pop() + pop());
				break;
			case '*':
				push(pop() * pop());
				break;
			case '-':
				op2 = pop();
				push(pop() - op2);
				break;
			case '/':
				op2 = pop();
				if (fabs(op2 - 0) > DBL_EPSILON)
					push(pop() / op2);
				else
					printf("error: zero divisor\n");
				break;
			case '%':
				op2 = pop();
				if (fabs(op2 - 0) > DBL_EPSILON)
					/* math.h for mod of doubles */
					push(fmod(pop(), op2));
				else
					printf("error: zero modulo\n");
				break;
			case '\n':
				printf("\t%.8g\n", pop());
				break;
			default:
				printf("error: unknown command %s\n", s);
				break;
		}
	}
	return 0;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
#define MAXVAL	100	/* Maximum depth of val stack */

static size_t sp = 0;		/* Next free stack position */
static double val[MAXVAL];	/* value stack */

/*
 * push: push f onto value stack
 */
static void push(double f)
{
	if (sp < MAXVAL) {
		val[sp++] = f;
	}
	else
		printf("error: stack full, can't push %g\n", f);
}

/*
 * pop: pop and return top value from stack.
 */
static double pop(void)
{
	if (sp > 0)
		return val[--sp];
	else {
		printf("error: stack empty\n");
		return 0.0L;
	}
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
#include <ctype.h>

#define NUMBER	'0'	/* A signal that a number was found. */
#define BUFSIZE	100


static char getch(void);
static void ungetch(char);

/*
 * getop: get next operator or numeric operand.
 */
static char getop(char s[])
{
	size_t i;
	char c;

	/* keep inputing char until c is neither a space nor a tab */
	while ((s[0] = c = getch()) == ' ' || c == '\t')

	/* set a default end of string char at 2nd array index */
	s[1] = '\0';

	/* if c is any operator other than '.' or '-' return it */
	if (!isdigit(c) && c != '.' && c != '-')
		return c;

	/*
	 * If c is the '-' sign, check the next char, if a digit or a point,
	 * continue else send char to store and return a minus
	 */
	i = 0;
	if (c == '-') {
		if (isdigit(c = getch()) || c == '.')
			s[i++] = c;
		else {
			if (c != (char)EOF)
				ungetch(c);
			return '-';
		}
	}

	/* If it is a digit, start counting */
	if (isdigit(c))
		while (isdigit(s[++i] = c = getch()))
			;

	/* if c is a decimal point, start counting the fractional part */
	if (c == '.')
		while (isdigit(s[++i] = c = getch()))
			;

	s[i] = '\0';
	if (c != (char)EOF)
		ungetch(c);

	return NUMBER;
}

static char buf[BUFSIZE];	/* buffer for ungetch */
static size_t bufp = 0;		/* next free position in buf */

/*
 * Get a (possibly pushed back) character.
 */
static char getch(void)
{
	return (bufp > 0) ? buf[--bufp] : (char)getchar();
}

/*
 * Push character back on input.
 */
static void ungetch(char c)
{
	if (bufp >= BUFSIZE)
		printf("ungetch: too many characters\n");
	else
		buf[bufp++] = c;
}

