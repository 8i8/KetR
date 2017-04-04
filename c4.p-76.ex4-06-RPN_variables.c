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
			/* fmod() for the remainder of two doubles divided */

#define MAXOP	100
#define NUMBER	'0'	/* A signal that a number was found. */
#define VAR	1000

#define SIN	1001
#define COS	1002
#define TAN	1003
#define EXP	1004
#define LOG	1005
#define POW	1006
#define COPY	1007
#define DEL	1008
#define CLEAR	1009
#define PRINT	1010
#define SWAP	1011

static int16_t getop(char []);
static void push(double);
static double pop(void);
static void printStack(void);
static void swapStack(void);
static void duplicate(void);
static void emptyStack(void);

int main(void)
{
	int16_t type;
	char s[MAXOP];
	double op2;

	while ((type = getop(s)) != EOF) {
		switch (type) {
			case NUMBER:
				push(atof(s));
				break;
			case VAR:
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
				duplicate();
				break;
			case DEL:
				pop();
				break;
			case CLEAR:
				emptyStack();
				break;
			case PRINT:
			case 'p':
				printStack();
				break;
			case SWAP:
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
			case 0:
				break;
			case -2:
				printf("error: unknown token \n");
				break;
			default:
				printf("error: unknown command %s\n", s);
				break;
		}
	}
	return 0;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Numerical stack operations
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
 * Swap the two top most elements in the stack.
 */
static void swapStack(void)
{
	double temp;

	if (sp > 0) {
		temp = val[sp-1];
		val[sp-1] = val[sp-2];
		val[sp-2] = temp;
	}
	else
		printf("error: to few elements on the stack.\n");

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
 *  Input
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
#include <ctype.h>
#include <string.h>

#define NUMBER	'0'	/* A signal that a number was found. */
#define BUFSIZE	100

static char isVarAssigned(char c);
static char getch(void);
static void ungetch(char);
static int8_t isToken(void);
static int16_t readToken(void);
static char tokenBuffer(char c);

/*
 * getop: get next operator or numeric operand.
 */
static int16_t getop(char s[])
{
	size_t i;
	char c;

	/* If a token has been set; read it. */
	if (isToken())
		return readToken();

	/* keep inputing char until c is neither a space nor a tab */
	while ((s[0] = c = getch()) == ' ' || c == '\t')
		;

	/* set a default end of string char at 2nd array index */
	s[1] = '\0';

	/*
	 * Send letters to the string buffer, to check for tokens, if true is
	 * returned, there was no token and the letter is a single character
	 * that is then sent for streat ment as a possible variable.
	 */
	if (isalpha(c)) {
		if (tokenBuffer(c))
			isVarAssigned(c);
		else {
			return 0;
		}
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

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Variable stack
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
static char isVarAssigned(char c)
{
	printf("var --> %c\n", c);
	return 0;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Input buffer
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
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

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Text token buffer
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
static char bufText[BUFSIZE];
static size_t bufpT = 0;
static int8_t token = 0;	/* Boolean, is a token set */

/*
 * Boolean switch showing availability of token.
 */
static int8_t isToken(void)
{
	if (token) {
		token = 0;
		return 1;
	}
	return 0;
}

/*
 * Push text onto stack.
 */
static void pushChar(char c)
{
	if (bufpT >= BUFSIZE)
		printf("pushChar: text buffer full\n");
	else {
		bufText[bufpT++] = c;
		bufText[bufpT] = '\0';
	}
}

/*
 * Empty text token buffer.
 */
static void clearText(void)
{
	bufpT = 0;
	bufText[bufpT] = '\0';
}

/*
 * Read the text buffer.
 */
static int16_t readToken(void)
{
	if (!strcmp(bufText, "sin")) {
		clearText();
		return 1001;
	} else if (!strcmp(bufText, "cos")) {
		clearText();
		return 1002;
	} else if (!strcmp(bufText, "tan")) {
		clearText();
		return 1003;
	} else if (!strcmp(bufText, "exp")) {
		clearText();
		return 1004;
	} else if (!strcmp(bufText, "log")) {
		clearText();
		return 1005;
	} else if (!strcmp(bufText, "pow")) {
		clearText();
		return 1006;
	} else if (!strcmp(bufText, "copy")) {
		clearText();
		return 1007;
	} else if (!strcmp(bufText, "del")) {
		clearText();
		return 1008;
	} else if (!strcmp(bufText, "print")) {
		clearText();
		return 1010;
	} else if (!strcmp(bufText, "swap")) {
		clearText();
		return 1011;
	}
	clearText();
	return -2;
}

/*
 * Buffer text input.
 */
static char tokenBuffer(char c)
{
	char d;

	if ((d = getch()) == ' ' || d == '\t' || d == '\n') {
		if (bufpT <= 1) {
			return c;
		} else {
			pushChar(tolower(c));
			token = 1;
			return 0;
		}
	} else {
		pushChar(tolower(c));
		if(d != EOF)
			tokenBuffer(d);
	}
	return 0;
}

