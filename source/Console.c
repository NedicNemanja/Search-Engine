#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Console.h"
#include "StringManipulation.h"
#include "Trie.h"
#include "PostingList.h"
#include "FileMapping.h"
#include "Arguments.h"
#include "QuickSort.h"

#define MAX_QUERRY_SIZE 10

void StartConsole(){
  printf("Console open:\n");
  char* command = NULL;
  do{
    char c = getWord(&command);

    /**************************************************************************/
    if(!strcmp(command,"/search")){
      //Read the querry word by word
      char* querry[MAX_QUERRY_SIZE];
      int qsize = 0;
      c = ReadQuerry(querry,&qsize,c);
      if(c != '\n') //ignore everything after the K-th word of a querry
        ReadTillEnd();

      /*Search the querry word by word******************************************
        calculate the score of all the documents this word appears it***********
        add the score to the array.********************************************/
      Score** ScoreArray = NULL; //here we keep the scores ptrs sorted by doc_id
      int ScoreArray_size = 0;
      for(int i=0; i<qsize; i++){
        //get this words posting list
        PostingList* pl = SearchTrie(querry[i],TrieRoot,1);
        /*calculate the scores of each document that appears in the pl
         and add them to the ScoreArray*/
        if(pl != NULL)
          PostingListScore(*pl,&ScoreArray,&ScoreArray_size);
        else
          printf("[%s] not found.\n", querry[i]);
      }

      QuickSortScores(ScoreArray,0,ScoreArray_size-1); //sort the array by score
      PrintTopScores(ScoreArray,ScoreArray_size,K);

      FreeQuerry(querry,qsize);
      FreeScoreArray(&ScoreArray,ScoreArray_size);
    }

    /**************************************************************************/
    else if(!strcmp(command,"/df")){
      if( c == '\n'){                 /*just /df */
        PrintAllPostingLists(&PLIST);
      }
      else{                           /* /df word */
        char* word = NULL;
        c = getWord(&word);
        PostingList* pl = SearchTrie(word,TrieRoot,1);
        if(pl != NULL)
          PrintPostingList(pl);
        else
          printf("[%s] not found.\n", word);
        //ignore everything after the word
        if(c != '\n')
          ReadTillEnd();
        if(word != NULL)
          free(word);
      }
    }

    /**************************************************************************/
    else if(!strcmp(command,"/tf")){  /* /tf doc_id word */
      if(c != '\n'){
        //read the doc_id
        int doc_id = -1;
        scanf("%d", &doc_id);
        //read the word
        char* word = NULL;
        c = getWord(&word);
        //ignore everything after the word
        if(c != '\n')
          ReadTillEnd();
        //if the format was ok
        if(doc_id==-1 || word==NULL)
          printf("Plase use the specifiec format: /tf id word\n");
        else{
          //find the word
          PostingList* pl = SearchTrie(word,TrieRoot,1);
          if(pl != NULL)
            PrintRecurrence(*pl,doc_id);
          else
            printf("[%s] not found.\n", word);
        }
        if(word != NULL)
          free(word);
      }
    }
    /**************************************************************************/
    else{
      if(strcmp(command,"/exit"))
        printf("Unknown command: %s\n", command);
    }

  }while(strcmp(command,"/exit"));
  free(command);
}

/**************Utility Functions***********************************************/

/*Read a word from stdin dynamically and return the char at the end of the wrd*/
char getWord(char** wordptr){
  char c = SkipWhitespace(stdin);
  int size=0;
  while(c != '\n' && c != ' ' && c != '\t'){
    size++;
    *wordptr = realloc(*wordptr,sizeof(char)*(size+1));
    (*wordptr)[size-1] = c;
    c = getc(stdin);
  }
  (*wordptr)[size] = '\0';
  return c;
}

/*Read till the end of the current stdin stream.
Usefull when we want to ignore more than K querries.*/
void ReadTillEnd(){
  char c;
  do{
    c = getc(stdin);
  }while(c != '\n');
}

//print top n results by score of a querry
void PrintTopScores(Score** ScoreArray ,int ScoreArray_size, int n){
  for(int i=0; i<n; i++){
    if(i>=ScoreArray_size)
      break;
    printf("%3d.", i);
    printf("(%5d)", ScoreArray[i]->doc_id);
    printf("[%+5f] ", ScoreArray[i]->value);
    PrintDocumentToTerminal(DMAP.map[ScoreArray[i]->doc_id],
                            ScoreArray[i]->Highlights,
                            ScoreArray[i]->numofHighlights);
    printf("\n");
  }
}

//read a querry and return the char that terminated it
char ReadQuerry(char** querry,int* qsize, char c){
  while(c != '\n' && (*qsize)<MAX_QUERRY_SIZE){
    char* word = NULL;
    c = getWord(&word);
    (*qsize)++;
    querry[(*qsize)-1] = word;
  }
  return c;
}

void FreeQuerry(char** querry,int qsize){
  for(int i=0; i<qsize; i++){
    free(querry[i]);
  }
}

void FreeScoreArray(Score*** ScoreArrayptr, int ScoreArray_size){
  Score** ScoreArray = *ScoreArrayptr;
  for(int i=0; i<ScoreArray_size; i++){
    FreeScore(ScoreArray[i]);
    ScoreArray[i] = NULL;
  }
  free(ScoreArray);
  *ScoreArrayptr = NULL;
}
