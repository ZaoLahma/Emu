#include "emu_debug.h"
#include "emu.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MIN_EXPECTED_NUM_ARGUMENTS (2)
#define DEBUGGER_ACTIVATED_FLAG    "dbg"

int main(int argc, char** argv)
{
  EMU_DEBUG_ASSERT_COND(argc >= MIN_EXPECTED_NUM_ARGUMENTS);

  if(argc < MIN_EXPECTED_NUM_ARGUMENTS)
  {
    (void) printf("Launch failed. Usage: \"gameboy path_to_rom\" with an optional third flag dbg\n");
  }
  else
  {
    char* romPath = argv[1u];
    bool debuggerActive = false;
    if(argc == 3u)
    {
      if(0u == memcmp(argv[2u], DEBUGGER_ACTIVATED_FLAG, strlen(DEBUGGER_ACTIVATED_FLAG)))
      {
        debuggerActive = true;
      }
    }
    EMU_init(romPath, debuggerActive);
    EMU_run();
  }
}
