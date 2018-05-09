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
	char input[MAXINPUT], command[MAXINPUT];
    char * data = input;
    int offset = 0;

    *data = '\n';
	do {
        if (*data == '\n') {
            fgets(input, MAXINPUT, stdin);
            data = input;
        }

        sscanf(data, "%s%n", command, &offset);
        data = data + offset;

        if (!strcmp(command, "add")) {
            printf("1\n");
        }
        else if (!strcmp(command, "duration")) {
            printf("2\n");
        }
        else if (!strcmp(command, "depend")) {
            printf("3\n");
        }
        else if (!strcmp(command, "remove")) {
            printf("4\n");
        }
        else if (!strcmp(command, "path")) {
            printf("5\n");
        }
        else if (strcmp(command, "exit")) {
            printf("illegal arguments\n");
        }


	
	} while (strcmp(command, "exit"));
	return 0;
}