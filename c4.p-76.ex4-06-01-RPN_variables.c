/*
 * Exercise 4-6. Add commands for handling variables. (It's easy to provide
 * twenty-six variables with single-letter names.) Add a variable for the most
 * recently printed value.
 */

#include <stdio.h>
#include <string.h>	/* For strcmp() */
#include <stdlib.h>	/* for atof() */
#include <float.h>	/* For DBL_EPSILON, the precision limit of double */
#include <math.h>	/* fabs() the absolute floating point value of input */
			/* fmod() for the remainder of two doubles devided */

#define MAXOP	100	/* max size of operand or operator */
#define NUMBER	1001	/* a signal that a number was found */
#define NEG	1002
#define SIN	1003
#define COS	1004
#define TAN	1005
#define EXP	1006
#define LOG	1007
#define POW	1008
#define COPY	1009
#define DEL	1010
#define PRINT	1011
#define SWAP	1012
#define EXIT	1013
#define CLEAR	1014
#define EMPTY	1015

static int readToken(char []);
static int getop(char []);
static void push(double);
static double pop(void);
static void printStack(void);
static void printTopTwo(void);
static void swapStack(void);
static void duplicate(void);
static void emptyStack(void);

/*
 * RPN calculator.
 */
int main(void)
{
	int type;
	char s[MAXOP];
	double op2;
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
			case COPY:
			case 'c':
				duplicate();
				break;
			case DEL:
			case 'd':
				pop();
				break;
			case CLEAR:
			case 'e':
				emptyStack();
				break;
			case PRINT:
				printStack();
				break;
			case 'p':
				printTopTwo();
				break;
			case SWAP:
			case 's':
				swapStack();
				break;
			case SIN:
				push(sin( pop() ));
				break;
			case COS:
				push(cos( pop() ));
				break;
			case TAN:
				push(tan( pop() ));
				break;
			case EXP:
				push(exp( pop() ));
				break;
			case LOG:
				push(log( pop() ));
				break;
			case '^':
			case POW:
				op2 = pop();
				push(pow( pop(), op2 ));
				break;
			case '\n':
				break;
			case EMPTY:
				break;
			case EXIT:
			case 'q':
				goto exit;
				break;
			default:
				printf("error: unknown command %s\n", s);
				break;
		}
	}
exit:
	return 0;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Tokens
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

/*
 * Read the input string.
 */
static int readToken(char s[])
{
	if (!strcmp(s, "sin"))
		return SIN;
	else if (!strcmp(s, "cos"))
	      return COS;
	else if (!strcmp(s, "tan"))
	      return TAN;
	else if (!strcmp(s, "exp"))
	      return EXP;
	else if (!strcmp(s, "log"))
	      return LOG;
	else if (!strcmp(s, "pow"))
	      return POW;
	else if (!strcmp(s, "copy"))
	      return COPY;
	else if (!strcmp(s, "del"))
	      return DEL;
	else if (!strcmp(s, "print"))
	      return PRINT;
	else if (!strcmp(s, "swap"))
	      return SWAP;
	else if (!strcmp(s, "exit"))
	      return EXIT;
	else
		printf("error: unrecognised token\n");

	s[0] = '\0';
	return 0;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Input getop.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
#include <ctype.h>

static int getch(void);
static void ungetch(int);

/*
 * getop: get the next operator or operand.
 */
static int getop(char s[])
{
	int i, c;

	while ((s[0] = c = getch()) == ' ' || c == '\t' || c == '\n')
		;

	s[1] = '\0';

	if (!isalnum(c) && c != '.' && c != '-')
		return c;	/* not a number */

	/*
	 * If the input is a letter chect to see if the following char is also
	 * a letter; If not return the single letter, if yes then check for a
	 * token.
	 */
	i = 0;
	if (isalpha(c)) {
		s[i++] = c;
		while (( s[i++] = c = getch() ))
			if (i == 2 && (c == ' ' || c == '\t' || c == '\n'))
				return s[0];
			else if (!isalpha(c))
				break;
		ungetch(c);
		s[--i] = '\0';
		return readToken(s);	/* Read the token and send to main */
	}

	/*
	 * If c is the '-' sign, check the next char, if a digit or a point,
	 * continue else send char to store and return a minus.
	 */
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

static char buf[BUFSIZE];	/* Buffer for next ungetch */
static int bufp = 0;		/* next free position in buf */

static int getch(void)		/* get a (possibly pushed back) character */
{
	return (bufp > 0) ? buf[--bufp] : getchar();
}

static void ungetch(int c)	/* push character back on input */
{
	if (bufp >= BUFSIZE)
		printf("ungetch: too many characters\n");
	else
		buf[bufp++] = c;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Stack
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
#define MAXVAL	100

static int sp = 0;
static int index[2*MAXVAL];
static int var[MAXVAL][2];
static double val[MAXVAL];

/*
 * Push onto stack.
 */
static void push(double f)
{
	if (sp < MAXVAL)
		val[sp++] = f;
	else
		printf("error: stack full, can't push %g\n", f);
}

/*
 * Pop stack.
 */
static double pop(void)
{
	if (sp > 0)
		return val[--sp];
	else {
		printf("error: stack empty\n");
		return 0.0;
	}
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Stack operations
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
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
 * Output the last two items in the stack.
 */
static void printTopTwo(void)
{
	size_t i;

	for (i = sp; i > sp-2; i--)
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

