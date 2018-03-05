#include <stdio.h>
#include <stdlib.h>
#include "ErrorCodes.h"
#include "ReallocDocument.h"

/*Skip whitespace and return the first non-whitespace char you find.*/
char SkipWhitespace(FILE* fp){
  char c;
  do{
      c = fgetc(fp);
  }while(c == ' ' || c == '\t');
  return c;
}

/*Read a stream of char from fp, and append them to the doc string until you find
whitespace,newline or EOF.
That last argument is valuable, its the first char we append.
While appending characters to the doc string,you might get out of its boundaries.
ReallocDocument() is used to resize our document and update the doc pointer in
the DMAP.*/
void AddWord(FILE* fp, char** doc_ptr, int* doc_size, int* char_index, char* c){
  char* doc = *doc_ptr;
  //while the word has not ended
  while(*c != ' ' && *c != '\t' && *c != '\n' && *c != EOF){
    //make document bigger if needed
    if(*char_index >= *doc_size){
      *doc_ptr = ReallocDocument(doc,doc_size);
      doc = *doc_ptr;
      NULL_Check(doc_ptr);
    }
    doc[*char_index] = *c;
    (*char_index)++;
    *c = fgetc(fp);
  }
}




/*Find where the next word in the document starts.
Keep track of the offset inside the document and count the size of the word.
If no more words are found return NULL.*/
char* getNextWord(char* doc, int* offset, int* word_size){
  *word_size = 0;
  //exception for end of document
  if(doc[*offset] == '\0')
    return NULL;

  //find where the word starts
  while((doc[*offset] == ' ' || doc[*offset] == '\t') && doc[*offset]!='\0')
    (*offset)++;
  int start = *offset;
  //exception for end of document
  if(doc[*offset] == '\0')
    return NULL;

  //read the word
  while(doc[*offset] != ' ' && doc[*offset] != '\t' && doc[*offset] != '\0'){
    (*offset)++;
    (*word_size)++;
  }
  return doc+start;
}

/*ignore and count every char in fstream until \n or EOF*/
char GoToEndOfline(FILE* fp, int* DSize){
  char c = fgetc(fp);	//ignore the space between index and 1st word
  while(c != '\n' && c!=EOF){
    c=fgetc(fp);
    if(c!='\n' && c!=EOF)
      (*DSize)++;
  }
  return c;
}


/*diff version unused
char* getnextWord(char* doc, int* offset){
  //exception for end of document
  if(doc[*offset] == '\0')
    return NULL;

  //if this is the first word, we already know where it starts
  if(*offset == 0){
    while(doc[*offset] != ' ' && doc[*offset] != '\t' && doc[*offset] != '\0'){
        printf("%d%c\n", *offset,doc[*offset]);
      (*offset)++;  //just increase offset until we pass this word
    }
    return doc;
  }

  //else skip until the whitespace
  while(doc[*offset] != ' ' && doc[*offset] != '\t' && doc[*offset] != '\0')
    (*offset)++;
  //exception for last word of the document
  if(doc[*offset-1] == '\0')
    return NULL;

  //skip the whitespace
  while(doc[*offset] == ' ' || doc[*offset] == '\t')
    (*offset)++;
  //exception for the whitespace at the end of the document
  if(doc[*offset-1] == '\0')
    return NULL;

  return doc+(*offset);
}
*/
