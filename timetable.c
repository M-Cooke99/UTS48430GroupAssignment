#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

void studentMain(void);
void adminMain(void);

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
void studentMain(void){

}

/******************************************************************************
 * Admin main: menu options
******************************************************************************/
void adminMain(void){

}
