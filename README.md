# CSE325

## Lab 3

### Task 1
After adding cps(), the initial output after typing `ps` was:

```bash
$ ps
name	pid	state	priority
init	1	SLEEPING	10
sh	2	SLEEPING	10
ps	4	RUNNING		10
```

### Task 2
Output using `ps` after running `lab3test` to create a few processes:
 ```bash
$ lab3test 4 &
$ Parent creating child 5
Child 5 created
ps
name	pid	state	priority
init	1	SLEEPING	10
sh	2	SLEEPING	10
lab3test	5	RUNNING		20
lab3test	4	SLEEPING	10
ps	6	RUNNING		10
 ```

 Output using `ps` after changing the priority of `lab3test` using `nice`:
 ```bash
$ nice 5 18
$ ps
name	pid	state	priority
init	1	SLEEPING	10
sh	2	SLEEPING	10
lab3test	5	RUNNING		18
lab3test	4	SLEEPING	10
ps	8	RUNNING		10
 ```

### Task 3

The general tendency of the round robin is to rotate the state of the children executing as such:

At first, `ps` outputted this:

```bash
ps
name     pid     state   priority 
init     1       SLEEEPING       10 
sh       2       SLEEEPING       10 
ps       12      RUNNING         10 
lab3test         10      SLEEEPING       10 
lab3test         5       SLEEEPING       10 
lab3test         7       SLEEEPING       10 
lab3test         8       RUNNABLE        20 
lab3test         9       RUNNABLE        20 
lab3test         11      RUNNING         20
```

A little later it outputted this:

```bash
$ ps
name     pid     state   priority 
init     1       SLEEEPING       10 
sh       2       SLEEEPING       10 
ps       14      RUNNING         10 
lab3test         10      SLEEEPING       10 
lab3test         5       SLEEEPING       10 
lab3test         7       SLEEEPING       10 
lab3test         8       RUNNING         20 
lab3test         9       RUNNABLE        20 
lab3test         11      RUNNABLE        20 
```

The state of `RUNNING` will be rotated between the two processes with the `RUNNABLE` status, and when the process with the status of `RUNNING` is rotated out it will be given the status of `RUNNABLE`

### Task 4

```bash
$ ps
name     pid     state   priority 
init     1       SLEEEPING       10 
sh       2       SLEEEPING       10 
lab3test         11      RUNNABLE        20 
lab3test         9       SLEEEPING       10 
lab3test         5       SLEEEPING       10 
lab3test         7       SLEEEPING       10 
lab3test         8       RUNNING         20 
lab3test         10      RUNNABLE        20 
ps       18      RUNNING         10 
```
