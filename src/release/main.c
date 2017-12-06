#include "emu_debug.h"
#include "emu.h"
#include <stdio.h>

#define EXPECTED_NUM_ARGUMENTS (2)

int main(int argc, char** argv)
{
  EMU_DEBUG_ASSERT_INT_EQ(EXPECTED_NUM_ARGUMENTS, argc);

  char* romPath = argv[1];
  EMU_init(romPath);
  EMU_run();

  if(argc != EXPECTED_NUM_ARGUMENTS)
  {
    (void) printf("Launch failed. Usage: \"nes path_to_rom\"\n");
  }
}
