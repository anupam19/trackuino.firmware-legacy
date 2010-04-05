#include "WProgram.h"

int main(void)
{
  // This main function is compatible with the arduino IDE,
  // in case we ever want to turn this into a gigantic .pde.
  init();
  
  setup();
    
  for (;;)
    loop();
  
  return 0;
}

