/*
 * Exercise 1-19. Write a function reverse(s) that reverses the character
 * string s. Use it to write a program that reverses its input a line at a
 * time.
 */
#include <stdio.h>

void fileDimentions(FILE *file, int values[]);
void fillFileMatrix(FILE *file, int dimentions[], char fileMatrix[][]);

int main(int argc, char *argv[])
{
	int i;
	int maxLength;
	int lineCount;
	int fileSize[2] = {0};
	char *ptr[];

	maxLength = lineCount = 0;

	for (i = 1; i < argc; i++) {

		FILE *file = fopen(argv[i], "r");

		if (file == NULL)
			return 1; //Could not open file

		fileDimentions(file, fileSize);
		rewind(file);
		maxLength = fileSize[0];
		lineCount = fileSize[1];
		char fileMatrix[maxLength][lineCount];
		ptr = &fileMatrix;
		rewind(file);
		fillFileMatrix(file, fileSize, *ptr);
	}
}

/*
 * Calculate longest line length and number of lines.
 */
void fileDimentions(FILE *file, int values[])
{
	int c;
	int curentLength;
	int maxLength;
	int lineCount;
	int lastChar;

	curentLength = maxLength = lineCount = 0;

	while ((c = fgetc(file)) != EOF)
	{
		if (c == '\n') {
			if (curentLength > maxLength)
				maxLength = curentLength;
			curentLength = 0;
			lineCount++;
		} else {
			curentLength++;
		}
		lastChar = c;
	}

	if (lastChar != '\n') {
		lineCount++;
		if (curentLength > maxLength)
			maxLength = curentLength;
	}

	values[0] = maxLength;
	values[1] = lineCount;
}

/*
 * Fill multidimentional array with file characters.
 */
void fillFileMatrix(FILE *file, int dimentions[], char fileMatrix[][])
{
	int c;
	int i;
	int charCount;
	int fileLength = dimentions[1];
	char line[dimentions[0]];

	charCount = 0;

	while ((c = fgetc(file)) != EOF)
	{
		for (i = 0; i < fileLength; i++) {

			while ((c != '\n') && (c != '\n'))
			{
				fileMatrix[charCount][i] = c;
				charCount++;
			}
		}
	}
}

