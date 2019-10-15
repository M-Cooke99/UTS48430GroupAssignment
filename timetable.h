#ifndef TIMETABLE_H
#define TIMETABLE_H

/******************************************************************************
 * LIBRARIES
******************************************************************************/
#include <string.h> /* strcmp, strlen */
#include <stdio.h> /*printf, scanf */
#include <stdlib.h> /* malloc, free */
#include <math.h>
#include "huffman.h" /* HuffmanCompression, HuffmanDecompression */


/******************************************************************************
 * #DEFINES
******************************************************************************/
#define COURSES_FILE "Courses_Info.txt"
#define STUDENTS_FILE "Student_List.txt"
#define ENROLLMENTS_FILE "Enrollments.txt"
#define SLOT_NUM 6
#define MAX_ENROLLMENT 4
#define MAX_COURSES_AMT 20
#define MAX_PER_CLASS 60
/*#define DEBUG*/

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
	int studentEn[MAX_PER_CLASS];
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

/*This part of the code is not used*/
/*struct studentCourse
{
	int LecGrp, LabGrp, TutGrp;
	course_t course;
};
typedef struct studentCourse studentCourse_t;

struct timetable
{
	int coursesAMT;
	studentCourse_t enrolledCRSE[MAX_ENROLLMENT];
	course_t course;
};
typedef struct timetable timetable_t;*/

struct student
{
	int number;
	char firstname[20];
	char lastname[20];
	char password[20] ;
	personal_t personalInfo;
	/*timetable_t enrollments;*/
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
void checkPassword(student_t stup);
void setPassword(student_t* stup);
int saveStudentChanges(studentNode_t* head, student_t stu);
int studentMain(void);
void printStuMenu(int* choicep);
void printStuTimetable(int stdNum, course_t AllCourses[], int coursesAMT);
void printStuDetails(student_t student);
int adminMain(void);
void printAdminMenu(int* choicep);
void addStu(studentNode_t** head);
int removeStu(studentNode_t** head);
int editStu(studentNode_t* head, student_t stu);
void printEditMenu(int* choicep);
void printStu(studentNode_t* head, course_t AllCourses[], int coursesAMT);
void printCourse(course_t AllCourses[], int coursesAMT);
int validCourseNum(int courseNum);
int validClassName(char className[]);
int validGroupNum(int groupNum);
void chooseClass(int* courseNum, char classType[], int* group);
void printClass(studentNode_t* head, course_t AllCourses[], int coursesAMT);
void printEnrollment(course_t AllCourses[], int coursesAMT);
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
int validPhoneNumber(long phoneNum);
int validDate(int day, int month, int year);
int FindCourse(int courseNum, course_t AllCourses[],
	int coursesAMT);
void EnrollAStudent(studentNode_t* head, course_t AllCourses[], 
	int coursesAMT);
int loadEnrollments(course_t AllCourses[], int CoursesAMT, 
	studentNode_t* head);
int saveEnrollments(course_t AllCourses[], int CoursesAMT);
void DisenrollAStudent(studentNode_t* head, course_t AllCourses[], 
	int coursesAMT);

#endif
