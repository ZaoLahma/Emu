#ifndef HEADER_TEST_EMU_FWK
#define HEADER_TEST_EMU_FWK

#include <stdio.h>

typedef enum
{
  TEST_OK = 0,
  TEST_NOK
} EMUTEST_Result;

EMUTEST_Result EMUTEST_getResult(void);
EMUTEST_Result EMUTEST_getOverallResult(void);
void EMUTEST_fail(void);

#define TEST_SUITE(testSuiteFunc) \
{ \
  (void) printf("\nRUNNING SUITE %s\n", #testSuiteFunc); \
  testSuiteFunc(); \
  (void) printf("\n-------------- \n"); \
} \

#define TEST_CASE(testFunc) \
{ \
  (void) printf("\nRUNNING TEST CASE %s\n", #testFunc); \
  testFunc(); \
  if(TEST_OK != EMUTEST_getResult()) \
  { \
    (void) printf("TEST CASE %s FAILED\n", #testFunc); \
  } \
  else \
  { \
    (void) printf("TEST %s SUCCEEDED \n", #testFunc); \
  } \
} \

#define TEST_ASSERT_INT_EQ(this, that) \
{ \
  if(this != that) \
  { \
    (void) printf("%s:%u - ASSERT FAILED: %s (%d) != %s (%d)\n", __FILE__, __LINE__, #this, this, #that, that); \
    EMUTEST_fail(); \
  } \
} \

#define TEST_ASSERT_UINT_EQ(this, that) \
{ \
  if(this != that) \
  { \
    (void) printf("%s:%u - ASSERT FAILED: %s (%u) != %s (%u)\n", __FILE__, __LINE__, #this, this, #that, that); \
    EMUTEST_fail(); \
  } \
} \

#endif
