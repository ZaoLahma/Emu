#ifndef HEADER_EMU_CPU
#define HEADER_EMU_CPU

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
  uint64_t cycles;
  uint16_t sp;
  uint16_t pc;
  uint8_t  a;
  uint8_t  b;
  uint8_t  c;
  uint8_t  d;
  uint8_t  e;
  uint8_t  h;
  uint8_t  l;
  bool     stateOk;
} EMUCPU_Context;

void EMUCPU_init(void);
void EMUCPU_run(uint8_t* prog);

#endif
