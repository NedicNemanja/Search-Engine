#ifndef STRINGMANIPULATION_H
#define STRINGMANIPULATION_H

#include "Highlight.h"

/*Skip whitespace and return the first non-whitespace char you find.*/
char SkipWhitespace(FILE* fp);

/*Print a document to terminal, make it wrap and highlight the words relevant to
the querry.*/
void PrintDocumentToTerminal(char* doc, Highlight* Highlights,int numofHlights);

/*print n char of a doc string.*/
void PrintChars(char* doc, int* index, int n);

void PrintWhitespace(int n);

int PrintHighlights( int carry, int* hindex, int line_end,
                      Highlight* Hlights,int* h,int numofHlights);
                      
/*
/*Get the pointer of a word IN the document, that points at the first word
after offset-many characters*
char* getNextWord(char* doc, int* offset, int* word_size);

/*ignore and count every char in fstream until \n or EOF*
char GoToEndOfline(FILE* fp, int* DSize);
*/
#endif
