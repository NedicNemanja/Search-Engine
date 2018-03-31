#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "StringManipulation.h"
#include "ErrorCodes.h"
#include "ReallocDocument.h"
#include <sys/ioctl.h>
#include "QuickSort.h"

/*Skip whitespace and return the first non-whitespace char you find.*/
char SkipWhitespace(FILE* fp){
  char c;
  do{
      c = fgetc(fp);
  }while(c == ' ' || c == '\t');
  return c;
}


void PrintDocumentToTerminal(char* doc, Highlight* Highlights,int numofHlights){
  //get terminal window size
  struct winsize w;
  ioctl(fileno(stdout),TIOCGWINSZ,&w);

  int gap = 23; //whitespace gap at the start of the line
  int avl_columns = w.ws_col - gap; //calc the available columns for printing
  int doc_size = strlen(doc);
  int dindex = 0;  //index inside the document
  int hindex = 0;  //index inside the highlight
  int h=0;  //upcoming(next) highlight index
  /*sometimes we ll stop a highlight before its finished,
    what's left we ll carry to the next line.*/
  int carry=0;

  /*print the document in chunks of avl_col characters
    and under every chunk show the highlight^^^^^^^^^^*/
  //we need to sort the Highlights
  QuickSortHighlights(Highlights,0,numofHlights-1);

  PrintChars(doc,&dindex,avl_columns);
  PrintWhitespace(gap);
  carry = PrintHighlights(carry,&hindex,avl_columns-1,Highlights,&h,numofHlights);
  while(dindex <= doc_size-1){
    PrintWhitespace(gap);
    PrintChars(doc,&dindex,avl_columns);
    PrintWhitespace(gap);
    carry = PrintHighlights(carry,&hindex,avl_columns-1, Highlights, &h, numofHlights);
  }

}

int PrintHighlights(int carry, int* hindex, int line_end,
                      Highlight* Hlights,int* h,int numofHlights){
  int line_index = 0, hbase_index = *hindex;

  //print carry of the last highlight
  if(carry != 0){
    while(carry>0){
      if(line_index>line_end)
        return carry;;
      printf("^");
      (*hindex)++;
      line_index++;
      carry--;
    }
    (*h)++;
  }

  while(*h<=numofHlights-1){
    //print whitespace until the next highlight or until end of line
    while(hbase_index+line_index < Hlights[*h].start){
      if(line_index>line_end)
        return carry;
      printf(" ");
      (*hindex)++;
      line_index++;
    }
    //then print the highlight
    for(int i=0; i<Hlights[*h].size; i++){
      if(line_index>line_end){
        /*if we break a highlight in the middle
        we must carry what remains to the next line*/
        carry = Hlights[*h].size-i;
        return carry;
      }
      printf("^");
      (*hindex)++;
      line_index++;
    }
    (*h)++;
  }
  //print whitespace after the highlights
  while(line_index<=line_end){
    printf(" ");
    (*hindex)++;
    line_index++;
  }
  return carry;
}

/*print n char of a doc string,
return 1 if the string ends before chars are printed.*/
void PrintChars(char* doc, int* index, int n){
  for(int i=0; i<n; i++){
    if(doc[(*index)] == '\0'){
      //if you reach the end of document
      //fill it with whitespace
      for(int j=i; j<n; j++)
        printf(" ");
      return;
    }
    printf("%c", doc[(*index)]);
    (*index)++;
  }
}

void PrintWhitespace(int n){
  for(int i=0; i<n; i++){
    printf(" ");
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
