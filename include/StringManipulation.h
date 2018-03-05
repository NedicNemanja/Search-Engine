#ifndef STRINGMANIPULATION_H
#define STRINGMANIPULATION_H

/*Skip whitespace and return the first non-whitespace char you find.*/
char SkipWhitespace(FILE* fp);

/*Read a stream of char from fp, and append them to the doc string until you find
whitespace,newline or EOF.
That last argument is valuable, its the first char we append.
While appending characters to the doc string,you might get out of its boundaries.
ReallocDocument() is used to resize our document and update the doc pointer in
the DMAP.*/
char AddWord(FILE* fp,char** doc_ptr, int* doc_size, int* char_index, char* c);

/*
/*Get the pointer of a word IN the document, that points at the first word
after offset-many characters*
char* getNextWord(char* doc, int* offset, int* word_size);

/*ignore and count every char in fstream until \n or EOF*
char GoToEndOfline(FILE* fp, int* DSize);
*/
#endif
