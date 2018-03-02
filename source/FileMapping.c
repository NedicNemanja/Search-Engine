#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FileMapping.h"
#include "ErrorCodes.h"

/*ignore and count every char in fstream until \n or EOF*/
char GoToEndOfline(FILE* fp, int* DSize){
  char c = fgetc(fp);	//ignore the space between index and 1sr word
  while(c != '\n' && c!=EOF){
    c=fgetc(fp);
    if(c!='\n' && c!=EOF)
      (*DSize)++;
  }
  return c;
}


/*Check if the file is sorted and learn the size of each document.*/
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

void MapAndTrie(int* DSizes, FILE* fp){
  int temp;
  //allocate a pointer in the map for each document
  DMAP.map = malloc(sizeof(char*)*DMAP.size);
  NULL_Check(DMAP.map);

  for(int i=0; i<DMAP.size; i++){ //for every document
    //allocate memory in the map for the document
    DMAP.map[i] = malloc(sizeof(char)*(DSizes[i]+1)); //+1 for terminating char
    NULL_Check(DMAP.map);
    //ignore the document index and the space after it
    fscanf(fp, "%d", &temp);
    fgetc(fp);
    //get and store the document to the map
    fgets(DMAP.map[i],DSizes[i]+1,fp);/*note: fgets() reads until specified size
                                      DSizes[i] or \n. As DSizes[i] describes
                                      the original string with the index,
                                      removing the index produces a smaller
                                      string.
                                      So it is certain that fgets() will stop
                                      at the \n. But it wont be saved to the map
                                      because we only allocated just enough for
                                      the string without the \n.*/
    //break the document into words
    char* word;
    int offset=0;
    do{ //for every word in the document
      word = strtok(DMAP.map[i]+offset, " ");
      if(word != NULL){
        //TrieInsert(word);
        printf("word:%s\n", word);
        offset += strlen(word)+1;
      }
    }while(word != NULL);
    printf("----------------------new document------------\n");
  }
}

/*Reads the file once and allocates the needed memory for it, if its sorted.
Then read the file again and store the documents to DMAP.*/
int MapFile(char* docfilename){
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
