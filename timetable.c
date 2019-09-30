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
#define MAX_ENROLLMENT 4
#define MAX_COURSES_AMT 20

/******************************************************************************
 * STRUCTURES
******************************************************************************/


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
	int StudentAMT;
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

struct studentCourse
{
	int LecGrp, LabGrp, TutGrp;
	course_t course;
};
typedef struct studentCourse studentCourse_t;

struct timetable
{
	int coursesAMT;
	studentCourse_t enrolledCRSE[MAX_ENROLLMENT];
};
typedef struct timetable timetable_t;

struct student
{
	int number;
	char firstname[20];
	char lastname[20];
	char password[20] ;
	personal_t personalInfo;
	timetable_t enrollments;
};
typedef struct student student_t;

struct studentNode
{
	student_t stu;
	struct studentNode* nextp;
};
typedef struct studentNode studentNode_t;

/******************************************************************************
 * FUNCTION PROTOTYPES
******************************************************************************/
student_t getStu(studentNode_t* head);
int checkStuNum(int stuNum, studentNode_t* head, student_t* match);
void correctPassword(student_t student);
int studentMain(void);
void printStuMenu(int* choicep);
void printStuTimetable(student_t student);
void printStuDetails(student_t student);
int adminMain(void);
void printAdminMenu(int* choicep);
void addStu(studentNode_t** head);
void removeStu(void);
void editStu(void);
void printStu(studentNode_t* head);
void printCourse(course_t AllCourses[], int coursesAMT);
int validCourseNum(int courseNum);
int validClassName(char className[]);
int validGroupNum(int groupNum);
void chooseClass(int* courseNum, char classType[], int* group);
int checkEnrollment(student_t stu, int courseNum, char classType[], int group);
void printClass(studentNode_t* head);
void printEnrollment();
int loadCourse(course_t AllCourses[], int* CoursesAMT);
int saveClasses(course_t AllCourses[], int CoursesAMT);
int validLocation(int building, int floor, int room);
int validTime(char day[], int hour, int minute);
int addCourse(course_t AllCourses[], int* CoursesAMT);
void getName(student_t* stu);
void getNumber(student_t* stu);
void getBirthday(student_t* stu);
void getAddress(student_t* stu);
void testPrint(studentNode_t* studentListp);
int saveStudentList(studentNode_t* head);
int loadStudentList(studentNode_t** head);
void getPhoneNumber(student_t* stup);
int validStuNum(int number);
int validDate(int day, int month, int year);
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
 * Author: Michael, Victor
 * IN: The storage location to save the student number
 * OUT: student corresponding to ID input
******************************************************************************/
student_t getStu(studentNode_t* head){
	int stuNum;
	student_t match;

	do {printf ("Enter Student Number> ");
		scanf ("%d", &stuNum);
		
		if (!checkStuNum(stuNum, head, &match))	{
			printf("Student Number Invalid\n");	
		}	
	}while (!checkStuNum(stuNum, head, &match));

	return match;
}

/******************************************************************************
 * Checks if student number exists in the list of students
 * !! Has an error, not working exactly as intended. While-loop continues when
 * when if statement is satisfied !!
 * Author: Victor
 * IN: student number, head of linked list and pointer to student_t
 * OUT: 1 if successful, 0 if unsuccesful
******************************************************************************/
int checkStuNum(int stuNum, studentNode_t* head, student_t* match){ 
	studentNode_t* current = head;
	while (current != NULL){
		if (stuNum == current->stu.number) {
			*match = current->stu;
			printf("ID Match found!\n"); /* For debug purposes */
			return 1;
		}
		printf("test\n");
		current = current->nextp;
	}
	return 0;
}

/******************************************************************************
 * Asks user for password and checks if the password is correct
 * !! Needs to be added: if password is equal to "default", the user should be
 * able to enter a new one !!
 * Author: Victor
 * IN: student for whom the password is to be asked and checked
 * OUT: none
******************************************************************************/
void correctPassword(student_t student){
	char pw[20];
	do {printf ("Enter password> ");
		scanf ("%s", pw);
		if (strcmp(student.password, pw)!=0)
		{
			printf("Password incorrect!\n");	
		}	
	}while (strcmp(student.password, pw)!=0);
	printf("Password correct!\n");
}



/******************************************************************************
 * Student main: menu options
 * Author: Michael, Victor
 * IN: None
 * OUT: 0 if successful
******************************************************************************/
int studentMain(void){
	int choice;

	studentNode_t* studentListp;
	studentListp = NULL;
	
	addStu(&studentListp);
	/*addStu(&studentListp);*/

	student_t currentStu = getStu(studentListp);
	/* correctPassword(currentStu); */

	do { printStuMenu(&choice);
        switch (choice){
            case 1: printStuTimetable(currentStu); break;
            case 2: printStuDetails(currentStu); break;
            case 3: saveStudentList(studentListp); break;
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
    "Enter Choice (number between 1-3)> ");
    scanf("%d", choicep);
}

/******************************************************************************
 * Prints the students timetable, chronologically per line
 * Assume input to be student struct in which an array of timetable structs is
 * Author:
 * IN: Student number for the timetable to be printed
 * OUT: 
******************************************************************************/
void printStuTimetable(student_t student){
}

/******************************************************************************
 * Prints the students details
 * Author: Victor
 * IN: The student for which the details need to be printed
 * OUT: None
******************************************************************************/
void printStuDetails(student_t student){
	printf("\nName: %s, %s\nStudent ID: %8d\nBirthday: %02d/%02d/%4d\n"
		"Address: %d %s\nPhone number: %10ld\n", student.lastname, 
		student.firstname, student.number, student.personalInfo.birthday.day, 
		student.personalInfo.birthday.month, 
		student.personalInfo.birthday.year, 
		student.personalInfo.address.houseNumber,
		student.personalInfo.address.streetName,
		student.personalInfo.phoneNum);
}

/******************************************************************************
 * Admin main: menu options
 * Author: Michael, Victor, Quentin
 * IN: None
 * OUT: 0 if successful
******************************************************************************/
int adminMain(void){
	int choice;
	course_t AllCourses[MAX_COURSES_AMT];
	int CoursesAMT = 0;
	studentNode_t* studentListp;
	studentListp = NULL;
	loadCourse(AllCourses, &CoursesAMT);

	do { printAdminMenu(&choice);
        switch (choice){
            case 1: addStu(&studentListp); break;
            case 2: removeStu(); break;
            case 3: editStu(); break;
            case 4: printStu(studentListp); break;
	    	case 5: printCourse(AllCourses, CoursesAMT); break;
	    	case 6: printClass(studentListp); break;
	    	case 7: printEnrollment(); break;
	    	case 8: addCourse(AllCourses, &CoursesAMT); break;
	    	case 9: saveStudentList(studentListp); break; /* for later: save edited information */
            default: printf("Invalid choice\n");} 
    } while (choice != 9);
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
    "8. Add a course\n"
    "9. Exit Program\n"
    "Enter Choice (number between 1-9)> ");
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
	getPhoneNumber(&new);

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
	printf("Enter first and last name of student, seperated by a space> ");
	scanf("%s%s", (*stup).firstname, (*stup).lastname);
}

/******************************************************************************
 * Asks for student number input and stores it in student struct
 * Author: Victor
 * IN: pointer to a student struct
 * OUT: None
******************************************************************************/
void getNumber(student_t* stup){
	int temp;
	do {
		printf("Enter student number> ");
		scanf("%d", &temp);

		if (!validStuNum(temp)){
			printf("Invalid student number.\n");
		}

	} while (!validStuNum(temp));
	
	(*stup).number = temp;
}

/******************************************************************************
 * Checks if student number input is valid
 * -> 8 digit student ID is what we want right?
 * Author: Victor
 * IN: integer of the student number
 * OUT: 1 if valid, 0 if invalid
******************************************************************************/
int validStuNum(int number){
	return (number >= 10000000 && number <= 99999999);
}

/******************************************************************************
 * Asks for student's birthday input and stores it in student struct
 * Author: Victor
 * IN: pointer to a student struct
 * OUT: None
******************************************************************************/
void getBirthday(student_t* stup){
	int day, month, year;

	do {
		printf("Enter birthday consisting of day, month, year,"
			" seperated by spaces> ");
		scanf("%d%d%d", &day, &month, &year);

		if(!validDate(day, month, year)){
			printf("Invalid date.");
		}

	} while (!validDate(day, month, year));
	
	(*stup).personalInfo.birthday.day = day;
	(*stup).personalInfo.birthday.month = month;
	(*stup).personalInfo.birthday.year = year;
}

/******************************************************************************
 * Checks if the date information put in is valid
 * Author: Victor
 * IN: integers for day, month and year
 * OUT: 1 if valid, 0 if invalid
******************************************************************************/
int validDate(int day, int month, int year){
	return (day >= 1 && day <= 31) && (month >= 1 && month <= 12) &&
	(year >= 1900 && year <= 2018);
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
 * Asks for student's phone number input and stores it in student struct
 * How do we want to check if the number is valid?
 * Author: Victor
 * IN: pointer to a student struct
 * OUT: None
******************************************************************************/
void getPhoneNumber(student_t* stup){
	printf("Enter phone number> ");
	scanf("%ld",&(*stup).personalInfo.phoneNum);
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
 * Author: Michael, Victor
 * IN: pointer to the first node of the student linked list
 * OUT: None
******************************************************************************/
void printStu(studentNode_t* head){
	student_t student = getStu(head);
	printStuDetails(student);
	printStuTimetable(student);
}

/******************************************************************************
 * Prints all courses' lectures and tutorials with amount of students
 * Author: Quentin
 * IN: The array in which the courses are stored, the amount of courses stored
 * OUT: None
******************************************************************************/
void printCourse(course_t AllCourses[], int coursesAMT)
{
	int i, j;
	for(i=0; i<coursesAMT; i++)
	{
		printf("%03d %s - Lecturer: %s\n",
			AllCourses[i].code, AllCourses[i].name, 
			AllCourses[i].lecturer);
		for(j=0; j<SLOT_NUM; j++)
		{
			printf("%-3s%d amount of student enrolled: %d\n",
				AllCourses[i].slot_a[j].type, 
				AllCourses[i].slot_a[j].group,
				AllCourses[i].slot_a[j].StudentAMT);
		}
		printf("\n");
	}
}

/******************************************************************************
 * Check if a course number is valid 
 * Author: Quentin, Victor
 * IN: Course number
 * OUT: 1 if valid, 0 if unvalid
******************************************************************************/
int validCourseNum(int courseNum){
	return courseNum>=100 && courseNum<=200;
}

/******************************************************************************
 * Check if a class is valid 
 * Author: Quentin, Victor
 * IN: class name
 * OUT: 1 if valid, 0 if unvalid
******************************************************************************/
int validClassName(char className[]){	
	return strcmp(className, "Lab") == 0 || strcmp(className, "Lec") == 0 || 
		strcmp(className, "Tut") == 0;
}

/******************************************************************************
 * Check if a group number is valid 
 * Author: Quentin, Victor
 * IN: group number
 * OUT: 1 if valid, 0 if unvalid
******************************************************************************/
int validGroupNum(int groupNum){
	return groupNum>=1 && groupNum<=2;
}

/******************************************************************************
 * Asks the user to choose a class 
 * Author: Quentin, Victor
 * IN: a pointer to the number of a course, the class type as a string,
 * a pointer to the group number
 * OUT: None
******************************************************************************/
void chooseClass(int* courseNum, char classType[], int* group)
{
	*courseNum = 0;
	*group = 0;
	int validInput = 1;

	while(validInput == 1)
	{
		printf("Enter the number of the course: ");
		scanf("%d", courseNum);
		if(!validCourseNum(*courseNum))
		{
			printf("Invalid input\n");
		}
		else
		{
			validInput = 0;
		}
	}
	validInput = 1;
	while(validInput == 1)
	{
		printf("Enter the class and the group seperated by a space "
			"(Lab 1, Tut 2, Lec 1...): ");
		scanf("%s %d", classType, group);
		if(!validClassName(classType) || !validGroupNum(*group))
		{
			printf("Invalid input\n");
		}
		else
		{
			validInput = 0;
		}
	}
	
}

/******************************************************************************
 * Check if a student is enrolled in a particular class 
 * Author: Quentin
 * IN: the student, the class to check (number of the course, the class type
 * and the group number) 
 * OUT: 0 if enrolled, 1 if not
******************************************************************************/
int checkEnrollment(student_t stu, int courseNum, char classType[], int group)
{
	int i;
	for(i=0; i < stu.enrollments.coursesAMT; i++)
	{
		if(stu.enrollments.enrolledCRSE[i].course.code == courseNum)
		{
			if(strcmp(classType, "Lab") == 0 && 
				stu.enrollments.enrolledCRSE[i].LabGrp == group)
			{
				return 0;
			}
			else if(strcmp(classType, "Lec") == 0 && 
				stu.enrollments.enrolledCRSE[i].LecGrp == group)
			{
				return 0;
			}
			else if(strcmp(classType, "Tut") == 0 && 
				stu.enrollments.enrolledCRSE[i].TutGrp == group)
			{
				return 0;
			}
		}
	}
	
	return 1;
}

/******************************************************************************
 * Prints the names and number of students enrolled in a given Lab/Lec/Tut 
 * Author: Quentin
 * IN: pointer to the first node in the linked list of students
 * OUT: None
******************************************************************************/
void printClass(studentNode_t* head)
{
	int courseNum, group; 
	char classType[4];
	int newLine = 0;

	chooseClass(&courseNum, classType, &group);

	studentNode_t* current = head;

    while (current != NULL)
    {
    	if(checkEnrollment(current->stu, courseNum, classType, group) == 0)
    	{
    		printf("%d %s\t", current->stu.number, current->stu.lastname);
    		newLine++;
    	}
    	current = current->nextp;
    	if(newLine == 4)
    	{
    		printf("\n");
    		newLine = 0;
    	}
    }
}


/******************************************************************************
 * Prints the number of students enrolled and a list of their student numbers
 * Author: Quentin
 * IN: pointer to the first node in the linked list of students
 * OUT: None
******************************************************************************/
void printEnrollment(studentNode_t* head)
{
	int newLine = 0;
	int enrolledNum = 0;
	studentNode_t* current = head;

    while (current != NULL)
    {
    	if(current->stu.enrollments.coursesAMT > 0)
    	{
    		printf("%d\t", current->stu.number);
    		newLine++;
    		enrolledNum++;
    	}
    	current = current->nextp;
    	if(newLine == 6)
    	{
    		printf("\n");
    		newLine = 0;
    	}
    }
    printf("\n%d students are enrolled in at least one course\n", enrolledNum);
}

/******************************************************************************
 * Loads the informations of one course from the txt file
 * Author: Quentin
 * IN: the array where we are storing the courses, a pointer to the amount of
 * courses in the array
 * OUT: 0 if succesfull, 1 if unsuccesfull
******************************************************************************/
int loadCourse(course_t AllCourses[], int* CoursesAMT)
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
            	fscanf(database, "%s %d: %s %2d:%2d to %s %2d:%2d - CB%2d.%2d.%3d - %s",
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
        return 1;
    }
    
    *CoursesAMT = i;
    return 0;
}

/******************************************************************************
 * Saves the classes to the file 
 * Author: Quentin
 * IN: the array where we are storing the courses, a pointer to the amount of
 * courses in the array
 * OUT: 0 if succesfull, 1 if unsuccesfull
******************************************************************************/
int saveClasses(course_t AllCourses[], int CoursesAMT)
{
	int i, j;
	FILE* database = NULL;
    database = fopen(COURSES_FILE, "w");

    if (database != NULL)
    {
         
        for(i=0; i < CoursesAMT; i++)
        {
        	fprintf(database, "\n ");
        	/*Saves the course general infos*/
            fprintf(database, "\n%d %s - Lecturer: %s",
            	AllCourses[i].code, AllCourses[i].name, AllCourses[i].lecturer);

            /*Saves each course slot infos*/
            for(j=0; j < SLOT_NUM; j++)
            {
            	fprintf(database, "\n%s %d: %s %02d:%02d to %s %02d:%02d - CB%02d.%02d.%03d - %s",
            		AllCourses[i].slot_a[j].type, AllCourses[i].slot_a[j].group, 
            		AllCourses[i].slot_a[j].start.day, AllCourses[i].slot_a[j].start.hour,
            		AllCourses[i].slot_a[j].start.minute, AllCourses[i].slot_a[j].end.day, 
            		AllCourses[i].slot_a[j].end.hour, AllCourses[i].slot_a[j].end.minute,
            		AllCourses[i].slot_a[j].building, AllCourses[i].slot_a[j].floor, 
            		AllCourses[i].slot_a[j].room, AllCourses[i].slot_a[j].lecturer);	
            }
        }
    }
    else
    {
        printf("Write error\n");
        return 1;
    }

    fclose(database);

    return 0;
}

/******************************************************************************
 * Check if a location is valid 
 * Author: Quentin
 * IN: building, floor, room
 * OUT: 0 if valid, 1 if unvalid
******************************************************************************/
int validLocation(int building, int floor, int room)
{
	if(building >= 0 && building <= 20 && floor >= 0 && floor <= 30 && 
		room >= 0 && room <= 600)
	{
		return 0;
	}
	return 1;
}
/* These validXXX functions can be written much more efficiently like this.
returns 1 if the statement is true, false 0.
int validLocation(int building, int floor, int room) {
	return building >= 0 && building <= 20 && floor >= 0 && floor <= 30 && 
		room >= 0 && room <= 600;
} 
*/

/******************************************************************************
 * Checks if the time information put in is valid
 * Author: Victor
 * IN: stirng for day, integers for hour and minute
 * OUT: 0 if valid, 1 if invalid
******************************************************************************/
int validTime(char day[], int hour, int minute)
{
	if((strcmp(day, "mon") == 0 || strcmp(day, "tue") == 0 || 
		strcmp(day, "wed") == 0 || strcmp(day, "thu") == 0 || 
		strcmp(day, "fri") == 0 || strcmp(day, "sat") == 0 ) &&
		(hour >= 0 && hour <= 23) && (minute >= 0 && minute <= 59))
	{
		return 0;
	}
	return 1;
}

/******************************************************************************
 * Add a course to the list then load it to the file
 * Author: Quentin
 * IN: the array where we are storing the courses, a pointer to the amount of
 * courses in the array
 * OUT: 0 if succesfull, 1 if unsuccesfull
******************************************************************************/
int addCourse(course_t AllCourses[], int* CoursesAMT)
{
	int validInput = 1;
	int i;
	int success = 1;

	while(validInput == 1)
	{
		printf("Enter the number of the course: ");
		scanf("%d", &AllCourses[*CoursesAMT].code);
		if(validCourseNum(AllCourses[*CoursesAMT].code)==1)
		{
			printf("Invalid input\n");
		}
		else
		{
			validInput = 0;
		}
	}
	validInput = 1;

	printf("Enter the name of the course: ");
	scanf("%s", AllCourses[*CoursesAMT].name);

	printf("Enter the lecturer of the course (ex: Name_Surname): ");
	scanf("%s", AllCourses[*CoursesAMT].lecturer);

	for(i=0; i <= SLOT_NUM; i++)
	{
		printf("\nClass number %d\n", (i+1));
		while(validInput == 1)
		{
			printf("Enter the class and the group seperated by a space "
				"(Lab 1, Tut 2, Lec 1...): ");
			scanf("%s %d", AllCourses[*CoursesAMT].slot_a[i].type, 
				&AllCourses[*CoursesAMT].slot_a[i].group);
			if(validClassName(AllCourses[*CoursesAMT].slot_a[i].type)==1
				|| validGroupNum(AllCourses[*CoursesAMT].slot_a[i].group)==1)
			{
				printf("Invalid input\n");
			}
			else
			{
				validInput = 0;
			}
		}
		validInput = 1;

		printf("Enter the lecturer of the class (ex: Name_Surname): ");
		scanf("%s", AllCourses[*CoursesAMT].slot_a[i].lecturer);

		while(validInput == 1)
		{
			printf("Enter the building, floor and the room seperated by a space: ");
			scanf("%d %d %d", &AllCourses[*CoursesAMT].slot_a[i].building,
				&AllCourses[*CoursesAMT].slot_a[i].floor, 
				&AllCourses[*CoursesAMT].slot_a[i].room);
			if(validLocation(AllCourses[*CoursesAMT].slot_a[i].building,
				AllCourses[*CoursesAMT].slot_a[i].floor, 
				AllCourses[*CoursesAMT].slot_a[i].room) ==1 )
			{
				printf("Invalid input\n");
			}
			else
			{
				validInput = 0;
			}
		}
		validInput = 1;

		AllCourses[*CoursesAMT].slot_a[i].StudentAMT = 0;

		while(validInput == 1)
		{
			printf("Enter the start time (ex format: mon 10:23): ");
			scanf("%s %d:%d", AllCourses[*CoursesAMT].slot_a[i].start.day,
				&AllCourses[*CoursesAMT].slot_a[i].start.hour, 
				&AllCourses[*CoursesAMT].slot_a[i].start.minute);
			if(validTime(AllCourses[*CoursesAMT].slot_a[i].start.day,
				AllCourses[*CoursesAMT].slot_a[i].start.hour, 
				AllCourses[*CoursesAMT].slot_a[i].start.minute) == 1)
			{
				printf("Invalid input\n");
			}
			else
			{
				validInput = 0;
			}
		}
		validInput = 1;

		while(validInput == 1)
		{
			printf("Enter the end time (ex format: mon 10:23): ");
			scanf("%s %d:%d", AllCourses[*CoursesAMT].slot_a[i].end.day,
				&AllCourses[*CoursesAMT].slot_a[i].end.hour, 
				&AllCourses[*CoursesAMT].slot_a[i].end.minute);
			if(validTime(AllCourses[*CoursesAMT].slot_a[i].end.day,
				AllCourses[*CoursesAMT].slot_a[i].end.hour, 
				AllCourses[*CoursesAMT].slot_a[i].end.minute) == 1)
			{
				printf("Invalid input\n");
			}
			else
			{
				validInput = 0;
			}
		}
		validInput = 1;
	}


	(*CoursesAMT)++;

	success = saveClasses(AllCourses, *CoursesAMT);
	return success;
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
    	fprintf(fp, "%8d %20s %20s pw: %20s %02d/%02d/%4d %3d %20s %10ld\n",
    		current->stu.number,current->stu.firstname, current->stu.lastname,
    		current->stu.password, current->stu.personalInfo.birthday.day,
    		current->stu.personalInfo.birthday.month,
    		current->stu.personalInfo.birthday.year,
    		current->stu.personalInfo.address.houseNumber,
    		current->stu.personalInfo.address.streetName,
    		current->stu.personalInfo.phoneNum);
    	current = current->nextp;
    }

    fclose(fp);

    return 0;

}

/******************************************************************************
 * Loads the linked list of students from a txt file
 * !! Currently compiles but has segmentation fault !!
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

    while (!feof(fp)){
    	fscanf(fp, "%8d %20s %20s pw: %20s %02d/%02d/%4d %3d %20s %10ld\n",
    		&current->stu.number,current->stu.firstname,current->stu.lastname,
    		current->stu.password, &current->stu.personalInfo.birthday.day,
    		&current->stu.personalInfo.birthday.month,
    		&current->stu.personalInfo.birthday.year,
    		&current->stu.personalInfo.address.houseNumber,
    		current->stu.personalInfo.address.streetName,
    		&current->stu.personalInfo.phoneNum);
    	current = current->nextp;
    }

    fclose(fp);
    return 0;

}