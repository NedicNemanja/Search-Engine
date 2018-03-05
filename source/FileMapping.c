#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FileMapping.h"
#include "ErrorCodes.h"
#include "StringManipulation.h"
//#include "Trie.h"

int MEDIAN_DOC_SIZE = 5; /*median size of a document in characters without
                          the index and \0. Consecutive whitespace counts as
                          1 character.*/

int MapAndTrie(char* docfilename){
  //open file for reading
  FILE* fp = fopen(docfilename, "r");
  if(!fp){
    fprintf(stderr,"Can't open file %s", docfilename);
    return CANT_OPEN_FILE;
  }
  DMAP.size = 0;

  char c;
  //read the file
  do{ //for every document
    int doc_index;
    //read doc_index
    int temp = fscanf(fp,"%d",&doc_index);
    if( temp == 0){ //if the line is unindexed
      fprintf(stderr,"Unindexed line %d\n", doc_index+1);
      fclose(fp);
      return UNINDEXED_LINE;
    }
    else if(temp == -1){  //if fscanf found EOF
      fclose(fp);
      break;
    }
    //check if its sorted
    if(doc_index != DMAP.size){
      fprintf(stderr, "Documents inside file and not sorted."
                      "Expected document %d, got document %d.\n"
                      ,DMAP.size,doc_index);
      fclose(fp);
      return UNORDERED_DOCUMENTS;
    }
    /****index check passed****/
    //resize the map
    DMAP.size++;
    DMAP.map = realloc(DMAP.map,sizeof(char*)*DMAP.size);
    NULL_Check(DMAP.map);
    //allocate space for this document in the map
    int doc_size = MEDIAN_DOC_SIZE;  //this one is the total doc size
    DMAP.map[doc_index] = malloc(sizeof(char)*(doc_size +1)); //+1 for the \0
    NULL_Check(DMAP.map[doc_index]);
    int char_index = -1;
    //read the document word by word
    do{
      //find where the word starts
      c = SkipWhitespace(fp);
      char_index++;
      if(c != '\n' && c != EOF){  //if you found a word
        int word_start = char_index;
        //append the word to the document (increase doc_size if needed)
        AddWord(fp,&(DMAP.map[doc_index]),&doc_size,&char_index,&c);
        DMAP.map[doc_index][char_index] = ' ';
        int word_size = char_index - word_start;
        //TrieInsert(DMAP.map[d],word_start,word_size);
      }
    }while(c != '\n' && c != EOF);
    DMAP.map[doc_index][char_index] = '\0';
    printf("document: %s\n", DMAP.map[doc_index]);
    //most likely there will be more space allocated than needed for a document
    //ResizeDocument(doc_size,char_index);
    printf("--------------------------------------------\n");
  }while(c != EOF);
  return OK;
}

/******************************************************************************
************************DIFFERENT APPROACH*************************************
****************************************************************************

Check if the file is sorted and learn the size of each document.
ERRORCODE ScanFile(FILE* fp, int** DSizesptr){
  //read the index of each line, when order breaks stop
  int doc_index=-1,temp;
  char endofline;
  int* DSizes = *DSizesptr;
  do{ //for every document
    //get the document index
    temp=fscanf(fp, "%d", &doc_index);
    if(temp>=1){  //check if the line is indexed
      //read the document till end of line and learn its size
      DMAP.size++;
      DSizes = realloc(DSizes,sizeof(int)*DMAP.size);
      NULL_Check(DSizes);
      DSizes[DMAP.size-1]=0;
      endofline = GoToEndOfline(fp,&DSizes[DMAP.size-1]);
    }
    else if(temp == 0){ //the line is unindexed
      fprintf(stderr,"Unindexed line %d\n", doc_index+1);
      return UNINDEXED_LINE;
    }
    else if(temp == -1){  //you reached the EOF
      endofline = EOF;
      break;
    }
  }while(doc_index == DMAP.size-1 && endofline!=EOF);
  *DSizesptr = DSizes;	//just updating the original variable

  if(endofline == EOF)
    return OK;
  //else order was broken
  fprintf(stderr, "Documents inside file and not sorted."
                  "Expected document %d, got document %d.\n"
                  ,DMAP.size-1,doc_index);
  fclose(fp);
  return UNORDERED_DOCUMENTS;
}

void FapAndTrie(int* DSizes, FILE* fp){
  int temp;
  //allocate a pointer in the map for each document
  DMAP.map = malloc(sizeof(char*)*DMAP.size);
  NULL_Check(DMAP.map);

  for(int i=0; i<DMAP.size; i++){ //for every document
    //put the document-i in the DMAP
    DMAP.map[i] = malloc(sizeof(char)*(DSizes[i]+1)); //+1 for terminating char
    NULL_Check(DMAP.map);
    fscanf(fp, "%d", &temp);  //ignore the document index
    fgetc(fp);                //and the space after it
    //get and store the document to the map
    fgets(DMAP.map[i],DSizes[i]+1,fp);/*note: fgets() reads until specified size
                                      DSizes[i] or \n. As DSizes[i] describes
                                      the original string with the index,
                                      removing the index produces a smaller
                                      string.
                                      So it is certain that fgets() will stop
                                      at the \n. But it wont be saved to the map
                                      because we only allocated just enough for
                                      the string without the \n.
    //break the document into words and put them in the trie
    //TrieInit();
    int offset=0,word_size;
    char* word = NULL;
    do{ //for every word in the document
      word = getNextWord(DMAP.map[i],&offset,&word_size);
      if(word != NULL){
        //TrieInsert(word);
        printf("word:%s\n", word);
        printf("document:%s\n", DMAP.map[i]);
      }
    }while(word != NULL);
    printf("----------------------new document------------\n");
  }
}

/*Reads the file once and allocates the needed memory for it, if its sorted.
Then read the file again and store the documents to DMAP.
int FapFile(char* docfilename){
  //open file for reading
  FILE* fp = fopen(docfilename, "r");
  if(!fp){
    fprintf(stderr,"Can't open file %s", docfilename);
    return CANT_OPEN_FILE;
  }

  //scan the file to learn if its sorted, and the size of each document
  int* DSizes=NULL;
  DMAP.size = 0;
  if(ScanFile(fp,&DSizes) != OK){
    fprintf(stderr, "%s file cannot be mapped.\n", docfilename);
    free(DSizes);
    return UNORDERED_DOCUMENTS;
  }

  //read file again and create the trie and map structures
  rewind(fp);
  MapAndTrie(DSizes,fp);
  return OK;
}
*/
