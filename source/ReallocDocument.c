#include <stdio.h>
#include <stdlib.h>
#include "ReallocDocument.h"

/*Make the document size GOLDEN_RATIO times bigger.
+1 is for \0.*/
void* ReallocDocument(char* doc, int* doc_size){
  fprintf(stderr, "Reallocating Document %d->", *doc_size);
  (*doc_size) *= GOLDEN_RATIO;
  fprintf(stderr, "%d\n", *doc_size);
  return realloc(doc, sizeof(char)*( (*doc_size) +1) );
}
