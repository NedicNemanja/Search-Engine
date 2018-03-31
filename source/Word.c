#include <stdio.h>
#include <stdlib.h>
#include "Word.h"
#include "ErrorCodes.h"
#include "FileMapping.h"

Word* CreateWord(int d_id, int word_size, int word_start){
  Word* new_word = malloc(sizeof(Word));
  NULL_Check(new_word);
  //INITALIZE
  new_word->doc_id = d_id;
  new_word->size = word_size;
  new_word->start = word_start;

  return new_word;
}

Word* CopyWord(Word source){
  return CreateWord(source.doc_id,source.size,source.start);
}

//return pointer to a point where this word starts in some document
char* getStart(Word word){
  return DMAP.map[word.doc_id]+word.start;
}

char getLetter(Word word, int letter_pos){
  char* start = getStart(word);
  return start[letter_pos];
}

void PrintWord(Word word){
  for(int i=0; i<(word.size); i++){
    printf("%c", getLetter(word,i));
  }
}
