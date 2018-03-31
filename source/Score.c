#include <stdlib.h>
#include <math.h>
#include "Score.h"
#include "FileMapping.h"
#include "ErrorCodes.h"
#include "Post.h"

/*for every document(post) of this word's posting List
caclulate the score and add it to the score array.*/
void PostingListScore(PostingList pl, Score*** ScoreArrayptr,int* ScoreArray_size){
  for(int i=0; i<pl.doc_frequency; i++){
    Post post = getPost(pl,i);
    Score* score = CreateScore(pl,post);
    AddDocumentScore(ScoreArrayptr, ScoreArray_size, score);
  }
}

/*Calculate the score of a document(identifyed by post)
for specific word(identifyed by pl).
Note(highlights): also remember where you found this word in the document.*/
Score* CreateScore(PostingList pl, Post post){
  Score* score = malloc(sizeof(Score));
  NULL_Check(score);
  score->doc_id = post.doc_id;
  //calculate score value
  double IDF  = log10(  (double)(DMAP.size - pl.doc_frequency + 0.5)
                                /(pl.doc_frequency + 0.5)   );

  double avg = DMAP.doc_lenght[post.doc_id] / ( (double)WORDS_IN_FILE/DMAP.size);

  score->value = IDF*(  ( post.recurrence * (k1+1) )
                       /( post.recurrence + k1*(1-b+b*avg))  );
  //set hightlights for this word in the document
  score->numofHighlights = post.recurrence;
  score->Highlights = malloc(sizeof(Highlight)*score->numofHighlights);
  //for each time this word reccures, keep a highlight
  for(int i=0; i<score->numofHighlights; i++){
    score->Highlights[i].start = post.word_posArray[i];
    score->Highlights[i].size = pl.word->size;
  }
  return score;
}

void FreeScore(Score* score){
  if(score == NULL)
    return;
  free(score->Highlights);
  score->Highlights = NULL;

  free(score);
}

/*Add a score of a document for some word the the ScoreArray.
If that document does not have previous scores create a score to track it.*/
void AddDocumentScore(Score*** ScoreArrayptr, int* Array_size, Score* new_score){
  Score** ScoreArray = *ScoreArrayptr;
  //search the array for doc_id
  int found_flag = 0;
  int pos = FindPos(ScoreArray,0,*Array_size-1,new_score->doc_id,&found_flag);

  //if you find the document already has a score, just add to it
  if(found_flag){
    //add the value
    ScoreArray[pos]->value += new_score->value;
    //append the highlights
    ScoreArray[pos]->Highlights = AddHighlights(ScoreArray[pos],new_score);
    FreeScore(new_score);
  }
  //if you didnt find this doc_id then add a new score to the array
  else{
    //make the array bigger
    (*Array_size)++;
    *ScoreArrayptr = realloc(*ScoreArrayptr, sizeof(Score*)*(*Array_size) );
    ScoreArray = *ScoreArrayptr;
    NULL_Check(ScoreArray);
    //move all the elements after pos to create space for new_score
    for(int i=*Array_size-2; i>=pos; i--){
      ScoreArray[i+1] = ScoreArray[i];
    }
    //insert the new_score
    ScoreArray[pos] = new_score;
  }
}

//Append the highlight arrays and return the product
Highlight* AddHighlights(Score* old_score, Score* new_score){
  int old_index = old_score->numofHighlights;
  old_score->numofHighlights += new_score->numofHighlights;
  Highlight* new_Highlights = realloc(old_score->Highlights,
                              sizeof(Highlight)*old_score->numofHighlights);
  //add every highlight of the new score to the old_score
  for(int i=0; i<(new_score->numofHighlights); i++){
    new_Highlights[old_index+i] = new_score->Highlights[i];
  }
  return new_Highlights;
}

/*Recursively find where doc_id is or where it should be in the array
using a binary search approach. Every key is uniqe in ScoreArray.*/
int FindPos(Score** ScoreArray, int start, int end, int key,int* found_flag){
  if(ScoreArray == NULL)
    return 0; //if the array is empty

  if(start >= end){ //base case array_size<1
    /*The key is not present, but we found where it should be placed
     if the pos is occupied by a larger key,
     we'll move the larger one right and insert our key in its place,
     otherwise we'll insert it after it.*/
    if(start>end)
      start--;
    if(key < ScoreArray[start]->doc_id)
      return start;
    else if(key > ScoreArray[start]->doc_id)
      return start+1;
    /*The key is present*/
    else{ //the key is at start
      *found_flag = 1;
      return start;
    }
  }

  int middle = start+ceil((double)(end-start)/2);
  if(key < ScoreArray[middle]->doc_id)
    return FindPos(ScoreArray,start,middle-1,key,found_flag);
  else if(key > ScoreArray[middle]->doc_id)
    return FindPos(ScoreArray,middle+1,end,key,found_flag);
  else{
    *found_flag = 1;
    return middle;
  }
}
