#ifndef ENCRYPT_H
#define ENCRYPT_H

#include <stdio.h>
#include <string.h>

FILE* fp;

void EncryptDecrypt (char file_name[]);
void getPass (char pass[], int* passlen);
int getFilelen (char file_name[]);
void getPassSeq (int filelen, char passSeq[], char pass[], int passlen);
void getFileSeq (char fileSeq[], char file_name[]);
void XORencrypt (char fileSeq[], char passSeq[], char outSeq[], int filelen);
void fileconvert (char file_name[], char outSeq[], int filelen);

#endif 
