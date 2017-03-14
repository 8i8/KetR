/*
 * Exercise 1-22. Write a program to ``fold'' long input lines into two or more
 * shorter lines after the last non-blank character that occurs before the n-th
 * column of input. Make sure your program does something intelligent with very
 * long lines, and if there are no blanks or tabs before the specified column.
 *
 * TODO This code is unfinished the tab folding needs to be worked on, I think
 * that this could be an interesting problem to write a unit test for.
 */
#include <stdio.h>

#define MAXLINE		1000
#define FOLD		20
#define TABWIDTH	8

int checkLength(int length, int head);
int checkTabs(char line[], int length, int head);
int foldLine(char line[], char folded[], int length, int tabOffset, int head);
int myGetlen(char line[], int lim);

int main(void) {

	char line[MAXLINE];
	char folded[FOLD+1];
	int totalLength;
	int length;
	int tabOffset;
	int head;

	while ( (totalLength = myGetlen(line, MAXLINE)) > 0 )
	{
		length = FOLD;
		head = 0;

		/*
		 * Check the quantity of charaters that are tabs to create an
		 * offset for length calcultions, to allow for these tabs.
		 */
		tabOffset = checkTabs(line, length, head);

		if ((totalLength + tabOffset) > FOLD)
		{
			/*
			 * Whilst the lenght of the fold is as long as a fold,
			 * keep folding ...
			 */
			while (length == FOLD)
			{
				tabOffset = checkTabs(line, length, head);
				head = foldLine(line, folded, length, tabOffset, head);
				length = checkLength(totalLength, head);
			}

			while (length > 0 && length < FOLD)
			{
				tabOffset = checkTabs(line, length, head);
				/*
				 * If there is any text left, fold again.
				 */
				if (length)
					head = foldLine(line, folded, length, tabOffset, head);

				length = checkLength(totalLength, head);
			}
		}
		else 
			printf("%s", line);
	}
	return 0;
}

/*
 * Calculate the length of the required string, give back the fold lenght if
 * the remaining string is longer, else calculate the lengh of the text that
 * remains, essential used at the end of the folding opperation.
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
 * Count the numbere of tabs in the given section of string and return length
 * corrected for that.
 */
int checkTabs(char line[], int length, int head)
{
	int i = 0;
	int tabCount = 0;
	int tabOffset;

	while (i++ < length)
	{
		if(line[head+i] == '\t')
			tabCount++;
	}

	/*
	 * TODO Stop this function and alter the length being used if the
	 * offset rises to abouve the fold length, this can occure when many
	 * tabs are found together in a line.
	 */

	tabOffset = tabCount * TABWIDTH;

	return tabOffset;
}

/*
 * Fold once and print folded string to screen, return position of `head` on
 * array.
 */
int foldLine(char line[], char folded[], int length, int tabOffset, int head)
{
	int i = 0;
	int j = 0;
	int startPoint = 0;
	int marker;
	int len = length - tabOffset;

	/*
	 * TODO there is a problem in the founctioning of this code when
	 * multiple tabs are included in one line and the tab offset is greater
	 * than the length.
	 */

	if(len < 0)
		len = 0;

/*
 * Set the start point to the current position of the head then set the initial
 * position of the marker to the end of the current desired fold length.
 */
	startPoint = head;
	marker = startPoint + len;

	/* 
	 * Use head as passed into the function to pick up where the function
	 * left off, if this is not the first run. Scan over one (more) folds
	 * length of text, mark every space line, and file ending; Each new
	 * mark overwriting the last, when the iteration reaches the required
	 * length, the last recorded mark made is the one that is used to fold
	 * the line.
	 */
	if (length == FOLD)
	{
		while (i < len)
		{
			if ((line[head] == ' ') || (line[head] == '\n') ||
					(line[head] == '\t') || (line[head] == '\0' ))
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
		if ((line[head+1] == ' ') || (line[head+1] == '\n') ||
				(line[head+1] == '\t') || (line[head+1] == '\0' ))
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
		if (marker == startPoint + length)
		{
			head = head - 2;
			folded[marker-startPoint-1] = '-';
		}

		// Write and end line to the end of the array.
		folded[marker-startPoint] = '\n';
		folded[marker-startPoint+1] = '\0';
	}
	else
	{
		for (j = 0; j < len; j++) {
			folded[j] = line[head];
			head++;
		}

		folded[marker-startPoint] = '\0';
		head = marker;
	}

	// Print the transfered line.
	printf("%s", folded);

	/*
	 * Return the position of the head so that it can be passed back into
	 * the function.
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

	for (i = 0; i < lim-2 && !((c = getchar()) == 'q' && prev == ':') && c != '\n'; i++)
	{
		line[i] = c;
		if (i > 0)
			prev = line[i-1];
	}

	if (c == '\n') {
		line[i] = c;
		++i;
	}
	line[i] = '\0';
	return i;
}

