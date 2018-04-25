#include "emu.h"
#include "emu_cpu.h"
#include "emu_rom.h"
#include "emu_debug.h"
#include "emu_window.h"
#include "emu_debugger.h"
#include <stdint.h>
#include <stdbool.h>

#define MAX_ROM_BYTE_SIZE (1000000u)

typedef struct
{
  uint8_t  romBuf[MAX_ROM_BYTE_SIZE];
  uint16_t romSize;
  bool     runTimeStatus;
} EMU_Context;

static EMU_Context emuCtxt;

void EMU_init(char* romPath, bool debuggerActive)
{
  emuCtxt.runTimeStatus = true;
  EMU_DEBUG_ASSERT_COND(EMUROM_OK == EMUROM_read(romPath, emuCtxt.romBuf, &emuCtxt.romSize));
  EMUCPU_init(emuCtxt.romBuf, emuCtxt.romSize);
  EMUDEBUGGER_init(debuggerActive);
  EMUWIN_init();
}

void EMU_run(void)
{
  while(emuCtxt.runTimeStatus)
  {
    EMUWIN_run();
    EMUDEBUGGER_run();
    EMUCPU_run();
  }
}

void EMU_abort(void)
{
  emuCtxt.runTimeStatus = false;
}
