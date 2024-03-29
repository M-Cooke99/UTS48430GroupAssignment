/******************************************************************************
 * DESCRPITPION:
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
	int stuNum;
	student_t match;

	do {printf ("Enter Student Number> ");
		scanf ("%d", &stuNum);
	}while (!checkStuNum(stuNum, head, &match));

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
	while (current != NULL){
		if (stuNum == current->stu.number) {
			*match = current->stu;
			return 1;
		}
		current = current->nextp;
	}
	printf("Student Number Invalid\n");
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
	if (strcmp((*stup).password,"default")!=0){
		checkPassword(*stup);
	}
	printf("Set a password> ");
	scanf("%s", (*stup).password);
}

/******************************************************************************
 * Ensures any changes made to a student is to be saved in the linked list
 * Author: Victor
 * IN: student for whom the password must be set
 * OUT: none
******************************************************************************/
int saveStudentChanges(studentNode_t* head, student_t stu){
	studentNode_t* current = head;
	while (current != NULL){
		if (stu.number == current->stu.number){
			current->stu = stu;
			return 1;
		}
	current = current->nextp;
	}
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
	studentNode_t* studentListp;
	studentListp = NULL;
	course_t AllCourses[MAX_COURSES_AMT];
	int CoursesAMT = 0;
	loadCourse(AllCourses, &CoursesAMT);
	loadEnrollments(AllCourses, CoursesAMT, studentListp);

	loadStudentList(&studentListp); 

	/* Makes login easier during programming */
	studentNode_t* current = studentListp;
	while (current != NULL){
		printf("ID: %d PW: %s\n", current->stu.number, current->stu.password);
		current = current->nextp;
	}

	student_t currentStu = getStu(studentListp);

	if (strcmp(currentStu.password, "default") == 0){
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
	int choice;
	course_t AllCourses[MAX_COURSES_AMT];
	int CoursesAMT = 0;
	studentNode_t* studentListp;
	studentListp = NULL;
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
	    	case 7: printEnrollment(studentListp); break;
	    	case 8: addCourse(AllCourses, &CoursesAMT); break;
	    	case 9: EnrollAStudent(studentListp, AllCourses, CoursesAMT);
	    		break;
	    	case 10: DisenrollAStudent(studentListp, AllCourses, CoursesAMT);
	    		break;
	    	case 11: saveStudentList(studentListp); break;
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
    "Enter Choice (number between 1-10)> ");
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

	do {printf("Enter birthday consisting of day, month, year,"
			" seperated by spaces> ");
		scanf("%d%d%d", &day, &month, &year);

		if(!validDate(day, month, year)){
			printf("Invalid date.\n");
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
	long phoneNum;

	do {printf("Enter phone number> ");
		scanf("%ld",&phoneNum);

		if(!validPhoneNumber(phoneNum)){
			printf("Invalid phone number.\n");
		}

	} while (!validPhoneNumber(phoneNum));

	(*stup).personalInfo.phoneNum = phoneNum;
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
	student_t stu = getStu(*head);
	studentNode_t* current = *head;
	studentNode_t* previous = NULL;
	studentNode_t* next_node = NULL;

	while (current!=NULL){
		if (stu.number == current->stu.number){
			if (previous == NULL){
				next_node = (*head)->nextp;
				free(*head);
				*head = next_node;
				return 0;
			} else if (current->nextp==NULL){
				free(current->nextp);
				previous->nextp=NULL;
			}
			next_node = current->nextp;
			free(current);
			previous->nextp = next_node;
			return 0;
		}
		previous = current;
		current = current->nextp;
	}
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
 * Check if a student is enrolled in a particular class (MAJ: USELESS)
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
	FILE* fp = NULL;
    fp = fopen(STUDENTS_FILE, "w");

    if (fp == NULL){
        printf("Write error\n");
        return 1;
    }

    studentNode_t* current = head;

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
    HuffmanCompression(STUDENTS_FILE);

    return 0;

}

/******************************************************************************
 * Loads the linked list of students from a txt file
 * Author: Victor
 * IN: pointer to the pointer to the first node in the linked list of students
 * OUT: 0 if successful, 1 if unsuccessful
******************************************************************************/
int loadStudentList(studentNode_t** head){
	FILE* fp = NULL; char line[10];
    fp = fopen(STUDENTS_FILE, "r");
    char firstdatapoint = 1;

    if (fp == NULL){
        printf("Read error\n");
        return 1;
    }

    fclose(fp);
    HuffmanDecompression(STUDENTS_FILE, CODE_FILE);
    fp = fopen(STUDENTS_FILE, "r");


    *head = (studentNode_t*) malloc(sizeof(studentNode_t));
    studentNode_t* current = *head;
    studentNode_t* previous = NULL;

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
			return i;
		}
	}
	return 1;
}

/******************************************************************************
 * Save the infos of the enrollments to the students linked list (NOT WORKING)
 * Author: Quentin
 * IN: the course we want to add, the student, a pointer to the first node in 
 * the linked list of students, the array of courses, the amount of courses in 
 * the array, the slot number in which we want to enroll the student
 * OUT: 0 if successful, 1 if unsuccessfull
******************************************************************************/
/*int EnrollStudent(int courseNum, int stdNum, studentNode_t* head,
	course_t AllCourses[], int coursesAMT, int slotNum)
{
	student_t std;
	course_t* crse = (course_t*) malloc(sizeof(course_t));
	checkStuNum(stdNum, head, &std);
	if(FindCourse(courseNum, crse, AllCourses, coursesAMT) != 1)
	{
		coursesAMT = std.enrollments.coursesAMT;
		if(coursesAMT < MAX_ENROLLMENT)
		{ 
			AddEnrolledCourse(*crse, &std);
			switch (slotNum)
			{
				case 0: 
					std.enrollments.enrolledCRSE[coursesAMT].LecGrp = 1; 
					break;
            	case 1:
            		std.enrollments.enrolledCRSE[coursesAMT].LecGrp = 2;
            		break;
            	case 2:
            		std.enrollments.enrolledCRSE[coursesAMT].LabGrp = 1;
            		break;
            	case 3: 
            		std.enrollments.enrolledCRSE[coursesAMT].LabGrp = 2;
            		break;
            	case 4: 
            		std.enrollments.enrolledCRSE[coursesAMT].TutGrp = 1;
            		break;
            	case 5:
            		std.enrollments.enrolledCRSE[coursesAMT].LecGrp = 2; 
            		break;
	    	
            	default: 
            		break;
            }
			return 0;
		}
	}
	return 1;
}*/

/******************************************************************************
 * Add a course to the student enrollments
 * Author: Quentin
 * IN: the course we want to add, a pointer to the student
 * OUT: NONE
******************************************************************************/
void AddEnrolledCourse(course_t theCourse, student_t* std)
{
	std->enrollments.enrolledCRSE[std->enrollments.coursesAMT].course = theCourse;
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
        				/*EnrollStudent(AllCourses[i].code, stdNum, head, 
        					AllCourses, CoursesAMT, j); 
        				To save the enrollments to the linked list, 
        				not working*/
        				studentAMT++;
        				fseek(database, 1, SEEK_CUR);
        				currChar = fgetc(database);
        			}
        		}
        		AllCourses[i].slot_a[j].StudentAMT = studentAMT;
            }
        }
        fclose(database);
    }
    else
    {
        printf("Read error\n");
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
    }
    else
    {
        printf("Write error\n");
        return 1;
    }

    fclose(database);

    return 0;
}
