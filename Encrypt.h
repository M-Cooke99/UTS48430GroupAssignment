/******************************************************************************
* Author: Michael Cooke
* Date: 26/09/2019
* Description: Encrypts/Decrypts a text file via XOR encryption with a varible
* ceasar shift to protect against brute force attacks. Different runmodes are * featured for user friendlyness and do not affect functionality.
* Runmodes:0- Default two layer encryption
*          1- Default two layer decryption
*          2- Multi layer encryption remember the order of passwords
*          3- Multi layer decryption remember to enter in reverse order
*          4- DEBUG symmentric single layer

NOTE ON DEBUG MODE
The functional part of the program is symmetric hence this can be used for either encryption or decryption and will display the bit level along with some milestone markers in order to determine that the program is functioning as intended. 
******************************************************************************/

#ifndef ENCRYPT_H
#define ENCRYPT_H

#include <stdio.h>
#include <string.h>

#define MAX_PASS_LEN 20
/******************************************************************************
* Description: Allocates to other functions and initiates main varaibles
* IN: A file to encrypt/decrypt and a runmode(see top)
* OUT: NONE
******************************************************************************/
void EncryptDecrypt (char file_name[], int mode);
/******************************************************************************
* Description: Asks user to enter a password and checks the length of the it
* IN: Password storage location, storage location for password lenght and
*     runmode
* OUT: NONE
******************************************************************************/
void getPass (char pass[], int* pass_len, int mode);
/******************************************************************************
* Description: Determines file lenght in order to create arrays
* IN: The file being encrypted/decrypted and the runmode(see top)
* OUT: Lenght of the file
* NOTE: fseek has been used hence file is now modified 
******************************************************************************/
int getFilelen (char file_name[], int mode);
/******************************************************************************
* Description: Creates a key of lenght equal to the filelenght using the 
*              password which is copied and ceasar shifted
* IN: Filelenght, password array, password lenght and runmode
* OUT: NONE
******************************************************************************/
void getPassSeq (int file_len, char pass_seq[], char pass[], int pass_len,
	int mode);
/******************************************************************************
* Description: Reads the file and enters all contents into memory
* IN: Location to store information, file to read, lenght of the file and 
*     runmode
* OUT: NONE
* NOTE: EOF is not used due to prior fseek
******************************************************************************/
void getFileSeq (char file_seq[], char file_name[], int file_len, int mode);
/******************************************************************************
* Description: XOR's password sequence and file contents
* IN: File contents, key, a storage array for output, file lenght and runmode
* OUT: NONE
******************************************************************************/
void XORencrypt (char file_seq[], char pass_seq[], char out_seq[],
	int file_len, int mode);
/******************************************************************************
* Description: Rewrites the file with encrypted/decrypted version
* IN: File name, output array, file lenght and runmode
* OUT: NONE
* NOTE: Cannot string print due to possible ASCII conflicts. Solution is to 
*       treat the file like a binary file and ignore display issues
******************************************************************************/
void fileconvert (char file_name[], char out_seq[], int filel_en, int mode);

#endif 
