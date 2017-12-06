#ifndef HEADER_TEST_EMU_FWK
#define HEADER_TEST_EMU_FWK

#include <stdio.h>

typedef enum
{
  TEST_OK = 0,
  TEST_NOK
} EMUTEST_Result;

EMUTEST_Result EMUTEST_getResult(void);
void EMUTEST_fail(void);

#define TEST_ASSERT_INT_EQ(this, that) \
{ \
  if(this != that) \
  { \
    (void) printf("ASSERT FAILED: %s (%d) != %s (%d)\n", #this, this, #that, that); \
    EMUTEST_fail(); \
  } \
} \

#endif
