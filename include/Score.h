#ifndef SCORE_H
#define SCORE_H
#include "PostingList.h"
#include "Highlight.h"

#define k1 1.2
#define b 0.75

//Score of a document for a specific querry
typedef struct Score{
  int doc_id;
  double value;
  /*Keep track of where the querry matched this document
    example: {[3,5],[6,9]} means that in this doc from range 3 to 8 and 6 to 15
    there are words that matched the querry.
    note: keep in mind that if a query has the same word more than once,
          then there will be more than one highlight for one word.*/
  Highlight* Highlights;
  int numofHighlights;
}Score;

/*Calculate the score of a document(identifyed by post)
for specific word(identifyed by pl).
Note(highlights): also remember where you found this word in the document.*/
Score* CreateScore(PostingList pl, Post post);

void FreeScore(Score* score);

/*Add the score of a document for some word the the ScoreArray.
If that document has previous scores just add the value of this one to the old
one and then delete it.*/
void AddDocumentScore(Score*** ScoreArrayptr, int* Array_size, Score* new_score);

/*Append the highlight arrays and return the product
Usefull when we are merging 2 scores that refer to the same document.*/
Highlight* AddHighlights(Score* old_score, Score* new_score);


/*****************non members**************************************************/
/*Calculate and track the score for each post of this list to the score array.*/
void PostingListScore(PostingList p,Score*** ScoreArrayptr,int* ScoreArray_size);

/****************Utility Functions ********************************************/
/*Recursively find where doc_id is or where it should be in the array
using a binary search approach.*/
int FindPos(Score** ScoreArray, int start, int end, int key,int* found_flag);

#endif
