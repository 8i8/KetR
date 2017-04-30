/*
 * Section 4-3 RPN calculator
 *
 * For the caluculator to be used as it is the input need be entered onto one line, carrage return will pop the stack.
 *  
 *  10 2 +<cr>
 *  12
 */

#include <stdio.h>
#include <stdlib.h>	/* for atof() */
#include <float.h>	/* For DBL_EPSILON, the precision limit of double */
#include <math.h>	/* fabs() the absolute floating point value of input */
			/* fmod() for the remainder of two doubles devided */

#define MAXOP	100	/* max size of operand or operator */
#define NUMBER	'0'	/* a signal that a number was found */
#define NEG	-2

int getop(char []);
void push(double);
double pop(void);

/*
 * RPN calculator.
 */
int main(void)
{
	int type;
	double op2;
	char s[MAXOP];
	int sign;

	sign = 1;

	while ((type = getop(s)) != EOF) {
		switch (type) {
			case NEG:
				sign = -1;
				break;
			case NUMBER:
				push(atof(s) * sign);
				sign = 1;
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
 *  Stack
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
#define MAXVAL	100

int sp = 0;
double val[MAXVAL];

/*
 * Push onto stack.
 */

void push(double f)
{
	if (sp < MAXVAL)
		val[sp++] = f;
	else
		printf("error: stack full, can't push %g\n", f);
}

/*
 * Pop stack.
 */
double pop(void)
{
	if (sp > 0)
		return val[--sp];
	else {
		printf("error: stack empty\n");
		return 0.0;
	}
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Getop.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
#include <ctype.h>

int getch(void);
void ungetch(int);

/*
 * getop: get the next operator or operand.
 */
int getop(char s[])
{
	int i, c;

	while ((s[0] = c = getch()) == ' ' || c == '\t')
		;
	s[1] = '\0';
	if (!isdigit(c) && c != '.' && c != '-')
		return c;	/* not a number */
	/*
	 * If c is the '-' sign, check the next char, if a digit or a point,
	 * continue else send char to store and return a minus.
	 */
	i = 0;
	if (c == '-') {
		if (isdigit(c = getch()) || c == '.') {
			ungetch(c);
			return NEG;
		} else {
			if (c != (char)EOF)
				ungetch(c);
			return '-';
		}
	}

	if (isdigit(c))		/* collect the intager part */
		while (isdigit(s[++i] = c = getch()))
			;
	if (c == '.')		/* collect thta fractional part */
		while (isdigit(s[++i] = c = getch()))
			;
	s[i] = '\0';
	if (c != EOF)
		ungetch(c);
	return NUMBER;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Buffer.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
#define BUFSIZE	100

char buf[BUFSIZE];	/* Buffer for next ungetch */
int bufp = 0;		/* next free position in buf */

int getch(void)		/* get a (possibly pushed back) character */
{
	return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c)	/* push character back on input */
{
	if (bufp >= BUFSIZE)
		printf("ungetch: too many characters\n");
	else
		buf[bufp++] = c;
}

