/******************************************************************************
 * DESCRPITPION:
******************************************************************************/

/******************************************************************************
 * HEADER FILES + PREPROCESSING DIRECTIVES
******************************************************************************/
#include <stdio.h> /*printf, scanf*/
#include <string.h> /*strcmp*/
#include "timetable.h" 

/******************************************************************************
 * STRUCTURES
******************************************************************************/
struct birthday
{
	int year, month, day;
};
typedef struct birthday birthday_t;

struct address
{
	int houseNumber;
	char [20] streetName;
};
typedef struct address address_t;

struct personal
{
	birthday_t birthday;
	address_t address;
	long phoneNum;
};
typedef struct personal personal_t;

struct student
{
	int number;
	char [20] name;
	char [20] password;
	personal_t personalInfo;
};
typedef struct student student_t;

struct time
{
	int day, hour, minute;
};
typedef struct time time_t;

struct slot
{
	char [4] type;
	char [20] lecturer;
	int building, floor, room;
	time_t start;
	time_t end;
};
typedef struct slot slot_t;

struct course
{
	int code;
	char [20] lecturer;
	slot_t slot0, slot1, slot2, slot3, slot4, slot5, slot6;
};
typedef struct course course_t;

/******************************************************************************
 * FUNCTION PROTOTYPES
******************************************************************************/
void getStuNum(long* stuNump);
int checkStuNum(long stuNum);
int studentMain(void);
void printStuMenu(int* choicep);
void printStuTimetable(int stuNum);
void printStuDetails(int stuNum);
int adminMain(void);
void printAdminMenu(int* choicep);
void addStu(void);
void removeStu(void);
void editStu(void);
void printStu(void);
void printCourse(void);
void printClass(void);
void printEnrollment();
/******************************************************************************
 * MAIN 
******************************************************************************/
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
 * Asks for the user to enter a student number until a vaild one is entered
 * Author: Michael
 * IN: The storage location to save the student number
 * OUT: None
******************************************************************************/
void getStuNum(long* stuNump){
	do {printf ("Enter Student Number\n");
		scanf ("%li",&stuNum);
		
		if (checkStuNum(stuNum) != 0)
		{
			printf("Student Number Invaild");	
		}	
		
	}while (checkStuNum(stuNum) != 0);

}

/******************************************************************************
 * Reads a student number and checks validity
 * Author:
 * IN: None
 * OUT: 0 if successful
******************************************************************************/
int checkStuNum(long stuNum){ 

}


/******************************************************************************
 * Student main: menu options
 * Author: Michael, Victor
 * IN: None
 * OUT: 0 if successful
******************************************************************************/
int studentMain(void){
	int choice;
	long stuNum;
	
	getStuNum(&stuNum);

	do { printStuMenu(&choice);
        switch (choice){
            case 1: printStuTimetable(stuNum) break;
            case 2: printStuDetails(stuNum) break;
            case 3: break;
            default: printf("Invalid choice\n");} 
    } while (choice != 3);
	return 0;
}

/******************************************************************************
 * Prints student menu options
 * Author: Victor
 * IN: Pointer to 'choice' storage location
 * OUT: None
******************************************************************************/
void printStuMenu(int* choicep){
	printf("\n"
    "1. View Timetable\n"
    "2. View Personal Details\n"
    "3. Exit Program\n"
    "Enter Choice (number between 1-3)>\n");
    scanf("%d", choicep);
}

/******************************************************************************
 * Prints the students timetable, chronologically per line
 * Author:
 * IN: Student number for the timetable to be printed
 * OUT: 
******************************************************************************/
void printStuTimetable(int stuNum){
	
}

/******************************************************************************
 * Prints the students details
 * Author:
 * IN: The student number for the details to be printed
 * OUT: None
******************************************************************************/
void printStuDetails(int stuNum){
	
}

/******************************************************************************
 * Admin main: menu options
 * Author: Michael, Victor
 * IN: None
 * OUT: 0 if successful
******************************************************************************/
int adminMain(void){
	int choice;
	
	do { printAdminMenu(&choice);
        switch (choice){
            case 1: void addStu(void) break;
            case 2: void removeStu(void) break;
            case 3: void editStu(void) break;
            case 4: void printStu(void) break;
	    case 5: void printCourse(void) break;
	    case 6: void printClass(void) break;
	    case 7: void printEnrollment() break;
	    case 8: break;
            default: printf("Invalid choice\n");} 
    } while (choice != 8);
	return 0;
}


/******************************************************************************
 * Prints admin menu options 
 * Author: Michael, Victor
 * IN: Pointer to 'choice' storage location
 * OUT: None
******************************************************************************/
void printAdminMenu(int* choicep){
    printf("\n"
    "1. Add Student\n"
    "2. Remove Student\n"
    "3. Edit Student\n"
    "4. View Student Timetable and Details\n"
    "5. Show Course Information\n"
    "6. Show Class information\n"
    "7. Show all enrolled students\n"
    "8. Exit Program\n"
    "Enter Choice (number between 1-8)>\n");
    scanf("%d", choicep);
}

/******************************************************************************
 * Add student function
 * Author:
 * IN: None
 * OUT: None
******************************************************************************/
void addStu(void){
	
}

/******************************************************************************
 * Remove student function
 * Author:
 * IN: None
 * OUT: None
******************************************************************************/
void removeStu(void){
	
}

/******************************************************************************
 * Edit student details function
 * Author:
 * IN: None
 * OUT: None
******************************************************************************/
void editStu(void){
	
}

/******************************************************************************
 * Prints a students details followed by their timetable
 * Author: Michael
 * IN: None
 * OUT: None
******************************************************************************/
void printStu(void){
	long stuNum;
	getStuNum(&stuNum);
	printStuDetails(stuNum);
	printf("\n");
	printStuTimetable(stuNum);
}

/******************************************************************************
 * Prints all courses' lectures and tutorials with amount of students
 * Author:
 * IN: None
 * OUT: None
******************************************************************************/
void printCourse(void){
	
}

/******************************************************************************
 * Prints the names and number of students enrolled in a given Lab/Lec/Tut 
 * Author:
 * IN: None
 * OUT: None
******************************************************************************/
void printClass(void){
	
}
/******************************************************************************
 * Prints the number of students enrolled and a list of their student numbers
 * Author:
 * IN: None
 * OUT: None
******************************************************************************/
void printEnrollment(void){
	
}
