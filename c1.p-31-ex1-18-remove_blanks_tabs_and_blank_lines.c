/*
 * Exercise 1-18. Write a program to remove trailing blanks and tabs from each
 * line of input, and to delete entirely blank lines.
 */
#include <stdio.h>

int fileLength(FILE *file);
void fillArray(FILE *file, char array[]);
void editArray(char array[], char arrayNew[], int len);
void tabsSpacesEmptyLines(char arrayTemp[], char arrayNew[], int len, int offset);
void emptyLinesAtStart(char arrayTemp[], char arrayNew[], int len, int offset);
void copyArray(char arrayOld[], char arrayNew[], int len, int offset);
void writeEndOfString(char array[], int len, int endOfString);
void printFileArray(char array[], int len);
void initArray(char array[], int len);

int main(int argc, char *argv[])
{
	int i, j, len;

	for (i = 1; i < argc; i++) {

		FILE *file = fopen(argv[i], "r");

		if (file == NULL)
			return 1; /*Could not open file*/

		len = fileLength(file);
		rewind(file);
		char fileArray[len];
		char fileCleared[len];
		fillArray(file, fileArray);
		editArray(fileArray,fileCleared, len);
		printFileArray(fileCleared, len);
		fclose(file);
	}
}

/*
 * Return the lenth of a text file in characters
 */
int fileLength(FILE *file)
{
	int c, count;
	count = 0;

	while ((c = fgetc(file)) != EOF)
		count++;

	return count;
}

/*
 * Fill an array from a text file
 */
void fillArray(FILE *file, char fileArray[])
{
	int i, c;
	i = 0;

	while ((c = fgetc(file)) != EOF)
	{
		fileArray[i] = c;
		i++;
	}
}

/*
 * Create working array and call edit functions.
 */
void editArray(char arrayOld[], char arrayNew[], int len)
{
	int offset;
	char arrayTemp[len];
	offset = 0;
	copyArray(arrayTemp, arrayOld, len, offset);

	tabsSpacesEmptyLines(arrayTemp, arrayNew, len, offset);
	emptyLinesAtStart(arrayTemp, arrayNew, len, offset);
}

/*
 * Remove all trailing spaces and tabs from each line
 */
void tabsSpacesEmptyLines(char arrayTemp[], char arrayNew[], int len, int offset)
{
	int i;
	int diff;
	int endOfString;
	diff = -1;
	endOfString = 0;

	while (diff != 0)
	{
		diff = 0;

		for (i = 0; i < len; i++)
		{
			if ((arrayTemp[i] == '\n') && (arrayTemp[i-1] == ' ')) {
				diff++;
				endOfString++;
			} else if ((arrayTemp[i] == '\n') && (arrayTemp[i-1] == '\t')) {
				diff++;
				endOfString++;
			} else if ((arrayTemp[i] == '\n') && (arrayTemp[i-1] == '\n')) {
				diff++;
				endOfString++;
			}
			arrayNew[i-diff] = arrayTemp[i];
		}
		copyArray(arrayTemp, arrayNew, len, offset);
	}
	writeEndOfString(arrayNew, len, endOfString);
}

/*
 * Delete empty lines from the start of the file if present.
 */
void emptyLinesAtStart(char arrayTemp[], char arrayNew[], int len, int offset)
{
	int i;
	int diff;
	int endOfString;
	diff = -1;
	endOfString = 0;

	while (diff != 0)
	{
		diff = 0;
		offset = 1;
		i = 0;

		if (arrayNew[i] == '\n') {
			arrayTemp = arrayNew;
			copyArray(arrayNew, arrayTemp, len, offset);
		} else 
			diff = 0;
	}
	writeEndOfString(arrayNew, len, endOfString);
}

/*
 * Copy one array into another, the offset variable can be used to create an
 * offset between the arrays, usefull here for removing empty lines at the
 * start of the file.
 */
void copyArray(char arrayNew[], char arrayOld[], int len, int offset)
{
	int i;

	for (i = 0; i < len-offset; i++)
	{
		arrayNew[i] = arrayOld[i+offset];
	}
	writeEndOfString(arrayNew, len, offset);
}

/*
 * Write end of string character '\0' into all remaining spaces at the end of
 * array.
 */
void writeEndOfString(char array[], int len, int endOfString)
{
	int i;

	for (i = 0; i < endOfString; i++)
		array[len - i] = '\0';
}

/*
 * Echo array to terminal.
 */
void printFileArray(char array[], int len)
{
	int i;
	i = 0;

	for (i = 0; i < len; i++)
	{
		printf("%c", array[i]);
	}
}
