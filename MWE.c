#include <stdio.h>
#include <stdlib.h>

FILE *fp;
long int fppos;

void openfile(void)
{
	if((fp = fopen("/var/og/syslog","r")) == NULL)
	{
		printf("Error: Opening file failed\n");
		exit(EXIT_FAILURE);
	}

	// Initialize file pointer position variable
	fppos = ftell(fp);
}

int lookforchanges(void)
{
	// Ask for the current position
	long int curpos = ftell(fp);

	// Seek to the end of the file
	fseek(fp, 0L, SEEK_END);

	// Ask for the end position
	long int pos = ftell(fp);

	// Go back to to previous position
	fseek(fp, curpos, SEEK_SET);

	// Return difference
	// value == 0 -> no change
	// value  < 0 -> File has shrunk (e.g. logrotate)
	// value  > 0 -> File has grown (new content)
	return (pos-fppos);
}

int main (int argc, char* argv[])
{

	// Try to open file for reading
	openfile();

	return EXIT_SUCCESS;
}
