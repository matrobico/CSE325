#include "types.h"
#include "stat.h"
#include "user.h"

int main(){
        int i = 0;
        int pid = 0;

        if ((pid = fork()) == 0){
                // printf(1, "Number of free pages before changing a variable process with pid: %d : %d\n", getpid(), getNumFreePages());
                i++;
                // printf(1, "Number of free pages after changing a variable process with pid: %d : %d\n", getpid(), getNumFreePages());
        } else {
                if ((pid = fork()) == 0){
                        // printf(1, "Number of free pages before changing a variable process with pid: %d : %d\n", getpid(), getNumFreePages());
                        i++;
                        // printf(1, "Number of free pages after changing a variable process with pid: %d : %d\n", getpid(), getNumFreePages());
                } else {
                        // printf(1, "Number of free pages before changing a variable process with pid: %d : %d\n", getpid(), getNumFreePages());
                        i++;
                        // printf(1, "Number of free pages after changing a variable process with pid: %d : %d\n", getpid(), getNumFreePages());

                        while(wait() != -1)
                                continue;
                }
        }

        return 0;
}