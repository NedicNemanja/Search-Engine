#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Arguments.h"
#include "ErrorCodes.h"

int ReadArguments(int argc, char* argv[]){
  int arg_index=1;
  int doc_flag=0, k_flag=0;

  //scan all arguements to find (-i,-k)
  while(arg_index < argc){
    //make sure there is an argument to check
    if(argv[arg_index] == NULL)
      break;

    if(!strcmp(argv[arg_index],"-i")){ //next arv_index is docfilename
      docfilename = argv[++arg_index];
      doc_flag = 1;
    }
    else if(!strcmp(argv[arg_index],"-k")){  //next arg_index is k
      K = atoi(argv[++arg_index]);
      k_flag = 1;
    }
    else{ //this argument should not be here
      fprintf(stdout, "Unknown argument: %s\n", argv[arg_index]);
      return UNKNOWN_CMDARGUMENT;
    }
    //next
    arg_index++;
  }

  //check that you got all the arguements
  if(doc_flag == 0){
    fprintf(stdout, "Specify a file to read documents from.\n"
                    "Example: ./minisearch -i docfile.txt\n");
    return UNSPECIFIED_i;
  }
  if(k_flag == 0){  //default: k=10
    K = 10;
    k_flag = 1;
  }

  return OK;
}
