#ifndef CONSOLE_H
#define CONSOLE_H

#include "Score.h"

/*Starts the console, where you can use the following commands:
/search q1 q2 ... qMAX_QUERRY_SIZE
/df
/df word
/tf id word
/exit
*/
void StartConsole();

/*********Utility functions****************************************************/

/*Read a word from stdin dynamically and return the char at the end of the wrd*/
char getWord(char** wordptr);

/*Read till the end of the current stdin stream.
Usefull when we want to ignore more than K querries.*/
void ReadTillEnd();

//print top n results by score of a querry
void PrintTopScores(Score** ScoreArray ,int ScoreArray_size, int n);

//read a querry and return the char that terminated it
char ReadQuerry(char** querry,int* qsize, char c);

void FreeQuerry(char** querry,int qsize);

void FreeScoreArray(Score*** ScoreArrayptr, int ScoreArray_size);

#endif
