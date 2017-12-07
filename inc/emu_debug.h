#ifndef HEADER_EMU_DEBUG
#define HEADER_EMU_DEBUG

#include "emu.h"
#include <stdio.h>

#define EMU_DEBUG_ASSERT_COND(cond) \
{ \
  if(!(cond)) \
  { \
    (void) printf("%s:%u - ASSERT %s FAILED\n", __FILE__, __LINE__, #cond); \
    EMU_abort(); \
  } \
} \

#define EMU_DEBUG_ASSERT_INT_EQ(this, that) \
{ \
  if(this != that) \
  { \
    (void) printf("%s:%u - ASSERT %s (%d) == %s (%d) FAILED\n", __FILE__, __LINE__, #this, this, #that, that); \
    EMU_abort(); \
  } \
} \

#endif