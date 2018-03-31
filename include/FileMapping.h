#ifndef FILEMAPPING_H
#define FILEMAPPING_H
#include <stdio.h>

//all the documents are kept here, indexing is the same as in the input file
typedef struct DocumentMAP{
  int size;     //number of documents in the map array
  char** map;  //flexible array for storing documents
  int* doc_lenght;  //array for keping track of num of words in each document
} DocumentMAP;

/******************global variables*******************************************/
//This is our MAP
DocumentMAP DMAP;

int FILE_SIZE;     /* total count of all doc sizes in the file
                      i.e not counting index. We use this to keep the
                      MEAN_DOC_SIZE up to date.*/
int MEAN_DOC_SIZE; /* mean size of a document in characters without
                      the index and \0. Consecutive whitespace counts as
                      1 character.*/
int WORDS_IN_FILE; /* total number of words in the file.*/

int MapAndTrie(FILE* fp);

/******************************************************************************
****************************Local members - NOT used externally****************
******************************************************************************/

void UpdateMEAN_DOC_SIZE(int new_doc_size);
void PrintMAP();
void AddWord(FILE* fp, char** doc_ptr, int* doc_size, int* char_index, char* c);
void NewDocument(int doc_size);
void FreeMap();

#endif
