#ifndef HEADER_EMU_CPU
#define HEADER_EMU_CPU

#include <stdint.h>

typedef struct
{
  uint64_t cycles;
  uint16_t pc;
  uint8_t  A;
  uint8_t  X;
  uint8_t  Y;
  uint8_t  sp;
} EMUCPU_Context;

void EMUCPU_init(void);
void EMUCPU_run(char* prog);
void EMUCPU_abort(void);

#endif
