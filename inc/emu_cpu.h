#ifndef HEADER_EMU_CPU
#define HEADER_EMU_CPU

#include <stdint.h>
#include <stdbool.h>

#define EMUCPU_RAM_SIZE           (0x10000u)

#define EMUCPU_NUM_FLAGS          (4u)
#define EMUCPU_ZERO_FLAG          (0u)
#define EMUCPU_CARRY_FLAG         (1u)
#define EMUCPU_HALF_CARRY_FLAG    (2u)
#define EMUCPU_SUBTRACT_FLAG      (3u)

union EMUCPU_Register
{
  uint16_t regValue;
  struct
  {
    uint8_t low;
    uint8_t high;
  };
};

typedef struct
{
  uint8_t ram[EMUCPU_RAM_SIZE];
  uint64_t cycles;
  uint16_t sp;
  uint16_t pc;
  union EMUCPU_Register bc;
  union EMUCPU_Register de;
  union EMUCPU_Register hl;
  uint8_t  a;
  uint8_t  flags[EMUCPU_NUM_FLAGS];
  bool     stateOk;
} EMUCPU_Context;

void EMUCPU_init(uint8_t* prog, uint16_t size);
void EMUCPU_run();
char* EMUCPU_getOpName(uint8_t op);

#endif
