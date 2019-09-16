#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

int studentMain(void);
int adminMain(void);
void printStudentMenu(int* choicep);
void printAdminMenu(int* choicep);

int main(int argc, char *argv[]) {
	if (argc <= 1){
		printf("Specify runtime mode\n");
		return 1;
	}
	if (strcmp(argv[1],	"-a") == 0){
		adminMain();
		return 0;
	} else if (strcmp(argv[1], "-s") == 0){
		studentMain();
		return 0;
	} else {
		printf("Invalid input\n");
		return 1;
	}
	return 0;
}

/******************************************************************************
 * Student main: menu options
******************************************************************************/
int studentMain(void){
	int choice;

	/* Verify Student ID */

	do { printStudentMenu(&choice);
        switch (choice){
            case 1: /* Timetable Printing Function */  break;
            case 2: /* Personal Details Function */ break;
            case 3: break;
            default: printf("Invalid choice\n");} 
    } while (choice != 3);
	return 0;
}

/******************************************************************************
 * Prints student menu options
******************************************************************************/
void printStudentMenu(int* choicep){
	printf("\n"
    "1. View Timetable\n"
    "2. View Personal Details\n"
    "3. Exit Program\n"
    "Enter Choice (number between 1-3)>\n");
    scanf("%d", choicep);
}

/******************************************************************************
 * Read student number, password and check validity. maybe three seperate 
 * functions?
******************************************************************************/

/******************************************************************************
 * Timetable view function: prints the students timetable 
 * chronologically per line
******************************************************************************/

/******************************************************************************
 * Personal details function: prints the students details. Maybe give the
 * the student an option to edit his details? This is usually possible in
 * similar real world implementations
******************************************************************************/

/******************************************************************************
 * Admin main: menu options
******************************************************************************/
int adminMain(void){
	int choice;
	
	do { printStudentMenu(&choice);
        switch (choice){
            case 1: /* Timetable Editing Function */  break;
            case 2: /* Course Information Function */ break;
            case 3: /* Show student list function */ break;
            case 4: break;
            default: printf("Invalid choice\n");} 
    } while (choice != 4);
	return 0;
}

/******************************************************************************
 * Prints admin menu options 
******************************************************************************/
void printAdminMenu(int* choicep){
	printf("\n"
    "1. Edit Timetable\n"
    "2. Show Course Information\n"
    "3. Show all enrolled students"
    "4. Exit Program\n"
    "Enter Choice (number between 1-3)>\n");
    scanf("%d", choicep);
}

/******************************************************************************
 * Timetable edit function: This will be a big function, try to keep it modular
 * Swap, add or remove students from course timetable. ... More?
******************************************************************************/

/******************************************************************************
 * View Course Information function: prints all courses' lectures and
 * tutorials with amount of students. Possibly add option to select a course to
 * view its members?
******************************************************************************/

/******************************************************************************
 * View all enrolled students function: prints list of all students enrolled.
 * Possibly add option to select a student to view all its enrolled courses?
******************************************************************************/
