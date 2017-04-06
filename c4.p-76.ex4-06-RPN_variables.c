/*
 * Exercise 4-6. Add commands for handling variables. (It's easy to provide
 * twenty-six variables with single-letter names.) Add a variable for the most
 * recently printed value.
 */

#include <stdio.h>
#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>	/* For atof */
#include <float.h>	/* For DBL_EPSILON, the precision limit of double */
#include <math.h>	/* fabs() the absolute floating point value of input */
			/* fmod() for the remainder of two doubles divided */
/* Error messages */
#define PARAM	1
#define DIVISOR	2
#define TOKEN	3
#define COMMAND	4

/* Function tokens */
#define NUMBER	998	/* A signal that a number was found. */
#define NEG	999
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
#define PS	1011
#define SWAP	1012
#define EXIT	1013

#define MAXOP	100
#define VARLIM	26

static void printError(int num);
/* Numerical stack */
static void push(double);
static double pop(void);
static char popVar(void);
static void printStack(void);
static void printHead(void);
static void swapStack(void);
static void duplicate(void);
static void emptyStack(void);
/* Input, get operation */
static int16_t getop(char []);
/* variable stack */
static void setVarToEmpty(void);
static void setVar(int i, double value);
static void printVar(void);
static void pushVar(char c);
static int twoValues(void);
static int isVar(void);

int main(void)
{
	int16_t type;
	char s[MAXOP];
	double op2;
	char ch2;
	int sign;

	setVarToEmpty();
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
			case VAR:
				pushVar(s[0]);
				break;
			case '=':
				if(isVar() == 1)
				{
					ch2 = popVar();
					setVar(ch2, pop());
				}
				else if (isVar() == 2)
				{
					op2 = pop();
					setVar(popVar(), op2);
				}
				else
					printError(VAR);
				break;
			case '+':
				if (twoValues())
					push(pop() + pop());
				else 
					printError(PARAM);
				break;
			case '*':
				if (twoValues())
					push(pop() * pop());
				else 
					printError(PARAM);
				break;
			case '-':
				if (twoValues()) {
					op2 = pop();
					push(pop() - op2);
				} else 
					printError(PARAM);
				break;
			case '/':
				if (twoValues()) {
					op2 = pop();
					/* DBL_EPSILON the smallest increment */
					if (fabs(op2) > DBL_EPSILON)
						push(pop() / op2);
					else {
						printError(DIVISOR);
					}
				} else 
					printError(PARAM);
				break;
			case '%':
				if (twoValues()) {
					op2 = pop();
					if (fabs(op2) > DBL_EPSILON)
						/* math.h for mod of doubles */
						push(fmod(pop(), op2));
					else {
						printError(DIVISOR);
					}
				} else 
					printError(PARAM);
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
			case 'p':
				printHead();
				break;
			case PRINT:
			case PS:
				printStack();
				break;
			case SWAP:
			case 's':
				swapStack();
				break;
			case 'v':
				printVar();
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
				if (twoValues()) {
					op2 = pop();
					push(pow( pop(), op2 ));
				} else 
					printError(PARAM);
				break;
			case '\n':
				break;
			case 0:
				break;
			case -2:
				printError(TOKEN);
				break;
			case EXIT:
			case 'q':
				goto exit;
				break;
			default:
				printError(COMMAND);
				break;
		}
	}
exit:
	return 0;
}

/*
 * Store of error messages.
 */
static void printError(int num)
{
	switch(num)
	{
		case PARAM:
			puts("error: insufficient parameters");
			break;
		case DIVISOR:
			puts("error: zero divisor");
			break;
		case TOKEN:
			puts("error: unknown token");
			break;
		case COMMAND:
			puts("error: unknown command");
			break;
		case VAR:
			puts("error: no variable");
			break;
		default:
			break;
	}
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Numerical stack
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
#define MAXVAL	100		/* Maximum depth of val stack */
#define I_DEPTH	2		/* Depth of index table, type, index */
#define BOOL	2		/* Number of states for boolean values */
#define	SET	1
#define UNSET	0

#define REAL	0

#define TYPE	0
#define INDEX	1

static size_t fp = 0;		/* Next free stack position */
static double st_val[MAXVAL];	/* value stack */
static void set_index(int type, int index);

/*
 * Push float onto numerical stack.
 */
static void push(double f)
{
	if (fp < MAXVAL) {
		st_val[fp] = f;
		set_index(REAL, fp);
		fp++;
	}
	else
		printf("error: Float stack full, can't push %g\n", f);
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Var stack
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
static size_t vp = 0;		/* Next free var stack position */
static char st_var[MAXVAL];	/* Var stack */

/*
 * pushVar: push c onto value stack
 */
static void pushVar(char v)
{
	if (vp < MAXVAL) {
		st_var[vp] = v;
		set_index(VAR, vp);
		vp++;
	}
	else
		printf("error: Variable stack full, can't push %c\n", v);
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Var value
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
static double var_values[VARLIM];	/* The variables a-z */
static int var_state[VARLIM];

/*
 * Set the value and status of a variable, the status here is used when listing
 * all of the active variables, the nature of the index requires this, the char
 * is defined numericaly by `c - 'A'`.
 */
static void setVar(int i, double value)
{
	var_values[i - 'A'] = value;
	var_state[i - 'A'] = SET;
}

/*
 * Clear the playing field.
 */
static void setVarToEmpty(void)
{
	size_t i = VARLIM;

	while (--i > 0)
		var_values[i] = '\0';
}

/*
 * Display all currently set variables and their values.
 */
static void printVar(void)
{
	size_t i;

	for (i = 0; i < VARLIM; i++)
		if (var_state[i] == SET)
			printf("%c:%f\n", ((char)i + 'A'), var_values[i]);
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Index stack
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
static size_t ip = 0;			/* Next free index position. */
static int st_ind[MAXVAL][I_DEPTH];	/* The stack index, records type. */

/*
 * index: keep track of the stack index and the type of value at that index.
 */
static void set_index(int t, int i)
{
	if (ip < MAXVAL) {
		st_ind[ip][TYPE] = t;
		st_ind[ip][INDEX] = i;
		ip++;
	}
	else
		printf("error: Index stack full, can't push\n");
}

/*
 * Are there two values that are required for the function?
 */
static int twoValues(void)
{
	if (ip >= 2)
		return 1;
	return 0;
}

/*
 * Is there a variable in one of the next two places?
 */
static int isVar(void)
{
	if (st_ind[ip-1][TYPE] == VAR)
		return 1;
	else if (st_ind[ip-2][TYPE] == VAR)
		return 2;
	return 0;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Stack operations
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

/*
 * Pop and return top value from stack.
 */
static double pop(void)
{
	if (ip > 0) {
		if (st_ind[--ip][TYPE] == REAL) {
			fp--;
			return st_val[st_ind[ip][INDEX]];
		}
		else
			printf("error: pop failed ");
	}
	else {
		printf("error: stack empty\n");
	}
	return 0;
}

/*
 * Pop and return top value from stack.
 */
static char popVar(void)
{
	if (ip > 0) {
		if (st_ind[--ip][TYPE] == VAR) {
			vp--;
			return st_var[st_ind[ip][INDEX]];
		} else
			printf("error: pop failed ");
	} else {
		printf("error: stack empty\n");
	}
	return 0;
}

/*
 * Output the hed of the stack.
 */
static void printHead(void)
{
	if (ip > 0) {
		if(st_ind[ip-1][TYPE] == REAL)
			printf("%f\n", st_val[st_ind[ip-1][INDEX]]);
		else if (st_ind[ip-1][TYPE] == VAR)
			printf("%c\n", st_var[st_ind[ip-1][INDEX]]);
	}
}

/*
 * Output the contents of the stack to the terminal.
 */
static void printStack(void)
{
	size_t i;

	for (i = 0; i < ip; i++) {
		if(st_ind[i][TYPE] == REAL)
			printf("%f\n", st_val[st_ind[i][INDEX]]);
		else if (st_ind[i][TYPE] == VAR)
			printf("%c\n", st_var[st_ind[i][INDEX]]);
	}
}

/*
 * Swap the two top most elements in the stack.
 */
static void swapStack(void)
{
	int i = 0;
	int temp[I_DEPTH];

	if (ip > 0) {
		while (i < I_DEPTH) {
			temp[i] = st_ind[ip-1][i];
			st_ind[ip-1][i] = st_ind[ip-2][i];
			st_ind[ip-2][i] = temp[i];
			i++;
		}
	}
	else
		printf("error: to few elements on the stack.\n");

}

/*
 * Copy and make a new, the top most stack value.
 */
static void duplicate(void)
{
	if (ip > 0) {
		if(st_ind[ip][TYPE] == REAL)
			push(st_val[st_ind[ip][INDEX]-1]);
		else if (st_ind[ip][TYPE] == VAR)
			pushVar(st_var[st_ind[ip][INDEX]-1]);
	}
}

/*
 * Empty the stack.
 */
static void emptyStack(void)
{
	ip = 0;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Input
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
#include <ctype.h>
#include <string.h>

#define BUFSIZE	100

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

	/* set a default end of string char at 2nd array ī*/
	s[1] = '\0';

	/*
	 * Send letters to the string buffer, to check for tokens, if true is
	 * returned, there was no token and the letter is a single character
	 * that is then sent for streat ment as a possible variable.
	 */
	if (isalpha(c)) {
		c = tokenBuffer(c);
		if (c) {
			if (isupper(c)) {
				s[0] = c;
				s[1] = '\0';
				return VAR;
			}
			else
				return c;
		}
		else
			return 0;
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
		if (isdigit(c = getch()) || c == '.') {
			ungetch(c);
			return NEG;
		} else {
			if (c != (char)EOF)
				ungetch(c);
			return '-';
		}
	}

	/* If it is a digit, construct the full number using s[] as a store */
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
 *  Input buffer
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
static char buf[BUFSIZE];		/* buffer for ungetch */
static size_t buf_p = 0;		/* next free position in buf */

/*
 * Get a (possibly pushed back) character.
 */
static char getch(void)
{
	return (buf_p > 0) ? buf[--buf_p] : (char)getchar();
}

/*
 * Push character back on input.
 */
static void ungetch(char c)
{
	if (buf_p >= BUFSIZE)
		printf("ungetch: too many characters\n");
	else
		buf[buf_p++] = c;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Text token buffer
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
static char bufText[BUFSIZE];
static size_t bufT_p = 0;
static int8_t token = 0;		/* Boolean, is a token set */

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
static void pushTokenChar(char c)
{
	if (bufT_p >= BUFSIZE)
		printf("pushTokenChar: text buffer full\n");
	else {
		bufText[bufT_p++] = c;
		bufText[bufT_p] = '\0';
	}
}

/*
 * Empty text token buffer.
 */
static void clearText(void)
{
	bufT_p = 0;
	bufText[bufT_p] = '\0';
}

/*
 * Read the text buffer.
 */
static int16_t readToken(void)
{
	if (!strcmp(bufText, "sin")) {
		clearText();
		return SIN;
	} else if (!strcmp(bufText, "cos")) {
		clearText();
		return COS;
	} else if (!strcmp(bufText, "tan")) {
		clearText();
		return TAN;
	} else if (!strcmp(bufText, "exp")) {
		clearText();
		return EXP;
	} else if (!strcmp(bufText, "log")) {
		clearText();
		return LOG;
	} else if (!strcmp(bufText, "pow")) {
		clearText();
		return POW;
	} else if (!strcmp(bufText, "copy")) {
		clearText();
		return COPY;
	} else if (!strcmp(bufText, "del")) {
		clearText();
		return DEL;
	} else if (!strcmp(bufText, "print")) {
		clearText();
		return PRINT;
	} else if (!strcmp(bufText, "ps")) {
		clearText();
		return PS;
	} else if (!strcmp(bufText, "swap")) {
		clearText();
		return SWAP;
	} else if (!strcmp(bufText, "exit")) {
		clearText();
		return EXIT;
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
		if (bufT_p < 1) {
			return c;
		} else {
			pushTokenChar(tolower(c));
			token = 1;
			return 0;
		}
	} else {
		pushTokenChar(tolower(c));
		if(d != EOF)
			tokenBuffer(d);
	}
	return 0;
}

