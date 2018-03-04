#ifndef STRINGMANIPULATION_H
#define STRINGMANIPULATION_H
/*Get the pointer of a word IN the document, that points at the first word
after offset-many characters*/
char* getNextWord(char* doc, int* offset, int* word_size);

/*ignore and count every char in fstream until \n or EOF*/
char GoToEndOfline(FILE* fp, int* DSize);

#endif
