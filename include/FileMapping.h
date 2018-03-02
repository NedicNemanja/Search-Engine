
//all the documents are kept here, indexing is the same as in the input file
typedef struct DocumentMAP{
  int size;     //number of documents in the map array
  char** map;  //flexible array for storing documents
} DocumentMAP;

DocumentMAP DMAP;

int MapFile(char* docfilename);
