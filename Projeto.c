#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Define Constants */
#define MAXINPUT 1000
#define MAXCOMMAND 9
#define MAXDESCRIPTION 8001
#define MAXDEPENDENCIES 10

/* Define Structs */
typedef struct {
    unsigned long id, duration;
    unsigned long dependencies[MAXDEPENDENCIES];
    char description[MAXDESCRIPTION];

} Task;

/* Global Variables */


/* ---------- Complementary Functions ---------- */



/* ---------- Asked Functions ---------- */



/* ---------- Main ---------- */
int main(int argc, char **argv) {
	char input[MAXINPUT];
    char command[MAXCOMMAND];

	do {
		input[0] = ' ';

		fgets(input, MAXINPUT, stdin);
        printf("Input: %s", input);
		sscanf(input, "%s", command);
        printf("Command: %s\n", command);
        printf("Input: %s", input);

	
	} while (strcmp(command, "exit"));
	return 0;
}