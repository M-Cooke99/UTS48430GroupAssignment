#ifndef ENCRYPT_H
#define ENCRYPT_H

#include <stdio.h>
#include <string.h>

FILE* fp;

void EncryptDecrypt (char file_name[], int mode);
void getPass (char pass[], int* passlen, int mode);
int getFilelen (char file_name[], int mode);
void getPassSeq (int filelen, char passSeq[], char pass[], int passlen, int mode);
void getFileSeq (char fileSeq[], char file_name[], int mode);
void XORencrypt (char fileSeq[], char passSeq[], char outSeq[], int filelen, int mode);
void fileconvert (char file_name[], char outSeq[], int filelen, int mode);

#endif 
