#ifndef QUICKSORT_H
#define QUICKSORT_H

#include "Score.h"
#include "Highlight.h"

void QuickSortScores(Score** ScoreArray, int low, int high);

void QuickSortHighlights(Highlight* Highlights,int low, int high);
#endif
