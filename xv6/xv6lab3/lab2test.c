#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int
main(int argc, char * argv[])
{
  int priority, pid;

  if (argc < 3) {
    printf(2, "Usage: lab2test [pid] [priority]\n");
    exit();
  }

  pid = atoi(argv[1]);
  priority = atoi(argv[2]);
  
  if (priority < 0) {
    printf(2, "Nice value set below expected range, clamping to 0\n");
    priority = 0;
  } else if (priority > 39) {
      printf(2, "Nice value set above expected range, clamping to 39\n");
      priority = 39;
  }

  setpriority(pid, priority);
  //printf(2, "You've changed the process with pid %d to have a nice value of %d\n", pid, priority);

  int nice = getpriority(pid);
  if (nice == -1) {
    printf(2, "Hmm, looks like it was unsuccesful. Maybe the pid for that process doesn't exist?\n");
  } else {
    printf(2, "You've changed the process with pid %d to have a nice value of %d\n", pid, priority);
  }  
  printf(2, "Nice value of process with pid %d: %d\n", pid, nice);

  exit();
}
