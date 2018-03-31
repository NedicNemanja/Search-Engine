#ifndef WORD_H
#define WORD_H

typedef struct Word{
  int doc_id;
  int size;
  int start; //where the word starts in the document
}Word;

Word* CreateWord(int doc_id,int size,int start);
Word* CopyWord(Word source);

char getLetter(Word word, int letter_pos);
void PrintWord(Word word);

#endif
