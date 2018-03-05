#ifndef ERRORCODES_H
#define ERRORCODES_H

#include <stdlib.h>

typedef enum ERRORCODE { OK=0,
                  BAD_ARGUMENTS=1,
                  UNORDERED_DOCUMENTS,
                  CANT_OPEN_FILE,
                  ALLOCATION_FAIL,
                  UNKNOWN_CMDARGUMENT,
                  UNSPECIFIED_i,
                  UNINDEXED_LINE
} ERRORCODE;

#define NULL_Check(ptr) if(ptr == NULL) {fprintf(stderr, "Malloc/Realloc failed %s:%d\n", __FILE__, __LINE__); exit(ALLOCATION_FAIL);}

#define CALL_OR_DIE(err_code) if(err_code != OK) {fprintf(stderr, "Exiting on ErrorCode: %d", err_code) exit(err_code)};

#endif
