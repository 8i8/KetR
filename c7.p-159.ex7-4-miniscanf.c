/*
 * Exercise 7-4. Write a private version of scanf analogous to minprintf from
 * the previous section.
 * TODO
 */
#include <stdio.h>
#include <stdarg.h>

/*
 * miniscanf:	minimal scanf with variable argument list
 */
static int miniscanf(char *fmt, ...)
{
	va_list ap;	/* argument pointer */
	char *p, *sval;
	int *ival;
	double *dval;

	va_start(ap, fmt);
	for (p = fmt; *p; p++) {
		if (*p != '%') {
			continue;
		}
		switch (*++p) {
			case 'd':
				ival = va_arg(ap, int*);
				scanf("%d", ival);
				break;
			case 'f':
				dval = va_arg(ap, double*);
				scanf("%lf", dval);
				break;
			case 's':
				sval = va_arg(ap, char*);
				scanf("%s", sval);
				break;
			default:
				putchar(*p);
				break;
		}
		va_end(ap);
	}
	return 0;
}

int main(void)
{
	int a;
	double b;
	char str[12] = {'\0'};

	miniscanf("%d %f %s", &a, &b, str);

	printf("d -> %d\nf -> %f\nstr -> %s\n", a, b, str);
	return 0;
}

