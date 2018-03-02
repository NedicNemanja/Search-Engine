#include <stdio.h>
#include "Arguments.h"
#include "ErrorCodes.h"
#include "FileMapping.h"

int main(int argc, char* argv[]){
  if(ReadArguments(argc,argv) != OK)
    return BAD_ARGUMENTS;

  if(MapFile(docfilename) != OK){
    return UNORDERED_DOCUMENTS;
  }
printf("can map file\n");
  return 0;
}
