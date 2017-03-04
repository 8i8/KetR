/*
 * Exercise 1-22. Write a program to ``fold'' long input lines into two or more
 * shorter lines after the last non-blank character that occurs before the n-th
 * column of input. Make sure your program does something intelligent with very
 * long lines, and if there are no blanks or tabs before the specified column.
 *
 * TODO check that this code works correctly for tabs.
 */
#include <stdio.h>

#define MAXLINE		1000
#define FOLD		20
#define TABLENGTH	8

int tabLength(char line[], int head, int fold, int tablength);
int checkLength(int length, int head);
int foldLine(char line[], char folded[], int head, int fold, int last);
int myGetlen(char line[], int lim);

int main(void)
{
	char line[MAXLINE+1];
	char folded[FOLD+1];
	int length;
	int offset;
	int head;
	int fold;
	int last;

	puts("Enter your text to be folded:");

	while ( (length = myGetlen(line, MAXLINE)) > 0 )
	{
		if(length > FOLD)
		{
			head = last = fold = 0;
			offset = tabLength(line, head, FOLD, TABLENGTH);
			fold = FOLD - offset;
			printf("Offset: %d\n", offset);
			printf("Fold: %d\n", fold);

			/*
			 * Whilst the length of the next fold is as long as FOLD, keep
			 * folding ...
			 */
			while(!last)
			{
				offset = tabLength(line, head, FOLD, TABLENGTH);
				fold = FOLD - offset;
				head = foldLine(line, folded, head, fold, last);
				printf("%s", folded);

				int i = head;
				int count;

				while(line[i++] != '\0')
					count++;

				if(count < FOLD) {
					offset = tabLength(line, head, count, TABLENGTH);
					fold = FOLD - offset;
					if((count - fold) < FOLD) {
						last = 1;
						head = foldLine(line, folded, head, fold, last);
						printf("%s", folded);
					}
				}
			}

		//		if(length + tabLength > FOLD)
		//		{
		//			length = checkLength(totalLength, head);
		//			offset = tabLength(line, head, FOLD, TABLENGTH);
		//			fold = FOLD - offset;
		//			head = foldLine(line, folded, length, fold, head, last);
		//			printf("%s", folded);
		//		}
		//		length = checkLength(totalLength, head);

		//		/*
		//		 * If there is any text left, fold again.
		//		 */
		//		if (length)
		//		{
		//			last = 1;
		//			fold = FOLD - tabLength(line, head, FOLD, TABLENGTH);
		//			head = foldLine(line, folded, length, fold, head, last);
		//			printf("%s", folded);
		//		}
		}
		else 
			printf("%s", line);
	}
	return 0;
}

/*
 * Calculate the combined length of all of the tabs included in the line.
 */
int tabLength(char line[], int head, int toFold, int tablength)
{
	int i;
	int count;

	count = 0;

	for (i = head; i < toFold; i++)
	{
		if (line[i] == '\t')
			count++;
	}
	count = count * tablength;

	return count;
}

/*
 * Calculate the length of the required string, return the lenght FOLD if the
 * remaining string is longer than FOLD, else calculate the lengh of the text
 * that remains, essentialy used at the end of the folding opperation.
 */
int checkLength(int totalLength, int head)
{
	int length;

	length = totalLength - head;

	if (length >= 0 && length < FOLD)
		return length;

	return FOLD;
}

/*
 * Fold once and print folded string to screen, return position of `head` on
 * array.
 */
int foldLine(char line[], char folded[], int head, int fold, int last)
{
	int i = 0;
	int j = 0;
	int startPoint = 0;
	int marker;

/*
 * Set the start point to the current position of the head then set the initial
 * position of the marker to the end of the current desired fold length.
 */
	startPoint = head;
	marker = startPoint + fold;

/* 
 * Use head, as passed into the function, to pick up where the function left
 * off if this is not the first run. Scan over one (more) folds length of
 * text, mark every space line, and file ending; Each new mark overwriting the
 * last, when the iteration reaches the required length, the last recorded mark
 * made is the one that is used to fold the line.
 */
	if (!last)
	{
		while (i < fold)
		{
			if ((line[head] == ' ') || 
					(line[head] == '\t') ||
						(line[head] == '\n') ||
							(line[head] == '\0' ))
			{
				marker = head;
			}
			head++;
			i++;
		}

		/*
		 * Check that the next character was not a space or the end of
		 * a line.
		 */
		if ((line[head+1] == ' ') || 
				(line[head+1] == '\t') ||
					(line[head+1] == '\n') ||
						(line[head+1] == '\0' ))
			marker = head+1;

		/*
		 * Return the head to the starting position ready to read the
		 * array.
		 */
		head = head - i;

		/* 
		 * For the length of the difference between the starting point
		 * of this iteration and the marker, read the first array,
		 * copying its contents into the second.
		 */
		for (j = 0; j < marker-startPoint+1; j++) {
			folded[j] = line[head];
			head++;
		}

		/*
		 * If the marker has not moved create a hyphen break in the
		 * extreamly long word.
		 */
		if (marker == startPoint + fold)
		{
			head = head - 2;
			folded[marker-startPoint-1] = '-';
		}

		/*
		 * Write and end line to the end of the array.
		 */
		folded[marker-startPoint] = '\n';
		folded[marker-startPoint+1] = '\0';
	}
	else
	{
		for (j = 0; j < fold; j++) {
			folded[j] = line[head];
			head++;
		}

		folded[marker-startPoint] = '\0';
		head = marker;
	}

/*
 * Print the transfered line.
 */
	//printf("%s", folded);

/*
 * Return the position of the head so that it can be passed back into the
 * function.
 */
	return head;
}

/*
 * Get string and return its length.
 */
int myGetlen(char line[], int lim)
{
	int i;
	int c;
	int prev;

	for (i = 0; i < lim-1 && !((c = getchar()) == 'q' && prev == ':') && c != '\n'; i++)
	{
		line[i] = c;
		if (i > 0)
			prev = line[i-1];
	}

	if (c == '\n')
		line[i++] = c;

	line[i] = '\0';
	return i;
}

