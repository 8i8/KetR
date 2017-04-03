/*
 * Exercise 4-5. Add access to library functions like sin, exp, and pow. See
 * <math.h> in Appendix B, Section 4.
 */

#include <stdio.h>
#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>	/* For atof */
#include <float.h>	/* For DBL_EPSILON, the precision limit of double */
#include <math.h>	/* fabs() the absolute floating point value of input */
			/* fmod() for the remainder of two doubles devided */

#define MAXOP	100
#define NUMBER	'0'	/* A signal that a number was found. */

static char getop(char []);
static void push(double);
static double pop(void);
static void printStack(void);
static void swapStack(void);
static char tokenBuffer(char c);
static void printBuffer(void);
static void duplicate(void);
static void emptyStack(void);

int main(void)
{
	uint16_t type;
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
				/* DBL_EPSILON the smallest increment */
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
			case 'c':
				duplicate();
				break;
			case 'd':
				pop();
				break;
			case 'e':
				emptyStack();
				break;
			case 'p':
				printStack();
				break;
			case 's':
				swapStack();
				break;
			case '\n':
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
#define MAXVAL	100		/* Maximum depth of val stack */

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

/*
 * Output the contents of the stack to the terminal.
 */
static void printStack(void)
{
	size_t i;

	for (i = 0; i < sp; i++)
		printf("%f\n", val[i]);
}

/*
 * Swap the two top most eliments in the stack.
 */
static void swapStack(void)
{
	double temp;

	if (sp > 1) {
		temp = val[sp-1];
		val[sp-1] = val[sp-2];
		val[sp-2] = temp;
	}
	else
		printf("error: to few eliments in the stack.\n");

}

/*
 * Copy and make a new the top most stack value.
 */
static void duplicate(void)
{
	if (sp > 0)
		push(val[sp-1]);
}

/*
 * Empty the stack.
 */
static void emptyStack(void)
{
	sp = 0;
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
		;

	/* set a default end of string char at 2nd array index */
	s[1] = '\0';

	if (isalpha(c)) {
		return tokenBuffer(c);
	}

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

/*
 * Buffer to read text input.
 */
static char tokenBuffer(char c)
{
	char buft[3];

	buft[0] = c;
	if ((c = getch()) == '\n') {
		ungetch(c);
		return buft[0];
	}
	return c;
}

/*
 * Output the contents of the stack to the terminal.
 */
static void printBuffer(void)
{
	printf("buffer --> %lu\n", bufp);
}

