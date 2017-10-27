#ifndef _TEST_H
#define _TEST_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define PRINT_LINE_N printf("test failed at line: %d\n", __LINE__);
#define TEST_CHECK(x) (if(x){} else {PRINT_LINE_N;return 1;})
#define TEST_EQUAL(x, y) ({if(x == y){} else {PRINT_LINE_N;printf("TEST_EQUAL ( %ld == %ld )\n", (long)x, (long)y);return 1;}})
#define TEST_EQUAL_FAIL(x, y) ({if(x == y) {PRINT_LINE_N;printf("TEST_EQUAL_FAIL ( %ld == %ld )\n", (long)x, (long)y);return 1;}})
#define TEST_NEQUAL(x, y) ({if(x != y){} else {PRINT_LINE_N;printf("TEST_NEQUAL ( %ld != %ld )\n", (long)x, (long)y);return 1;}})
#define TEST_NEQUAL_FAIL(x, y) ({if(x != y) {PRINT_LINE_N;printf("TEST_NEQUAL_FAIL ( %ld != %ld )\n", (long)x, (long)y);return 1;}})
#define TEST_STR_LIKE(x, y) ({if(strcmp(x, y)){PRINT_LINE_N;return 1;}})
#define TEST_STR_LIKE_FAIL(x, y) ({if(strcmp(x, y)){} else {PRINT_LINE_N;return 1;}})
#define TEST_STR_NLIKE(x, y) ({if(!strcmp(x, y)){PRINT_LINE_N;return 1;}})

#define TEST_END return 0;
#define TEST_FAIL return 1;
#define FAIL(x)





int fill_buffer_from_file(char *filename, char **lbuffer)
{
  FILE * pFile;
  long lSize;
  size_t result;
  pFile = fopen ( filename , "rb" );
  if (pFile==NULL) {fputs ("File error\n",stderr); return (1);}

  // obtain file size:
  fseek (pFile , 0 , SEEK_END);
  lSize = ftell (pFile);
  rewind (pFile);

  // allocate memory to contain the whole file:
  char *buffer = (char*) malloc (sizeof(char)*lSize+1);
  if (buffer == NULL) {fputs ("Memory error\n",stderr); return (2);}

  // copy the file into the buffer:
  result = fread (buffer,1,lSize,pFile);
  if (result != lSize) {fputs ("Reading error\n",stderr); return (3);}

  buffer[sizeof(char)*lSize] = 0;

  /* the whole file is now loaded in the memory buffer. */
  // terminate
  fclose (pFile);

  *lbuffer = buffer;

  return 0;

}

#endif


