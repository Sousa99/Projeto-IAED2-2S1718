#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Define Constants */
#define MAXINPUT 100000
#define MAXCOMMAND 9
#define MAXDESCRIPTION 8001
#define MAXDEPENDENCIES 10

/* Define Structs */
typedef struct task {
    unsigned long id, duration;
    struct task *dependencies[MAXDEPENDENCIES];
    char description[MAXDESCRIPTION];

} Task;

/* Global Variables */


/* ---------- Complementary Functions ---------- */



/* ---------- Asked Functions ---------- */



/* ---------- Main ---------- */
int main(int argc, char **argv) {
	char input[MAXINPUT];
    char *data;

	do {
		fgets(input, MAXINPUT, stdin);
        data = input;
        



	
	} while (strcmp(command, "exit"));
	return 0;
}