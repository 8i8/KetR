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

#define MAXOP	100	/* max size of operand or operator */
#define NUMBER	'0'	/* a signal that a number was found */

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
				if (op2 != 0.0)
					push(pop() / op2);
				else
					printf("error: zero divisor");
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
	if (sp < MAXVAL) {
		printf("pushed\n");
		val[sp++] = f;
	} else
		printf("error: stack full, can't push %g\n", f);
}

/*
 * Pop stack.
 */
double pop(void)
{
	if (sp > 0) {
		printf("popped\n");
		return val[--sp];
	} else {
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
	if (!isdigit(c) && c != '.')
		return c;	/* not a number */
	i = 0;
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

