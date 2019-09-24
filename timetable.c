/******************************************************************************
 * DESCRPITPION:
******************************************************************************/

/******************************************************************************
 * HEADER FILES + PREPROCESSING DIRECTIVES
******************************************************************************/
#include <stdio.h> /*printf, scanf*/
#include <string.h> /*strcmp*/
#include "timetable.h" 

#define COURSES_FILE "Courses_Info.txt"
#define STUDENTS_FILE "Student_List.txt"
#define SLOT_NUM 6

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
	char streetName[20];
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
	char name[20] ;
	char password[20] ;
	personal_t personalInfo;
};
typedef struct student student_t;

struct studentNode
{
	student_t stu;
	struct studentNode* nextp;
};
typedef struct studentNode studentNode_t;

struct timeC
{
	int hour, minute;
	char day[4];
};
typedef struct timeC timeC_t;

struct slot
{
	char type[4];
	int group;
	char lecturer[20];
	int building, floor, room;
	timeC_t start;
	timeC_t end;
};
typedef struct slot slot_t;

struct course
{
	int code;
	char name[20];
	char lecturer[20];
	slot_t slot_a[SLOT_NUM];
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
void addStu(studentNode_t** head);
void removeStu(void);
void editStu(void);
void printStu(void);
void printCourse(void);
void printClass(void);
void printEnrollment();
int loadCourse(course_t AllCourses[]);
void getName(student_t* stu);
void getNumber(student_t* stu);
void getBirthday(student_t* stu);
void getAddress(student_t* stu);
void testPrint(studentNode_t* studentListp);
int saveStudentList(studentNode_t* head);
int loadStudentList(studentNode_t** head);
/******************************************************************************
 * MAIN 
 * Author: Victor
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
 * Asks for the user to enter a student number until a valid one is entered
 * Should return student struct
 * Author: Michael
 * IN: The storage location to save the student number
 * OUT: None
******************************************************************************/
void getStuNum(long* stuNump){
	do {printf ("Enter Student Number\n");
		scanf ("%li",stuNump);
		
		if (checkStuNum(*stuNump) != 0)
		{
			printf("Student Number Invalid");	
		}	
		
	}while (checkStuNum(*stuNump) != 0);

}

/******************************************************************************
 * Reads a student number and checks validity
 * input should be student struct
 * Should 
 * Author:
 * IN: None
 * OUT: 0 if successful
******************************************************************************/
int checkStuNum(long stuNum){ 
	return 0;
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

	studentNode_t* studentListp;
	studentListp = NULL;
	
	/* addStu(&studentListp);
	addStu(&studentListp); */

	getStuNum(&stuNum);

	do { printStuMenu(&choice);
        switch (choice){
            case 1: printStuTimetable(stuNum); break;
            case 2: printStuDetails(stuNum); break;
            case 3: saveStudentList(studentListp); break;
            default: printf("Invalid choice\n");} 
    } while (choice != 3);
	return 0;
}

/******************************************************************************
 * Merely a test print function in order to check the linked list
 * Author: Victor
 * IN: pointer to a student struct
 * OUT: None
******************************************************************************/
void testPrint(studentNode_t* head){
	studentNode_t* current = head;
	while (current != NULL){
		printf("%s\n", current->stu.name);
		current = current->nextp;
	}
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
    "Enter Choice (number between 1-3)> ");
    scanf("%d", choicep);
}

/******************************************************************************
 * Prints the students timetable, chronologically per line
 * Assume input to be student struct in which an array of ....
 * Author:
 * IN: Student number for the timetable to be printed
 * OUT: 
******************************************************************************/
void printStuTimetable(int stuNum){
	/* Wouldn't it be better to save a student's timetable in its struct? 
	 		- Victor */
}

/******************************************************************************
 * Prints the students details
 * Assume student struct as input
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
	studentNode_t* studentListp;
	studentListp = NULL;
	
	do { printAdminMenu(&choice);
        switch (choice){
            case 1: addStu(&studentListp); break;
            case 2: removeStu(); break;
            case 3: editStu(); break;
            case 4: printStu(); break;
	    	case 5: printCourse(); break;
	    	case 6: printClass(); break;
	    	case 7: printEnrollment(); break;
	    	case 8: saveStudentList(studentListp); break; /* for later: save edited information */
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
    "Enter Choice (number between 1-8)> ");
    scanf("%d", choicep);
}

/******************************************************************************
 * Function to add students to the list of enrolled students.
 * Author: Victor
 * IN: pointer to a pointer to the first node in the linked list of students
 * OUT: None
******************************************************************************/
void addStu(studentNode_t** head){
	student_t new;
	getName(&new);
	getNumber(&new);
	strncpy(new.password,"default",20);
	getBirthday(&new);
	getAddress(&new);

	if (*head == NULL){
		*head = (studentNode_t*) malloc(sizeof(studentNode_t));
		(*head)->stu=new;
		(*head)->nextp=NULL;
	} else {
		studentNode_t* current = *head;
		while (current->nextp != NULL){
			current = current->nextp;
		}
		current->nextp = (studentNode_t*) malloc(sizeof(studentNode_t));
		current->nextp->stu = new;
		current->nextp->nextp = NULL;
	} 

}

/******************************************************************************
 * Asks for name input and stores it in student struct
 * Author: Victor
 * IN: pointer to a student struct
 * OUT: None
******************************************************************************/
void getName(student_t* stup){
	printf("Enter student name> ");
	scanf("%s", (*stup).name);
}

/******************************************************************************
 * Asks for student number input and stores it in student struct
 * !! Probably needs an intermediate check on validity !!
 * Author: Victor
 * IN: pointer to a student struct
 * OUT: None
******************************************************************************/
void getNumber(student_t* stup){
	printf("Enter student number> ");
	scanf("%d",&(*stup).number);
}

/******************************************************************************
 * Asks for student's birthday input and stores it in student struct
 * !! Probably needs an intermediate check on validity !!
 * Author: Victor
 * IN: pointer to a student struct
 * OUT: None
******************************************************************************/
void getBirthday(student_t* stup){
	printf("Enter birthday consisting of day, month, year, seperated by spaces> ");
	scanf("%d%d%d", &(*stup).personalInfo.birthday.day,
		&(*stup).personalInfo.birthday.month,&(*stup).personalInfo.birthday.year);
}

/******************************************************************************
 * Asks for student's address input and stores it in student struct
 * Needs an intermediate check on validity ?
 * Author: Victor
 * IN: pointer to a student struct
 * OUT: None
******************************************************************************/
void getAddress(student_t* stup){
	printf("Enter address consisting of housenumber and streetname,"
		" seperated by a space> ");
	scanf("%d%s",&(*stup).personalInfo.address.houseNumber,
		(*stup).personalInfo.address.streetName);
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
 * Will change depending on student main functions!
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

/******************************************************************************
 * Load the informations of one course from the txt file
 * Author: Quentin
 * IN: the number of the course that we want to load
 * OUT: The number of courses loaded from the file returned as an int
******************************************************************************/
int loadCourse(course_t AllCourses[])
{
	int i, j;
	char line[10];

    FILE* database = NULL;
    database = fopen(COURSES_FILE, "r");
 
    if (database != NULL)
    {
         
        for(i=0; fgets(line, 10, database) != NULL; i++)
        {
        	/*Scan of the course general infos*/
            fscanf(database, "%d %s - Lecturer: %s",
            	&AllCourses[i].code, AllCourses[i].name, AllCourses[i].lecturer);

            /*Scan of each course slot infos*/
            for(j=0; j < SLOT_NUM; j++)
            {
            	fscanf(database, "%s%d: %s %2d:%2d to %s %2d:%2d - CB%2d.%2d.%3d - %s",
            		AllCourses[i].slot_a[j].type, &AllCourses[i].slot_a[j].group, 
            		AllCourses[i].slot_a[j].start.day, &AllCourses[i].slot_a[j].start.hour,
            		&AllCourses[i].slot_a[j].start.minute, AllCourses[i].slot_a[j].end.day, 
            		&AllCourses[i].slot_a[j].end.hour, &AllCourses[i].slot_a[j].end.minute,
            		&AllCourses[i].slot_a[j].building, &AllCourses[i].slot_a[j].floor, 
            		&AllCourses[i].slot_a[j].room, AllCourses[i].slot_a[j].lecturer);	
            }
        }
 
        fclose(database);
    }
    else
    {
        printf("Read error\n");
    }
    
    return i;
}

/******************************************************************************
 * Saves the the linked list of students to a txt file
 * Author: Victor
 * IN: pointer to the first node in the linked list of students
 * OUT: 0 if succesfull, 1 if unsuccesfull
******************************************************************************/
int saveStudentList(studentNode_t* head){
	FILE* fp = NULL;
    fp = fopen(STUDENTS_FILE, "w");

    if (fp == NULL){
        printf("Write error\n");
        return 1;
    }

    studentNode_t* current = head;

    while (current != NULL){
    	fwrite(&(current->stu),1,sizeof(current->stu),fp);
    	current = current->nextp;
    }

    fclose(fp);

    return 0;

}

/******************************************************************************
 * Loads the linked list of students from a txt file
 * !! NOT FUNCTIONING AT THIS STAGE !!
 * Author: Victor
 * IN: pointer to the pointer to the first node in the linked list of students
 * OUT: 0 if succesfull, 1 if unsuccesfull
******************************************************************************/
int loadStudentList(studentNode_t** head){
	FILE* fp = NULL;
    fp = fopen(STUDENTS_FILE, "r");

    if (fp == NULL){
        printf("Read error\n");
        return 1;
    }

    *head = (studentNode_t*) malloc(sizeof(studentNode_t));
    studentNode_t* current = *head;

    while (fread(&(current->stu), sizeof(current->stu), 1, fp)){
    	current = current -> nextp;
    }

    fclose(fp);
    return 0;

}