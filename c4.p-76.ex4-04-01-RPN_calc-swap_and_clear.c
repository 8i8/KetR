/*
 * Exercise 4-4. Add the commands to print the top elements of the stack
 * without popping, to duplicate it, and to swap the top two elements. Add a
 * command to clear the stack.
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
void printStack(void);
void printTopTwo(void);
void swapStack(void);
void duplicate(void);
void emptyStack(void);

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
			case 'c':		/* Duplicate last entry. */
				duplicate();
				break;
			case 'd':		/* Delete last entry. */
				pop();
				printStack();
				break;
			case 'e':		/* Clear the stack. */
				emptyStack();
				break;
			case 'p':		/* Print the stack */
				printStack();
				break;
			case 's':		/* Swap the previous two entrys. */
				swapStack();
				printStack();
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

/*
 * Output the contents of the stack to the terminal.
 */
void printStack(void)
{
	size_t i;

	for (i = 0; i < sp; i++)
		printf("%f\n", val[i]);
}

/*
 * Output the last two items in the stack.
 */
void printTopTwo(void)
{
	size_t i;

	for (i = sp; i > sp-2; i--)
		printf("%f\n", val[i]);
}

/*
 * Swap the two top most eliments in the stack.
 */
void swapStack(void)
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
void duplicate(void)
{
	if (sp > 0)
		push(val[sp-1]);
}

/*
 * Empty the stack.
 */
void emptyStack(void)
{
	sp = 0;
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

	if (isdigit(c))		/* collect the integer part */
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

