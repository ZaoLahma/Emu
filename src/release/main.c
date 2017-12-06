#include "emu_debug.h"
#include "emu.h"
#include <stdio.h>

#define EXPECTED_NUM_ARGUMENTS (2)

int main(int argc, char** argv)
{
  EMU_DEBUG_ASSERT_INT_EQ(EXPECTED_NUM_ARGUMENTS, argc);

  if(argc == EXPECTED_NUM_ARGUMENTS)
  {
    char* romPath = argv[1];
    EMU_init(romPath);
    EMU_run();
  }
  else
  {
    (void) printf("Usage: nes path_to_rom\n");
  }
}
