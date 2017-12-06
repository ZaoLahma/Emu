#ifndef HEADER_TEST_EMU_CPU
#define HEADER_TEST_EMU_CPU

/*
  Function to be implemented by production
  code to expose CPU context
*/
struct EMUCPU_Context;
void EMUCPU_getContext(struct EMUCPU_Context** context);

void EMUCPU_test(void);

#endif
