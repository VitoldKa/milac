#ifndef _TEST_H
#define _TEST_H

#include <string.h>

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

#endif
