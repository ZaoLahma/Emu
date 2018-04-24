#ifndef HEADER_EMU_ROM
#define HEADER_EMU_ROM

#include <stdint.h>

typedef enum
{
  EMUROM_OK = 0,
  EMUROM_NOK
} EMUROM_Return;

EMUROM_Return EMUROM_read(char* romPath, uint8_t* romBuf, uint16_t* romSize);

#endif
