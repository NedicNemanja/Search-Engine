#include "QuickSort.h"

/*That the last element as pivot and split the array in 2 parts.
One that has all the emelents smaller than pivot
and on that has all the larger.*/
int PartitionScoreArray(Score** ScoreArray, int low, int high){
  double pivot = ScoreArray[high]->value;
  int i = low-1;

  for(int j=low; j<=high-1; j++){ //from low until pivot
    if(ScoreArray[j]->value > pivot){  //if key<=pivot
      i++;
      //swap
      Score* temp = ScoreArray[i];
      ScoreArray[i] = ScoreArray[j];
      ScoreArray[j] = temp;
    }
  }
  //and last swap
  Score* temp = ScoreArray[i+1];
  ScoreArray[i+1] = ScoreArray[high];
  ScoreArray[high] = temp;
  return i+1;
}

void QuickSortScores(Score** ScoreArray,int low,int high){
  if(low<high){
    int partition_index = PartitionScoreArray(ScoreArray,low,high);

    QuickSortScores(ScoreArray, low, partition_index-1);
    QuickSortScores(ScoreArray, partition_index+1, high);
  }
}

/************Highlights*******************************************************/

int PartitionHighlights(Highlight* Highlights, int low, int high){
  double pivot = Highlights[high].start;
  int i = low-1;

  for(int j=low; j<=high-1; j++){ //from low until pivot
    if(Highlights[j].start <= pivot){  //if key<=pivot
      i++;
      //swap
      Highlight temp = Highlights[i];
      Highlights[i] = Highlights[j];
      Highlights[j] = temp;
    }
  }
  //and the last swap
  Highlight temp = Highlights[i+1];
  Highlights[i+1] = Highlights[high];
  Highlights[high] = temp;
  return i+1;
}

void QuickSortHighlights(Highlight* Highlights,int low, int high){
  if(low<high){
    int partition_index = PartitionHighlights(Highlights,low,high);

    QuickSortHighlights(Highlights, low, partition_index-1);
    QuickSortHighlights(Highlights, partition_index+1, high);
  }
}
