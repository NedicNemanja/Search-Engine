#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "FileMapping.h"
#include "ErrorCodes.h"
#include "StringManipulation.h"
#include "ReallocDocument.h"
#include "Trie.h"


//This is our MAP
DocumentMAP DMAP = { 0, NULL, NULL};

int FILE_SIZE = 0;      /*total count of all doc sizes in the file
                          i.e not counting index. We use this to keep the
                          MEAN_DOC_SIZE up to date.*/
int MEAN_DOC_SIZE = 5;  /*mean size of a document in characters without
                          the index and \0. Consecutive whitespace counts as
                          1 character.*/
int WORDS_IN_FILE = 0;  /*total num of wrods in file.*/




/************************Reading file to create Map and Trie*******************/

int MapAndTrie(FILE* fp){
  char c;
  do{ //for every document
    /********************check indexing****************************************/
    int doc_index;
    int temp = fscanf(fp,"%d",&doc_index);
    if( temp == 0){ //if the line is unindexed
      fprintf(stderr,"Unindexed line %d\n", doc_index+1);
      //free initalized space
      FreeTrie(TrieRoot);
      TrieRoot = NULL;
      FreeList(PLIST.next);
      PLIST.next = NULL;
      FreeMap();
      return UNINDEXED_LINE;
    }
    else if(temp == -1){  //if fscanf found EOF
      break;
    }
    //check if its sorted
    if(doc_index != DMAP.size){
      fprintf(stderr, "Documents inside file and not sorted."
                      "Expected document %d, got document %d.\n"
                      ,DMAP.size,doc_index);
      //free initalized space
      FreeTrie(TrieRoot);
      TrieRoot = NULL;
      FreeList(PLIST.next);
      PLIST.next = NULL;
      FreeMap();
      return UNORDERED_DOCUMENTS;
    }
    /*********index check passed****add a new document to the map**************/
    int doc_size = MEAN_DOC_SIZE;
    NewDocument(doc_size);
    int char_index = -1;

    //read the document word by word and store it
    do{
      //skip until the word starts
      c = SkipWhitespace(fp);
      if(c != '\n' && c != EOF){  //if you found a word (doc not empty)
        char_index++;
        int word_start = char_index;
        //append the word to the document (increase doc_size if needed)
        AddWord(fp,&(DMAP.map[doc_index]),&doc_size,&char_index,&c);
        DMAP.doc_lenght[doc_index]++;
        WORDS_IN_FILE++;
        //initialize the word you just got
        Word* word = CreateWord(doc_index,          //word->doc_id
                                char_index-word_start,  //word->size
                                word_start);        //word->start
        //insert the word to the Trie
        int ignore;
        TrieRoot = TrieInsert(TrieRoot, word, 1, &ignore);
        //add a space after the word in the document
        //(the last space is overridden by \0 anyway)
        DMAP.map[doc_index][char_index] = ' ';
        free(word);
      }
    }while(c != '\n' && c != EOF);

    /****************document added***cleanup*********************************/
    //most likely there will be more space allocated than needed for a document
    if(char_index == -1){
      //empty documents need no space and are marked as NULL in the map
      free(DMAP.map[doc_index]);
      DMAP.map[doc_index] = NULL;
      UpdateMEAN_DOC_SIZE(0);
    }
    else{
      DMAP.map[doc_index][char_index] = '\0'; //terminate the document
      if(char_index < doc_size){  //check if the size is precise, if not resize
        DMAP.map[doc_index] = ResizeDocument(DMAP.map[doc_index],char_index);
        NULL_Check(DMAP.map[doc_index]);
        UpdateMEAN_DOC_SIZE(char_index);
      }
    }
  }while(c != EOF);

  return OK;
}

/*******************************************************************************
**************************Utility FUnctions for MapAndTrie**********************
********************************************************************************/
void UpdateMEAN_DOC_SIZE(int new_doc_size){
  FILE_SIZE += new_doc_size;
  MEAN_DOC_SIZE = ceil((double)FILE_SIZE/DMAP.size);
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
    if(*char_index > *doc_size-2){
      *doc_ptr = ReallocDocument(doc,doc_size);
      NULL_Check(*doc_ptr);
      doc = *doc_ptr;
      NULL_Check(doc_ptr);
    }
    doc[*char_index] = *c;
    (*char_index)++;
    *c = fgetc(fp);
  }
}

void PrintMAP(){
  printf("*****************Printing the DMAP*********************\n");
  for(int i=0; i<DMAP.size; i++){
    if(DMAP.map[i] == NULL)
      printf("(size:0)%d", i);
    else
      printf("(size:%d)%d %s", (int)strlen(DMAP.map[i]),i,DMAP.map[i]);
    printf("\n");
  }
}

/*****************DocumentMAP members*****************************************/
/*Make a new slot in the map for a new document. Allocate space for it.*/
void NewDocument(int doc_size){
  int doc_index = DMAP.size;
  //resize the map
  DMAP.size++;
  DMAP.map = realloc(DMAP.map,sizeof(char*)*DMAP.size);
  NULL_Check(DMAP.map);
  //resize the doc_lenght array as well
  DMAP.doc_lenght = realloc(DMAP.doc_lenght,sizeof(int)*DMAP.size);
  DMAP.doc_lenght[doc_index] = 0;
  //allocate space for this document in the map
  DMAP.map[doc_index] = malloc(sizeof(char)*(doc_size +1)); //+1 for the \0
  NULL_Check(DMAP.map[doc_index]);
}

void FreeMap(){
  //free all the documents
  for(int i=0; i<DMAP.size; i++){
    if(DMAP.map[i] != NULL)
      free(DMAP.map[i]);
    DMAP.map[i] = NULL;
  }
  //free the array for keeping track of document lenghts
  if(DMAP.doc_lenght != NULL){
    free(DMAP.doc_lenght);
    DMAP.doc_lenght = NULL;
  }
  //Free the map array where we kept the documents
  if(DMAP.map != NULL){
    free(DMAP.map);
    DMAP.map = NULL;
  }
}

/******************************************************************************
************************DIFFERENT APPROACH*************************************
*******************ScanFile first, then choose weather to continue*************
****************************UNUSED*********************************************

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
