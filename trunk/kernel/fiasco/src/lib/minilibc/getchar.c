#include <stdio.h>
#include "libc_backend.h"


int getchar(void)
{
  char s;
  if(__libc_backend_ins(&s,1)) 
    return s;
  else 
    return 0;
}
