/******************************************************************************
* Author: Michael Cooke
* Date: 26/09/2019
* Description: Encrypts/Decrypts a text file via XOR encryption with a varible
* ceasar shift to protect against brute force attacks. Different runmodes are * featured for user friendlyness and do not affect functionality.
* Runmodes:0- Default two layer encryption
*          1- Default two layer decryption
*          2- Multi layer encryption remember the order of passwords
*          3- Multi layer decryption remember to enter in reverse order
*          4- DEBUG symmentic signle layer

NOTE ON DEBUG MODE
The functional part of the program is symmetric hence this can be used for either encryption or decryption and will display the bit level along with some milestone markers in order to determine that the program is functioning as intended. 
******************************************************************************/
#include "Encrypt.h"

/******************************************************************************
* MAIN - incase this is to be run stand-alone
******************************************************************************/
int main (int argc, char* argv[]) 
{
	/*Check a file and run_mode have been entered*/
	 if (argc < 2)
	{
		printf("Incorrect format\n");
		printf("Correct is %s runmode argument_file\n",argv[0]);
	} 
	/*Enter correct runmode*/
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
	else if (strcmp(argv[1],"-me") == 0)
	{
		char* input_file = argv[2];
		EncryptDecrypt(input_file, 2);
	}
	else if (strcmp(argv[1],"-md") == 0)
	{
		char* input_file = argv[2];
		EncryptDecrypt(input_file, 3);
	}
	else if (strcmp(argv[1],"-debug") == 0)
	{
		char* input_file = argv[2];
		EncryptDecrypt(input_file, 4);
	} */
	/*Incase of invalid entry*/
	else
	{
		printf("Please enter -e to encrypt or -d to decrypt\n");
		printf("-debug can be entered to run in symmetric debug mode\n");
	}
	return 0;
}

/******************************************************************************
* Description: Allocates to other functions and initiates main varaibles
* IN: A file to encrypt/decrypt and a runmode(see top)
* OUT: NONE
******************************************************************************/
void EncryptDecrypt (char file_name[], int mode)
{
	char pass[MAX_PASS_LEN];
	int pass_len = 0;
    int i = 0;
    int multi = 0;
	int file_len = getFilelen(file_name, mode);
	char file_seq[file_len], pass_seq[file_len], out_seq[file_len];
	getFileSeq(file_seq, file_name, file_len, mode);
	/*Check for multi layer*/
	if (mode == 2)
	{
		printf("How many layers of encryption\n");
		scanf("%d",&multi);
	}
	else if (mode == 3)
	{
		printf("How many layers of encryption were applied?\n");
		scanf("%d",&multi);
		printf("To decrypt apply your previous passwords in reverse order\n");
	}/*Default encrypt*/
	else if (mode == 0)
	{
		strcpy(pass,"student");
		pass_len = strlen(pass);
		getPassSeq(file_len, pass_seq, pass, pass_len, mode);
		XORencrypt(file_seq, pass_seq, out_seq, file_len, mode);
		strcpy(pass,"default");
		pass_len = strlen(pass);
		getPassSeq(file_len, pass_seq, pass, pass_len, mode);
		XORencrypt(file_seq, pass_seq, out_seq, file_len, mode);
	}/*Default decrypt*/
	else if (mode == 1)
	{
		strcpy(pass,"default");
		pass_len = strlen(pass);
		getPassSeq(file_len, pass_seq, pass, pass_len, mode);
		XORencrypt(file_seq, pass_seq, out_seq, file_len, mode);
		strcpy(pass,"student");
		pass_len = strlen(pass);
		getPassSeq(file_len, pass_seq, pass, pass_len, mode);
		XORencrypt(file_seq, pass_seq, out_seq, file_len, mode);
	}/*Set to run once*/
	else
	{
		multi = 1;
	}
	/*Multi layer implementation*/
	for (i=0; i<multi; i++)
	{
		getPass(pass, &pass_len, mode);
		getPassSeq(file_len, pass_seq, pass, pass_len, mode);
		XORencrypt(file_seq, pass_seq, out_seq, file_len, mode);
	}
	fileconvert(file_name, out_seq, file_len, mode);
}
/******************************************************************************
* Description: Determines file lenght in order to create arrays
* IN: The file being encrypted/decrypted and the runmode(see top)
* OUT: Lenght of the file
* NOTE: fseek has been used hence file is now modified 
******************************************************************************/
int getFilelen (char file_name[], int mode)
{
	long size = 0;
	FILE* fp = NULL;
	fp = fopen(file_name,"r");
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	fclose(fp);
	
	if (mode == 4)
	{
		printf("size = %ld\n",size);
	}
	return size;
}
/******************************************************************************
* Description: Reads the file and enters all contents into memory
* IN: Location to store information, file to read, lenght of the file and 
*     runmode
* OUT: NONE
* NOTE: EOF is not used due to prior fseek
******************************************************************************/
void getFileSeq (char file_seq[], char file_name[], int file_len, int mode)
{
	int i;
	FILE* fp = NULL;
	fp = fopen(file_name,"r");
	if (fp == NULL)
	{
		printf("Can't open file\n");
	}
	else
	{
		for (i=0; i<file_len; i++)		
		{
			file_seq[i] = fgetc(fp);
		}
		if (mode == 4)
		{
			printf("Sequence acquired\n");
		}
	}
	fclose(fp);
}
/******************************************************************************
* Description: Asks user to enter a password and checks the length of the it
* IN: Password storage location, storage location for password lenght and
*     runmode
* OUT: NONE
******************************************************************************/
void getPass (char pass[], int* pass_len, int mode)
{
	if (mode == 0||mode == 2)
	{
		do
		{
			printf("Enter a password atleast 5 characters long\n");
			scanf("%s",pass);			
			*pass_len = strlen(pass);
		}while (*pass_len<5);
	}
	else if (mode == 1||mode == 3)
	{
		printf("Enter the password you used to encrypt\n");
		scanf("%s",pass);
		*pass_len = strlen(pass);
	}
	else if (mode == 4)
	{
		printf("Enter your password\n");
		scanf("%s",pass);
		*pass_len = strlen(pass);
		if (*pass_len<5)
		{/*Option for shorter password in DEBUG mode*/
			printf("A longer password would be safer\n");
			printf("Re-enter your short password ");
			printf("or change to a new one\n");
			scanf("%s",pass);
			*pass_len = strlen(pass);
			printf("Password lenght = %d\n",*pass_len);
		}
	}
}

/******************************************************************************
* Description: Creates a key of lenght equal to the filelenght using the 
*              password which is copied and ceasar shifted
* IN: Filelenght, password array, password lenght and runmode
* OUT: NONE
******************************************************************************/
void getPassSeq (int file_len, char pass_seq[], char pass[], 
				 int pass_len, int mode)
{	
	/*Math for how many times the password is copied*/
	int i, j, multi, remainder;
	multi = file_len/pass_len;
	remainder = file_len%pass_len;
	/*Magic(copy and shift)*/
	for (i=0; i<multi; i++)
	{
		for (j=0; j<pass_len; j++)
		{
			pass_seq[i*pass_len+j] = pass[j] + i;
			/*
			for (j>*pass_len)
			{
				pass[j] = pass[j] + pass_seq[j-*pass_len];
			}
			pass_seq[i*pass_len+j] = pass[j]
			*/
		}
	}
	for (j=0; j<remainder; j++)
	{
		pass_seq[multi*pass_len+j] = pass[j] + i;
			/*
			for (j>*pass_len)
			{
				pass[j] = pass[j] + pass_seq[j-*pass_len];
			}
			pass_seq[multi*pass_len+j] = pass[j]
			*/
	}
	if (mode == 4)
	{
		printf("Password Sequence generated\n");
	}
}

/******************************************************************************
* Description: XOR's password sequence and file contents
* IN: File contents, key, a storage array for output, file lenght and runmode
* OUT: NONE
******************************************************************************/
void XORencrypt (char file_seq[], char pass_seq[], char out_seq[], 
				 int file_len, int mode)
{	
	int i;	
	if (mode == 4)
	{
		printf("Generating output\nBit level = IN PASS OUT\n");
	}
	for (i=0; i<file_len; i++)
	{
		out_seq[i] = file_seq[i] ^ pass_seq[i];
		if (mode == 4)
		{	/*DISPLAYS BIT LEVEL FOR DEBUG MODE*/
			printf("I%d P%d O%d ",file_seq[i], pass_seq[i], out_seq[i]);
		}
		file_seq[i] = out_seq[i];
	}
	if (mode == 4)
	{
		printf("\n");
	}
}
/******************************************************************************
* Description: Rewrites the file with encrypted/decrypted version
* IN: File name, output array, file lenght and runmode
* OUT: NONE
* NOTE: Cannot string print due to possible ASCII conflicts. Solution is to 
*       treat the file like a binary file and ignore display issues
******************************************************************************/
void fileconvert (char file_name[], char out_seq[], int file_len, int mode)
{
	int i;
	FILE* fp = NULL;
	fp = fopen (file_name,"w");
	if (fp == NULL)
	{
		printf("Can't open file\n");
	}
	else
	{
		for (i=0;i<file_len;i++)
		{
			fputc(out_seq[i],fp);
		}
		if (mode == 4)
		{
			printf("File encrypted/decrypted\n");
		}
	}
	fclose(fp);
}
