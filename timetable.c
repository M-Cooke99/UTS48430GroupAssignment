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
	return 0;
}

/* GITHUB PUSH TEST */



/******************************************************************************
 * Prints student menu options
******************************************************************************/
void printStudentMenu(int* choicep){
	
}

/******************************************************************************
 * Read student number, password and check validity. maybe three seperate 
 * functions?
******************************************************************************/

/******************************************************************************
 * Admin main: menu options
******************************************************************************/
int adminMain(void){
	int choice;
	return 0;
}

/******************************************************************************
 * Prints admin menu options
******************************************************************************/
void printAdminMenu(int* choicep){
	
}

