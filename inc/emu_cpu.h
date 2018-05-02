#ifndef HEADER_EMU_CPU
#define HEADER_EMU_CPU

#include <stdint.h>
#include <stdbool.h>

void EMUCPU_init(uint8_t* prog, uint16_t size);
void EMUCPU_run();
char* EMUCPU_getOpName(uint8_t op);

#endif
