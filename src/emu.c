#include "emu.h"
#include "emu_cpu.h"
#include "emu_rom.h"
#include "emu_debug.h"
#include <stdint.h>
#include <stdbool.h>

#define MAX_ROM_BYTE_SIZE (1000000u)

typedef struct
{
  char     romBuf[MAX_ROM_BYTE_SIZE];
  uint32_t romSize;
  bool     runTimeStatus;
} EMU_Context;

static EMU_Context emuCtxt;

void EMU_init(char* romPath)
{
  emuCtxt.runTimeStatus = true;
  EMUCPU_init();
  EMU_DEBUG_ASSERT(EMUROM_OK == EMUROM_read(romPath, emuCtxt.romBuf, &emuCtxt.romSize));
}

void EMU_run(void)
{
  if(emuCtxt.runTimeStatus)
  {
    EMUCPU_run(emuCtxt.romBuf);
  }
}

void EMU_abort(void)
{
  emuCtxt.runTimeStatus = false;
  EMUCPU_abort();
}
