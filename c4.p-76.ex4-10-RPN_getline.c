/*
 * Exercise 4-10. An alternate organization uses getline to read an entire
 * input line; this makes getch and ungetch unnecessary. Revise the calculator
 * to use this approach.
 */

#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <string.h>	/* For strcmp() */
#include <stdlib.h>	/* for atof() */
#include <float.h>	/* For DBL_EPSILON, the precision limit of double */
#include <math.h>	/* fabs() the absolute floating point value of input */
			/* fmod() for the remainder of two doubles divided */

#define MAXOP	100	/* max size of operand or operator */
#define PREV	0
#define NUMBER	1001	/* a signal that a number was found */
#define VARIAB	1002
#define NEG	1003
#define SIN	1004
#define COS	1005
#define TAN	1006
#define EXP	1007
#define LOG	1008
#define POW	1009
#define COPY	1010
#define DEL	1011
#define PRINT	1012
#define PRINT_2	1013
#define SWAP	1014
#define EXIT	1015
#define CLEAR	1016
#define EMPTY	1017

static int readToken(char []);
static int getop(char []);
static void push(int, double);
static double pop(void);
static void printStack(void);
static void printTopTwo(void);
static void printOutput(size_t i);
static void swapStack(void);
static void duplicate(void);
static void printVarStack(void);
static void printValueStack(void);
static void printIndex(void);
static void emptyStack(void);
static int getVarChar(void);
static double getVarValue(int v);
static int getVarPos(void);

/*
 * RPN calculator.
 */
int main(void)
{
	int type;
	char s[MAXOP];
	double op2;
	int sign;
	int var;

	sign = 1;
	var = 0;

	while ((type = getop(s)) != EOF)
	{
		switch (type) {
			case NEG:
				sign = -1;
				break;
			case NUMBER:
				push(PREV, atof(s) * sign);
				sign = 1;
				break;
			case VARIAB:
				push(s[0], getVarValue(s[0]));
				break;
			case '=':
				if (getVarPos() == 1) {
					var = getVarChar();
					pop();
					push(var, pop());
				} else if (getVarPos() == 2) {
					var = getVarChar();
					swapStack();
					pop();
					push(var, pop());
				} else
					printf("error: no variable in range");
				break;
			case '+':
				push(var, pop() + pop());
				break;
			case '*':
				push(var, pop() * pop());
				break;
			case '-':
				op2 = pop();
				push(var, pop() - op2);
				break;
			case '/':
				op2 = pop();
				/* DBL_EPSILON the smallest increment */
				if (fabs(op2 - 0) > DBL_EPSILON)
					push(var, pop() / op2);
				else
					printf("error: zero divisor\n");
				break;
			case '%':
				op2 = pop();
				if (fabs(op2 - 0) > DBL_EPSILON)
					/* math.h for mod of doubles */
					push(var, fmod(pop(), op2));
				else
					printf("error: zero modulo\n");
				break;
			case COPY:
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
			case PRINT_2:
			case 'p':
				printTopTwo();
				break;
			case PRINT:
				printStack();
				break;
			case SWAP:
			case 's':
				swapStack();
				break;
			case SIN:
				push(var, sin( pop() ));
				break;
			case COS:
				push(var, cos( pop() ));
				break;
			case TAN:
				push(var, tan( pop() ));
				break;
			case EXP:
				push(var, exp( pop() ));
				break;
			case LOG:
				push(var, log( pop() ));
				break;
			case '^':
			case POW:
				op2 = pop();
				push(var, pow( pop(), op2 ));
				break;
			case '\n':
				break;
			case EMPTY:
				break;
			case EXIT:
			case 'q':
				goto exit;
				break;
			case 'x':
				printVarStack();
				break;
			case 'y':
				printValueStack();
				break;
			case 'z':
				printIndex();
				break;
			default:
				printf("error: unknown command %s\n", s);
				break;
		}
		var = 0;
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
	int val;

	if (!strcmp(s, "sin"))
		val = SIN;
	else if (!strcmp(s, "cos"))
		val = COS;
	else if (!strcmp(s, "tan"))
		val = TAN;
	else if (!strcmp(s, "exp"))
		val = EXP;
	else if (!strcmp(s, "log"))
		val = LOG;
	else if (!strcmp(s, "pow"))
		val = POW;
	else if (!strcmp(s, "copy"))
		val = COPY;
	else if (!strcmp(s, "del"))
		val = DEL;
	else if (!strcmp(s, "print") || !strcmp(s, "pr"))
		val = PRINT;
	else if (!strcmp(s, "top") || !strcmp(s, "pt"))
		val = PRINT_2;
	else if (!strcmp(s, "swap"))
		val = SWAP;
	else if (!strcmp(s, "exit"))
		val = EXIT;
	else
		printf("error: unrecognised token\n");

	s[0] = '\0';

	return val;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Input getop.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
#define MAXLINE		255
#include <ctype.h>

static size_t getline(char s[], size_t lim);

/*
 * getop: get the next operator or operand.
 */
static int getop(char s[])
{
	static char in[MAXLINE];	/* Array retained between itterations */
	static int i = 0;		/* Index also remembered */
	int j = 0;
	char c;

	s[0] = '\0';			/* Reset getop string */
	if (in[i] == '\n')		/* Reset when end of line is reached */
		i = j = 0;

	if (i == 0)			/* Get input from stdin */
		getline(in, MAXLINE);

	while (((c = in[i++]) == ' ' || c == '\t'))	/* Remove spaces */
		;

	s[1] = '\0';
	s[j++] = c;

	if (!isalnum(c) && c != '.' && c != '-')
		return c;	/* not a number */

	/*
	 * If the input is a letter check to see if the char that follows is
	 * also a letter; If not return the single letter, if yes then check
	 * for a token.
	 */
	if (islower(c)) {
		while (( s[j++] = c = in[i++] ))
			if (j == 2 && (c == ' ' || c == '\t' || c == '\n')) {
				i--;
				s[1] = '\0';
				return s[0];
			} else if (!isalpha(c))
				break;
		i--;
		s[--j] = '\0';
		return readToken(s);	/* Read the token and send to main */
	}

	if (isupper(c)) {
		return VARIAB;
	}

	/*
	 * If c is the '-' sign, check the next char, if a digit or a point,
	 * continue else send char to store and return a minus.
	 */
	if (c == '-') {
		if (isdigit(c = in[i++]) || c == '.') {
			i--;
			return NEG;
		} else {
			i--;
			return '-';
		}
	}

	if (isdigit(c))		/* collect the integer part */
		while (isdigit(s[j++] = c = in[i++]))
			;
	if (c == '.')		/* collect the fractional part */
		while (isdigit(s[j++] = c = in[i++]))
			;
	s[--j] = '\0';
	i--;

	return NUMBER;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Buffer.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

static size_t getline(char s[], size_t lim)
{
	char c;
	int i = 0;

	while (--lim > 0 && (c = getchar()) != EOF && c != '\n')
		s[i++] = c;
	if (c == '\n')
		s[i++] = c;
	s[i] = '\0';

	return i;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Stack
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
#define MAXVAL		100
#define ALPHABET	26
#define FLOAT_LEN	16

enum type	{ NUM, VAR, TYPE };
enum inde	{ ID, TYP };

/*
 * in_p: stack index
 * va_p: value index
 * vr_p: variable index
 */
static int in_i; 
static int vl_i;
static int vr_i;

/* ind ~ id ~ type */
static int ind[MAXVAL][TYPE];
static int var[MAXVAL];
static double varVal[ALPHABET] = {0.0};
static double val[MAXVAL];

/*
 * Push double onto stack.
 */
static void pushDub(double f)
{
	if (vl_i < MAXVAL)
		val[vl_i++] = f;
	else
		printf("error: stack full, can't push %g\n", f);
}

/*
 * Push a double value into both the supplied var and the stack.
 */
static void pushVar(int v, double f)
{
	if (vr_i < MAXVAL) {
		varVal[v - 'A'] = f;
		var[vr_i++] = v;
	} else
		printf("error: stack full, can't push %c:%g\n", v, f);
}

/*
 * Generic push, either var or double.
 */
static void push(int v, double f)
{
	if (v == PREV) {
		ind[in_i][TYP] = NUM;
		ind[in_i++][ID] = vl_i;
		pushDub(f);
	} else {
		ind[in_i][TYP] = VAR;
		ind[in_i++][ID] = vr_i;
		pushVar(v, f);
	}
}

/*
 * Pop stack.
 */
static double pop(void)
{
	if (in_i > 0) {
		in_i--;
		if (ind[in_i][TYP] == NUM) {
			return val[--vl_i];
		} else if (ind[in_i][TYP] == VAR) {
			--vr_i;
			return varVal[var[ind[in_i][ID]]-'A'];
		}
	} else
		printf("error: insufficient values stack\n");

	return 0.0;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Stack operations
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

/*
 * Print the contents of the stack.
 */
static void printStack(void)
{
	printOutput(0);
}

/*
 * Set last printed value to Z var.
 */
static void setLastPrintedVar()
{
	if(ind[in_i-1][TYP] == NUM)
		varVal[25] = val[ind[in_i-1][ID]];
	else if(ind[in_i-1][TYP] == VAR)
		varVal[25] = varVal[var[ind[in_i-1][ID]]-'A'];
}

/*
 * Output the last two items in the stack.
 */
static void printTopTwo(void)
{
	if (in_i > 1)
		printOutput(in_i-2);
	else if (in_i == 1)
		printOutput(in_i-1);

	setLastPrintedVar();
}

/*
 * Print loop for stack output to terminal.
 */
static void printOutput(size_t i)
{
	char s[FLOAT_LEN];

	for ( ; i < in_i; i++)
		if (ind[i][TYP] == NUM) {
			sprintf(s, "%.15g", val[ ind[i][ID] ]);
			printf(" -> %s\n", s);
		} else if (ind[i][TYP] == VAR) {
			sprintf(s, "%.15g", varVal[ var[ind[i][ID]] -'A']);
			printf("%c-> %s\n", var[ind[i][ID]], s);
		}
	setLastPrintedVar();
}

/*
 * Swap the two top most elements in the stack.
 */
static void swapStack(void)
{
	int tempTp, tempId;

	if (in_i > 1) {
		tempTp = ind[in_i-1][TYP];
		tempId = ind[in_i-1][ID];
		ind[in_i-1][TYP] = ind[in_i-2][TYP];
		ind[in_i-1][ID] = ind[in_i-2][ID];
		ind[in_i-2][TYP] = tempTp;
		ind[in_i-2][ID] = tempId;
	}
	else
		printf("error: to few elements in the stack.\n");

}

/*
 * Copy and make a new the top most stack value.
 */
static void duplicate(void)
{
	if (vl_i > 0)
		push(PREV, val[vl_i-1]);
}

/*
 * Print the variable stack.
 */
static void printVarStack(void)
{
	int i = 0;
	char s[FLOAT_LEN];

	while(i < ALPHABET) {
		sprintf(s, "%.15g", varVal[i]);
		printf("%c -> %s\n", i + 'A', s);
		i++;
	}
}

/*
 * Print the value stack.
 */
static void printValueStack(void)
{
	int i = 0;
	char s[FLOAT_LEN];

	while (i < vl_i) {
		sprintf(s, "%.15g", val[i]);
		printf("%d -> %s\n", i, s);
		i++;
	}
}

/*
 * Print the index.
 */
static void printIndex(void)
{
	int i = 0;

	while (i < in_i) {
		printf("%d -> TYP: %d ID: %d\n", i, ind[i][TYP], ind[i][ID]);
		i++;
	}
}

/*
 * Empty the stack.
 */
static void emptyStack(void)
{
	in_i = vl_i = 0;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Variables
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

/*
 * Return variable position in top two stack positions.
 */
static int getVarChar(void)
{
	if (ind[in_i-1][TYP] == VAR) {
		return var[ind[in_i-1][ID]];
	} else if (ind[in_i-2][TYP] == VAR) {
		return var[ind[in_i-2][ID]];
	}
	return 0;
}

/*
 * getVarValue
 */
static double getVarValue(int v)
{
	return varVal[v - 'A'];
}

/*
 * Get var position.
 */
static int getVarPos(void)
{
	if (ind[in_i-1][TYP] == VAR)
		return 1;
	if (ind[in_i-2][TYP] == VAR)
		return 2;
	return 0;
}

