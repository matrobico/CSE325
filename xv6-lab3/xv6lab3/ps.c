#include "types.h"
#include "user.h"
#include "stat.h"
#include "fcntl.h"

int
main(int argc, char ** argv)
{
  if (argc > 1) {
    printf(2, "Usage: ps\n");
    exit();
  }

  cps();

  exit();
}
