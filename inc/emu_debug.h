#ifndef HEADER_EMU_DEBUG
#define HEADER_EMU_DEBUG

#include "emu.h"
#include <stdio.h>

#define EMU_DEBUG_ASSERT(cond) \
{ \
  if(!(cond)) \
  { \
    (void) printf("%s:%u - ASSERT %s FAILED\n", __FILE__, __LINE__, #cond); \
    EMU_abort(); \
  } \
} \

#endif
