#include <stdio.h>
#include "Arguments.h"
#include "ErrorCodes.h"
#include "FileMapping.h"
#include "Console.h"
#include "Trie.h"
#include "List.h"

int main(int argc, char* argv[]){
  //read command line arguments
  if(ReadArguments(argc,argv) != OK)
    return BAD_ARGUMENTS;

  //open file for reading
  FILE* fp;
  fp = fopen(docfilename, "r");
  if(!fp){
    fprintf(stderr,"Can't open file %s", docfilename);
    return CANT_OPEN_FILE;
  }

  //Read the file, Map it, and add all the word to the Trie
  if(MapAndTrie(fp) != OK){
    fclose(fp);
    return UNORDERED_DOCUMENTS;
  }

  StartConsole();

  //clean up
  FreeTrie(TrieRoot);
  TrieRoot = NULL;

  FreeList(PLIST.next);
  PLIST.next = NULL;

  FreeMap();
  fclose(fp);
  return 0;
}
