/******************************************************************************
* Author: Michael Cooke
* Date: 26/09/2019
* Description: Encrypts/Decrypts a text file via XOR encryption with a varible
* ceasar shift to protect against brute force attacks
******************************************************************************/
#include "Encrypt.h"
/******************************************************************************
* MAIN
******************************************************************************/

int main (int argc, char* argv[])
{
	/*Check a file and run_mode have been entered*/
	if (argc < 2)
	{
		printf("Incorrect format\n");
		printf("Correct is %s runmode argument_file\n",argv[0]);
	}
	else if (strcmp(argv[1],"-e") == 0)
	{
		char* input_file = argv[2];
		EncryptDecrypt(input_file, 0);
	}
	else if (strcmp(argv[1],"-d") == 0)
	{
		char* input_file = argv[2];
		EncryptDecrypt(input_file, 1);
	}
	else if (strcmp(argv[1],"-debug"))
	{
		char* input_file = argv[2];
		EncryptDecrypt(input_file, 2);
	}
	else
	{
		printf("Please enter -e to encrypt or -d to decrypt\n");
		printf("-debug can be entered to run in symmetric debug mode\n");
	}
	return 0;
}

/******************************************************************************
* Allocates to other functions and initiates main varaibles
******************************************************************************/
void EncryptDecrypt (char file_name[], int mode)
{
	char pass[20];
	int passlen;
	int filelen = getFilelen(file_name, mode);
	char fileSeq[filelen], passSeq[filelen], outSeq[filelen];
	
	getFileSeq(fileSeq, file_name, mode);
	getPass(pass, &passlen, mode);
	getPassSeq(filelen, passSeq, pass, passlen, mode);
	XORencrypt(fileSeq, passSeq, outSeq, filelen, mode);
	fileconvert(file_name, outSeq, filelen, mode);
}
/******************************************************************************
* Determines file lenght in order to create array lenghts
* NOTE: This cannot be done with a seek stlye function
******************************************************************************/
int getFilelen (char file_name[], int mode)
{
	long size = 0;
	char ch;
	FILE* = fp;
	fp = fopen(file_name,"r");
	if (fp == NULL)
	{
		printf("Can't open file\n");
	}
	else
	{
		if (mode == 2)
		{
			printf("File sucessfully opened\n");
		}
		for (ch=fgetc(fp); ch!=EOF; ch=fgetc(fp))		
		{
			size++;
		}
	}
	close(fp);
	return size;
}
/******************************************************************************
* Reads the file and enters it into memory
******************************************************************************/
void getFileSeq (char fileSeq[], char file_name[], int mode)
{
	int i=0;
	char ch;
	FILE* = fp;
	fp = fopen(file_name,"r");
	if (fp == NULL)
	{
		printf("Can't open file\n");
	}
	else
	{
		for (ch=fgetc(fp); ch!=EOF; ch=fgetc(fp))		
		{
			fileSeq[i] = ch;
			i++;
		}
	}
	fclose(fp);
}
/******************************************************************************
* Asks user to enter a password and checks the length
******************************************************************************/
void getPass (char pass[], int* passlen, int mode)
{
	do
	{
		if (mode == 0)
		{
			do
			{
				printf("Enter a password\n");
				scanf("%s",pass);
				*passlen = strlen(pass);
			}while (passlen<5);
		}
		else if (mode == 1)
		{
			printf("Enter the password you used to encrypt\n");
			scanf("%s",pass);
			*passlen = strlen(pass);
		}
		else if (mode == 2)
		{
			printf("Enter your password\n");
			scanf("%s",pass);
			*passlen = strlen(pass);
			if (passlen<5)
			{
				printf("A longer password would be safer\n");
				printf("Re-enter your short password ");
				printf("or change to a new one\n");
				scanf("%s",pass);
				*passlen = strlen(pass);
				printf("Password lenght = %d\n",passlen);
			}
		}
	}while(passlen<5);
}

/******************************************************************************
* Creates a key of lenght equal to the filelenght using the password
******************************************************************************/
void getPassSeq (int filelen, char passSeq[], char pass[], 
				 int passlen, int mode)
{	
	int i, j, multi, remainder;
	multi = filelen/passlen;
	remainder = filelen%passlen;
	
	for (i=0; i<multi; i++)
	{
		for (j=0; j<passlen; j++)
		{
			passSeq[i*passlen+j] = pass[j] + i;
		}
	}
	for (j=0; j<remainder; j++)
	{
		passSeq[multi*passlen+j] = pass[j] + i;
	}
	if (mode == 2)
	{
		printf("Password Sequence generated");
	}
}

/******************************************************************************
* XOR password sequence and file contents
******************************************************************************/
void XORencrypt (char fileSeq[], char passSeq[], char outSeq[], 
				 int filelen, int mode)
{	
	int i;	
	if (mode == 2)
	{
		printf("Generating output\nBit level = IN PASS OUT\n");
	}
	for (i=0; i<filelen; i++)
	{
		outSeq[i] = fileSeq[i] ^ passSeq[i];
		if (mode == 2)
		{
			printf("I%d P%d O%d "fileSeq[i], passSeq[i], outSeq[i]);
		}
	}
	if (mode == 2)
	{
		printf("\n");
	}
}
/******************************************************************************
* Rewrites the file
NOTE: Cannot string print due to possible ASCII conflicts
******************************************************************************/
void fileconvert (char file_name[], char outSeq[], int filelen, int mode)
{
	FILE* = fp;
	int i;
	fp = fopen (file_name,"w");
	if (fp == NULL)
	{
		printf("Can't open file\n");
	}
	else
	{
		for (i=0;i<filelen;i++)
		{
			fputc(outSeq[i],fp);
		}
		if (mode == 2)
		{
			printf("File encrypted/decrypted\nTo undo this ");
			printf("rerun the program with the same password just entered\n");
		}
	}
	fclose(fp);
}
