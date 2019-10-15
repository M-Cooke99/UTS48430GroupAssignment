/******************************************************************************
 * DESCRPITPION:
******************************************************************************/

/******************************************************************************
 * HEADER FILES
******************************************************************************/
#include "timetable.h" 

/******************************************************************************
 * MAIN 
 * Author: Victor
******************************************************************************/
int main(int argc, char *argv[]) {
	if (argc <= 1){
		printf("Specify runtime mode\n");
		printf("The following opions are available:\n");
        printf("%8s %s %s %s \n", " ", "-a", " ", "Admin mode");
        printf("%8s %s %s %s \n", " ", "-s", " ", "Student mode");
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
	int stuNum;	student_t match;

	/*Loop asking the user for a student number, until a valid one is entered*/
	do { printf ("Enter Student Number> ");
		scanf ("%d", &stuNum);
	} while (!checkStuNum(stuNum, head, &match));

	#ifdef DEBUG
		printf("DEBUG: Match Found\n");
	#endif

	return match;
}

/******************************************************************************
 * Checks if student number exists in the list of students
 * Author: Victor
 * IN: student number, head of linked list and pointer to student_t
 * OUT: 1 if successful, 0 if unsuccesful
******************************************************************************/
int checkStuNum(int stuNum, studentNode_t* head, student_t* match){ 
	studentNode_t* current = head;

	/* Loops through linked list of students until a match is found */
	while (current != NULL){
		if (stuNum == current->stu.number) {
			*match = current->stu;
			return 1;
		}
		current = current->nextp;
	}
	printf("Student Number Invalid.\n");
	return 0;
}

/******************************************************************************
 * Asks user for password and checks if the password is correct
 * Author: Victor
 * IN: student for whom the password is to be asked and checked
 * OUT: none
******************************************************************************/
void checkPassword(student_t student){
	char pw[20];

	/* Loop asking the user for a password, until the correct one is entered*/
	do {printf ("Enter password> ");
		scanf ("%s", pw);	
		if (strcmp(student.password, pw)!=0){
			printf("Password incorrect!\n");	
		}	
	} while (strcmp(student.password, pw)!=0);
	printf("Password correct!\n");
} 

/******************************************************************************
 * Let's a student set its password
 * Author: Victor
 * IN: student for whom the password must be set
 * OUT: none
******************************************************************************/
void setPassword(student_t* stup){

	/* Known users have to confirm their password before 
	being able to change it */
	if (strcmp((*stup).password,"default")!=0){

		#ifdef DEBUG
			printf("DEBUG: Known user, confirm password\n");
		#endif

		checkPassword(*stup);
	}
	printf("Set a password> ");
	scanf("%s", (*stup).password);

	#ifdef DEBUG
		printf("DEBUG: Password set to '%s'\n", (*stup).password);
	#endif
}

/******************************************************************************
 * Ensures any changes made to a student is to be saved in the linked list
 * Author: Victor
 * IN: student for whom the password must be set
 * OUT: none
******************************************************************************/
int saveStudentChanges(studentNode_t* head, student_t stu){
	studentNode_t* current = head;

	/* Loops through linked list of students until the correct student is 
	found, when found the student with updated details is saved in the list */
	while (current != NULL){
		if (stu.number == current->stu.number){
			current->stu = stu;

			#ifdef DEBUG
				printf("DEBUG: Changes saved succesfully\n");
			#endif

			return 1;
		}
	current = current->nextp;
	}

	#ifdef DEBUG
		printf("DEBUG: Unable to save changes, student not found\n");
	#endif

	return 0;
}

/******************************************************************************
 * Student main: menu options
 * Author: Michael, Victor
 * IN: None
 * OUT: 0 if successful
******************************************************************************/
int studentMain(void){
	int choice;	studentNode_t* studentListp; studentListp = NULL;
	course_t AllCourses[MAX_COURSES_AMT]; int CoursesAMT = 0;

	/* Load necessary information from local files */
	loadCourse(AllCourses, &CoursesAMT);
	loadEnrollments(AllCourses, CoursesAMT, studentListp);
	loadStudentList(&studentListp); 

	/* Makes login easier during programming */
	studentNode_t* current = studentListp;
	while (current != NULL){
		printf("ID: %d PW: %s\n", current->stu.number, current->stu.password);
		current = current->nextp;
	}

	/* Retrieves the student struct to be used */
	student_t currentStu = getStu(studentListp);

	/* password check and set if first login */
	if (strcmp(currentStu.password, "default") == 0){

		#ifdef DEBUG
			printf("DEBUG: First login, password needs to be set\n");
		#endif

		setPassword(&currentStu);
		saveStudentChanges(studentListp,currentStu);
	} else {
		checkPassword(currentStu);
	}

	do { printStuMenu(&choice);
        switch (choice){
            case 1: printStuTimetable(currentStu.number,AllCourses,CoursesAMT); 
            		break;
            case 2: printStuDetails(currentStu); break;
            case 3: editStu(studentListp, currentStu); 
            		/* update currentStu after editing of details */
            		checkStuNum(currentStu.number,studentListp,&currentStu); 
            		break;
            case 4: saveStudentList(studentListp); break;
            default: printf("Invalid choice\n");} 
    } while (choice != 4);
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
    "3. Edit Personal Details\n"
    "4. Exit Program\n"
    "Enter Choice (number between 1-4)> ");
    scanf("%d", choicep);
}

/******************************************************************************
 * Prints the students timetable, chronologically per line
 * Assume input to be student struct in which an array of timetable structs is
 * Author: Hana, Quentin
 * IN: Student number for the timetable to be printed, the array where we are 
 * storing the courses, the amount of courses in the array
 * OUT: 
******************************************************************************/
void printStuTimetable(int stdNum, course_t AllCourses[], int coursesAMT){

printf("\n==== timetable ====\n");
 int i, j, f;
 int courseHeader;

	for(i = 0; i<coursesAMT; i++)
	{
		courseHeader = 0;
		for(f=0; f<SLOT_NUM; f++)
		{
			for(j=0; j<AllCourses[i].slot_a[f].StudentAMT; j++)
			{
				if(stdNum == AllCourses[i].slot_a[f].studentEn[j])
				{
					if(courseHeader == 0)
					{
						printf("%03d %s - Lecturer: %s \n",
							AllCourses[i].code, 
							AllCourses[i].name, 
							AllCourses[i].lecturer);
						courseHeader = 1;
					}
					printf("%s%d time: %sday %02d:%02d ~ %02d:%02d, location: CB.%d.%d.%d \n",
						AllCourses[i].slot_a[f].type,
						AllCourses[i].slot_a[f].group,

						AllCourses[i].slot_a[f].start.day,
						AllCourses[i].slot_a[f].start.hour,
						AllCourses[i].slot_a[f].start.minute , 
						AllCourses[i].slot_a[f].end.hour,
						AllCourses[i].slot_a[f].end.minute ,

						AllCourses[i].slot_a[f].building ,
						AllCourses[i].slot_a[f].floor, 
						AllCourses[i].slot_a[f].room );
				}
			}
		}
	}	
}

/******************************************************************************
 * Prints the students details
 * Author: Victor
 * IN: The student for which the details need to be printed
 * OUT: None
******************************************************************************/
void printStuDetails(student_t student){
	printf("\nName: %s, %s\nStudent ID: %8d\nBirthday: %02d/%02d/%4d\n"
		"Address: %d %s\nPhone number: %010ld\n", student.lastname, 
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
	int choice;	course_t AllCourses[MAX_COURSES_AMT]; int CoursesAMT = 0;
	studentNode_t* studentListp; studentListp = NULL;

	/* Load necessary information from local files */
	loadCourse(AllCourses, &CoursesAMT);
	loadStudentList(&studentListp);
	loadEnrollments(AllCourses, CoursesAMT, studentListp);

	do { printAdminMenu(&choice);
        switch (choice){
            case 1: addStu(&studentListp); break;
            case 2: removeStu(&studentListp); break;
            case 3: editStu(studentListp, getStu(studentListp)); break;
            case 4: printStu(studentListp, AllCourses, CoursesAMT); break;
	    	case 5: printCourse(AllCourses, CoursesAMT); break;
	    	case 6: printClass(studentListp, AllCourses, CoursesAMT); break;
	    	case 7: printEnrollment(AllCourses, CoursesAMT); break;
	    	case 8: addCourse(AllCourses, &CoursesAMT); break;
	    	case 9: EnrollAStudent(studentListp, AllCourses, CoursesAMT);
	    			break;
	    	case 10:DisenrollAStudent(studentListp, AllCourses, CoursesAMT);
	    			break;
	    	case 11:saveStudentList(studentListp); break;
            default: printf("Invalid choice\n");} 
    } while (choice != 11);
	return 0;
}


/******************************************************************************
 * Prints admin menu options 
 * Author: Michael, Victor, Quentin
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
    "9. Enroll a student to a class\n"
    "10. Remove a student from a class\n"
    "11. Exit Program\n"
    "Enter Choice (number between 1-11)> ");
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

	/* Retrieve details of new student from user input */
	getName(&new);
	getNumber(&new);
	strncpy(new.password,"default",20);
	getBirthday(&new);
	getAddress(&new);
	getPhoneNumber(&new);

	/* Add new student to linked list */
	if (*head == NULL){
		*head = (studentNode_t*) malloc(sizeof(studentNode_t));
		(*head)->stu=new;
		(*head)->nextp=NULL;

		#ifdef DEBUG
			printf("DEBUG: Student list has been created and first student "
				"has succesfully been added");
		#endif

	} else {
		studentNode_t* current = *head;
		while (current->nextp != NULL){
			current = current->nextp;
		}
		current->nextp = (studentNode_t*) malloc(sizeof(studentNode_t));
		current->nextp->stu = new;
		current->nextp->nextp = NULL;

		#ifdef DEBUG
			printf("DEBUG: Student has succesfully been added to the list");
		#endif
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
	
	#ifdef DEBUG
		printf("DEBUG: Name set to %s %s\n", (*stup).firstname, 
			(*stup).lastname);
	#endif
}

/******************************************************************************
 * Asks for student number input and stores it in student struct
 * Author: Victor
 * IN: pointer to a student struct
 * OUT: None
******************************************************************************/
void getNumber(student_t* stup){
	int temp;
	
	/*Loop asking the user for a student number, until a valid one is entered*/
	do { printf("Enter student number> ");
		scanf("%d", &temp);

		if (!validStuNum(temp)){
			printf("Invalid student number.\n");
		}

	} while (!validStuNum(temp));
	
	/* Save input to student struct */
	(*stup).number = temp;

	#ifdef DEBUG
		printf("DEBUG: Student number valid and set to %d\n", (*stup).number);
	#endif
}

/******************************************************************************
 * Checks if student number input is valid
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

	/* Loop asking the user for a birthday, until a valid one is entered */
	do {printf("Enter birthday consisting of day, month, year,"
			" seperated by spaces> ");
		scanf("%d%d%d", &day, &month, &year);

		if(!validDate(day, month, year)){
			printf("Invalid date.\n");
		}

	} while (!validDate(day, month, year));
	
	/* Save input to student struct */
	(*stup).personalInfo.birthday.day = day;
	(*stup).personalInfo.birthday.month = month;
	(*stup).personalInfo.birthday.year = year;

	#ifdef DEBUG
		printf("DEBUG: Birthday valid and set to %02d/%02d/%4d\n",
			(*stup).personalInfo.birthday.day,
			(*stup).personalInfo.birthday.month,
			(*stup).personalInfo.birthday.year);
	#endif
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

	#ifdef DEBUG
		printf("DEBUG: Address set to %d %s\n", 
			(*stup).personalInfo.address.houseNumber,
			(*stup).personalInfo.address.streetName);
	#endif
}

/******************************************************************************
 * Asks for student's phone number input and stores it in student struct
 * How do we want to check if the number is valid?
 * Author: Victor
 * IN: pointer to a student struct
 * OUT: None
******************************************************************************/
void getPhoneNumber(student_t* stup){
	long phoneNum;

	/* Loop asking the user for a phone number, until a valid one is entered */
	do {printf("Enter phone number> ");
		scanf("%ld",&phoneNum);

		if(!validPhoneNumber(phoneNum)){
			printf("Invalid phone number.\n");
		}

	} while (!validPhoneNumber(phoneNum));

	/* Save input to student struct */
	(*stup).personalInfo.phoneNum = phoneNum;

	#ifdef DEBUG
		printf("DEBUG: Phone number valid and set to %ld\n", 
			(*stup).personalInfo.phoneNum);
	#endif
}

/******************************************************************************
 * Checks if student phone number input is valid
 * Author: Victor
 * IN: integer containing the phone number
 * OUT: 1 if valid, 0 if invalid
******************************************************************************/
int validPhoneNumber(long phoneNum){
	return phoneNum >= 100000000 && phoneNum <= 999999999;
}

/******************************************************************************
 * Removes student from the student linked list
 * Author: Victor
 * IN: Pointer to the pointer of the first node in the list of students
 * OUT: 0 if successful, 1 if unsuccessful
******************************************************************************/
int removeStu(studentNode_t** head){
	student_t stu = getStu(*head); studentNode_t* current = *head;
	studentNode_t* previous = NULL;	studentNode_t* next_node = NULL;

	/* Loops through linked list of students until the correct one has been 
		found, after which it is removed from the linked list */
	while (current!=NULL){
		if (stu.number == current->stu.number) {
			if (previous == NULL){
				next_node = (*head)->nextp;
				free(*head);
				*head = next_node;
			} else if (current->nextp==NULL){
				free(current->nextp);
				previous->nextp=NULL;
			} else {
				next_node = current->nextp;
				free(current);
				previous->nextp = next_node;
			}

			#ifdef DEBUG
				printf("DEBUG: Student has succesfully been removed from "
					"the list\n");
			#endif

			return 0;
		}
		previous = current;
		current = current->nextp;
	}

	#ifdef DEBUG
		printf("DEBUG: Removing the student has been unsuccesful\n");
	#endif

	return 1;
}

/******************************************************************************
 * Edit student details function
 * Author: Victor
 * IN: pointer to the first node in the student list, student to be edited
 * OUT: 0 if successful
******************************************************************************/
int editStu(studentNode_t* head, student_t stu){
	int choice;

	do { printEditMenu(&choice);
        switch (choice){
            case 1: getName(&stu); 
            		printf("\nName changed to: %s %s\n", stu.firstname,
            		stu.lastname); break;
            case 2: getBirthday(&stu); 
            		printf("\nBirthday changed to: %02d/%02d/%4d\n", 
            		stu.personalInfo.birthday.day, 
            		stu.personalInfo.birthday.month, 
            		stu.personalInfo.birthday.year); break;
            case 3: getAddress(&stu); printf("\nAddress changed to: %d %s \n", 
            		stu.personalInfo.address.houseNumber,
            		stu.personalInfo.address.streetName); break;
            case 4: getPhoneNumber(&stu); 
            		printf("\nPhone number changed to: %ld\n", 
            		stu.personalInfo.phoneNum); break;
            case 5: setPassword(&stu); break;
	    	case 6: saveStudentChanges(head,stu); break;
            default: printf("Invalid choice\n");} 
    } while (choice != 6);
	return 0;
}

/******************************************************************************
 * Prints the student edit menu
 * Author: Victor
 * IN: pointer to an integer choice variable
 * OUT: None
******************************************************************************/
void printEditMenu(int* choicep){
	printf("\nWhich information would you like to change?\n"
    "1. Name\n"
    "2. Birthday\n"
    "3. Address\n"
    "4. Phone Number\n"
    "5. Password\n"
    "6. Exit Menu\n"
    "Enter Choice (number between 1-6)> ");
    scanf("%d", choicep);
}

/******************************************************************************
 * Prints a students details followed by their timetable
 * Author: Michael, Victor
 * IN: pointer to the first node of the student linked list, the array where 
 * we are storing the courses, a pointer to the amount of courses in the array
 * OUT: None
******************************************************************************/
void printStu(studentNode_t* head, course_t AllCourses[], int coursesAMT){
	/* Retrieve student and display information */
	student_t student = getStu(head);
	printStuDetails(student);
	printStuTimetable(student.number, AllCourses, coursesAMT);
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
	#ifdef DEBUG
		printf("DEBUG: The amount of student enrolled in each class has" 
			"succesfully been printed");
	#endif
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
	#ifdef DEBUG
		printf("DEBUG: Choice of class valid and set to %d %s %d\n", 
			*courseNum, classType, *group);
	#endif
	
}


/******************************************************************************
 * Prints the names and number of students enrolled in a given Lab/Lec/Tut 
 * Author: Quentin
 * IN: pointer to the first node in the linked list of students, the array of 
 * the courses, the amount of courses
 * OUT: None
******************************************************************************/
void printClass(studentNode_t* head, course_t AllCourses[], int coursesAMT)
{
	int courseNum, group, i, k, stuNum; 
	char classType[4];
	int newLine = 0;
	int location;
	student_t oneStudent;

	chooseClass(&courseNum, classType, &group);
	location = FindCourse(courseNum, AllCourses, coursesAMT);

    for (i = 0; i<6; i++)
    {
    	if(AllCourses[location].slot_a[i].group == group && 
    		strcmp(AllCourses[location].slot_a[i].type, classType)==0)
    	{
    		printf("Enrolled students :\n");
    		for(k=0; k<AllCourses[location].slot_a[i].StudentAMT; k++)
    		{
    			stuNum = AllCourses[location].slot_a[i].studentEn[k];
    			checkStuNum(stuNum, head, &oneStudent);
    			printf("%d %s\t", stuNum, oneStudent.lastname);
    			newLine++;
    			if(newLine == 4)
    			{
    				printf("\n");
    				newLine = 0;
    			}
    		}
    	}
    }
    printf("\n");
    #ifdef DEBUG
		printf("DEBUG: Class succesfully printed\n");
	#endif
}


/******************************************************************************
 * Prints the number of students enrolled and a list of their student numbers
 * Author: Quentin
 * IN: the array of the courses, the amount of courses
 * OUT: None
******************************************************************************/
void printEnrollment(course_t AllCourses[], int coursesAMT)
{
	int i, f, j, k;
	int enrolledNum = 0;
	int enrolled[200];
	int alreadyFound;

    for(i = 0; i<coursesAMT; i++)
	{
		for(f=0; f<SLOT_NUM; f++)
		{
			for(j=0; j<AllCourses[i].slot_a[f].StudentAMT; j++)
			{
				alreadyFound = 0;
				for(k=0; k<enrolledNum; k++)
				{
					if(enrolled[k] == AllCourses[i].slot_a[f].studentEn[j])
					{
						alreadyFound = 1;
					}
				}
				if(alreadyFound != 1)
				{
					enrolled[enrolledNum]= AllCourses[i].slot_a[f].studentEn[j];
					enrolledNum++;
				}
			}
		}
	}	
    printf("\n%d students are enrolled in at least one course\n", enrolledNum);
    #ifdef DEBUG
		printf("DEBUG: Number of enrollments succesfully printed and is equal to %d \n",
			enrolledNum);
	#endif
}

/******************************************************************************
 * Loads the informations of one course from the txt file
 * Author: Quentin
 * IN: the array where we are storing the courses, a pointer to the amount of
 * courses in the array
 * OUT: 0 if successful, 1 if unsuccessful
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
        #ifdef DEBUG
			printf("DEBUG: Courses list succesfully loaded from %s\n",
				COURSES_FILE);
		#endif
    }
    else
    {
        printf("Read error\n");
        #ifdef DEBUG
			printf("DEBUG: Courses list unsuccesfully loaded from %s\n",
				COURSES_FILE);
		#endif
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
 * OUT: 0 if successful, 1 if unsuccessful
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
        #ifdef DEBUG
			printf("DEBUG: Courses list succesfully saved to %s\n",
				COURSES_FILE);
		#endif
    }
    else
    {
        printf("Write error\n");
        #ifdef DEBUG
			printf("DEBUG: Courses list unsuccesfully saved to %s\n",
				COURSES_FILE);
		#endif
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
 * OUT: 0 if successful, 1 if unsuccessful
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
	#ifdef DEBUG
		printf("DEBUG: New course %d %s and all the informations are valid\n", 
			AllCourses[*CoursesAMT].code, AllCourses[*CoursesAMT].name);
	#endif

	success = saveClasses(AllCourses, *CoursesAMT);
	return success;
}

/******************************************************************************
 * Saves the the linked list of students to a txt file
 * !! Currently does not save timetables !!
 * Author: Victor
 * IN: pointer to the first node in the linked list of students
 * OUT: 0 if successful, 1 if unsuccessful
******************************************************************************/
int saveStudentList(studentNode_t* head){
	FILE* fp = NULL; fp = fopen(STUDENTS_FILE, "w");

	/* check if file has been opened correctly before writing */
    if (fp == NULL){
        printf("Write error\n");
        return 1;
    }

    studentNode_t* current = head;

    /* Loop through linked list of students and save each student in the opened
     	text file */
    while (current != NULL){
    	fprintf(fp, "\n \n%8d %s %s pw: %s %02d/%02d/%4d %d %s %10ld",
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

    #ifdef DEBUG
		printf("DEBUG: Student list succesfully saved to %s\n",
			STUDENTS_FILE);
	#endif

    return 0;

}

/******************************************************************************
 * Loads the linked list of students from a txt file
 * Author: Victor
 * IN: pointer to the pointer to the first node in the linked list of students
 * OUT: 0 if successful, 1 if unsuccessful
******************************************************************************/
int loadStudentList(studentNode_t** head){
	FILE* fp = NULL; char line[10]; fp = fopen(STUDENTS_FILE, "r"); 
	char firstdatapoint = 1;

	/* check if file has been opened correctly before reading */
    if (fp == NULL){
        printf("Read error\n");
        return 1;
    }

    *head = (studentNode_t*) malloc(sizeof(studentNode_t));
    studentNode_t* current = *head;
    studentNode_t* previous = NULL;

    /* Loop terminiting when the next line is empty, until then read the line
    	and save the corresponding information in a linked list */
    while(fgets(line, 10, fp) != NULL){
    	if (firstdatapoint){
    		firstdatapoint = 0;
    	} else {
  			current = (studentNode_t*) malloc(sizeof(studentNode_t));
  			previous->nextp = current;
    	}
    	fscanf(fp, "%d %s %s pw: %s %d/%d/%d %d %s %10ld",
    		&(current->stu.number),current->stu.firstname,current->stu.lastname,
    		current->stu.password, &(current->stu.personalInfo.birthday.day),
    		&(current->stu.personalInfo.birthday.month),
    		&(current->stu.personalInfo.birthday.year),
    		&(current->stu.personalInfo.address.houseNumber),
    		current->stu.personalInfo.address.streetName,
    		&(current->stu.personalInfo.phoneNum));
    	current->nextp = NULL;
    	previous = current;
    }

    fclose(fp);

    #ifdef DEBUG
		printf("DEBUG: Student list succesfully loaded from %s\n",
			STUDENTS_FILE);
	#endif

    return 0;

}

/******************************************************************************
 * Enroll a student to a class (not to the linked list) 
 * Author: Quentin
 * IN: pointer to the first node in the linked list of students, the array of 
 * the courses, the amount of courses
 * OUT: None
******************************************************************************/
void EnrollAStudent(studentNode_t* head, course_t AllCourses[], int coursesAMT)
{
	int courseNum, group, i, location; 
	char classType[4];
	student_t oneStudent;

	oneStudent = getStu(head);
	printf("Class informations\n");
	chooseClass(&courseNum, classType, &group);
	location = FindCourse(courseNum, AllCourses, coursesAMT);

    for (i = 0; i<6; i++)
    {
    	if(AllCourses[location].slot_a[i].group == group && 
    		strcmp(AllCourses[location].slot_a[i].type, classType)==0)
    	{
    		AllCourses[location].slot_a[i].studentEn[AllCourses[location].slot_a[i].StudentAMT] 
    			= oneStudent.number;
    		AllCourses[location].slot_a[i].StudentAMT++;
    	}
    }
    #ifdef DEBUG
		printf("DEBUG: Student %d succesfully enrolled\n", oneStudent.number);
	#endif
    saveEnrollments(AllCourses, coursesAMT);
}

/******************************************************************************
 * Remove a student from a class (not to the linked list) 
 * Author: Quentin
 * IN: pointer to the first node in the linked list of students, the array of 
 * the courses, the amount of courses
 * OUT: None
******************************************************************************/
void DisenrollAStudent(studentNode_t* head, course_t AllCourses[], 
	int coursesAMT)
{
	int courseNum, group, i, j, k, location; 
	char classType[4];
	student_t oneStudent;

	oneStudent = getStu(head);
	printf("Class informations\n");
	chooseClass(&courseNum, classType, &group);
	location = FindCourse(courseNum, AllCourses, coursesAMT);

    for (i = 0; i<6; i++)
    {
    	if(AllCourses[location].slot_a[i].group == group && 
    		strcmp(AllCourses[location].slot_a[i].type, classType)==0)
    	{
    		for(j=0; j<AllCourses[location].slot_a[i].StudentAMT; j++)
    		{
    			if(AllCourses[location].slot_a[i].studentEn[j] == oneStudent.number)
    			{
    				for(k=j+1; k<AllCourses[location].slot_a[i].StudentAMT; k++)
    				{
    					AllCourses[location].slot_a[i].studentEn[k] 
    					= AllCourses[location].slot_a[i].studentEn[k+1];
    				}
    				AllCourses[location].slot_a[i].StudentAMT--;
    			}
    		}
    	}
    }
    #ifdef DEBUG
		printf("DEBUG: Student %d succesfully disenrolled\n", oneStudent.number);
	#endif
    saveEnrollments(AllCourses, coursesAMT);
}

/******************************************************************************
 * Find a course from the number
 * Author: Quentin
 * IN: the number of the course, a pointer to the course we want to find, the 
 * array of the courses, the amount of courses
 * OUT: the place in the array if found, 1 if not found
******************************************************************************/
int FindCourse(int courseNum, course_t AllCourses[], int coursesAMT)
{
	int i;
	for(i = 0; i<coursesAMT; i++)
	{
		if(AllCourses[i].code == courseNum)
		{
			#ifdef DEBUG
				printf("DEBUG: Course found\n");
			#endif
			return i;
		}
	}
	#ifdef DEBUG
		printf("DEBUG: Course not found\n");
	#endif
	return 1;
}



/******************************************************************************
 * Load the enrollments of the students from the file
 * Author: Quentin
 * IN: the array where we are storing the courses, the amount of courses in the
 * array, a pointer to the first node in the linked list of students
 * OUT: 0 if successful, 1 if unsuccessful
******************************************************************************/
int loadEnrollments(course_t AllCourses[], int CoursesAMT, studentNode_t* head)
{
	int i, j, stdNumLenght;
	int course_code = 0;
	int studentAMT, stdNum;
	char currChar;
	char line[10];

    FILE* database = NULL;
    database = fopen(ENROLLMENTS_FILE, "r");
 
    if (database != NULL)
    {
         
        for(i=0;  i<CoursesAMT; i++)
        {
        	fgets(line, 10, database);
        	/*Scan of the course general infos and find the right course in the
        	array*/
            fscanf(database, "%d", &course_code);
            fseek(database, 23, SEEK_CUR);

            /*Scan of each course slot*/
            for(j=0; j < SLOT_NUM; j++)
            {
            	studentAMT = 0;
            	fseek(database, 8, SEEK_CUR);
            	currChar = fgetc(database);
            	/*loading each student enrolled in the class*/
        		while (currChar != '\n')
        		{
        			if(studentAMT<MAX_PER_CLASS)
        			{
        				fseek(database, -1, SEEK_CUR);
        				/*student number calcul*/
        				stdNum = 0;
        				for(stdNumLenght = 7; stdNumLenght>=0; stdNumLenght--)
        				{
        					currChar = fgetc(database);
        					stdNum = stdNum + (currChar-'0')*pow(10, stdNumLenght);
        				}
        				AllCourses[i].slot_a[j].studentEn[studentAMT] = stdNum;
        				studentAMT++;
        				fseek(database, 1, SEEK_CUR);
        				currChar = fgetc(database);
        			}
        		}
        		AllCourses[i].slot_a[j].StudentAMT = studentAMT;
            }
        }
        fclose(database);
        #ifdef DEBUG
			printf("DEBUG: Enrollments list succesfully loaded from %s\n",
				ENROLLMENTS_FILE);
		#endif
    }
    else
    {
        printf("Read error\n");
        #ifdef DEBUG
			printf("DEBUG: Enrollments list unsuccesfully loaded from %s\n",
				ENROLLMENTS_FILE);
		#endif
        return 1;
    }
    
    return 0;
}

/******************************************************************************
 * Saves the enrollments to the file 
 * Author: Quentin
 * IN: the array where we are storing the courses, a pointer to the amount of
 * courses in the array
 * OUT: 0 if successful, 1 if unsuccessful
******************************************************************************/
int saveEnrollments(course_t AllCourses[], int CoursesAMT)
{
	int i, j, k;
	FILE* database = NULL;
    database = fopen(ENROLLMENTS_FILE, "w");

    if (database != NULL)
    {
        for(i=0; i < CoursesAMT; i++)
        {
        	fprintf(database, " \n");
        	/*Saves the course general infos*/
            fprintf(database, "%d %-21s\n", AllCourses[i].code, 
            	AllCourses[i].name);

            /*Saves each course slot infos*/
            for(j=0; j < SLOT_NUM; j++)
            {
            	fprintf(database, "%s %d:\t",
            		AllCourses[i].slot_a[j].type, AllCourses[i].slot_a[j].group);
            	for(k = 0; k<AllCourses[i].slot_a[j].StudentAMT; k++)
            	{
            		fprintf(database, "\t%d", AllCourses[i].slot_a[j].studentEn[k]);
            	}
            	fprintf(database, " \n");
            }
        }
        #ifdef DEBUG
			printf("DEBUG: Enrollments list succesfully saved to %s\n",
				ENROLLMENTS_FILE);
		#endif
    }
    else
    {
        printf("Write error\n");
        #ifdef DEBUG
			printf("DEBUG: Enrollments list succesfully saved to %s\n",
				ENROLLMENTS_FILE);
		#endif
        return 1;
    }

    fclose(database);

    return 0;
}
