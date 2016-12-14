compilation: % gcc sem.c -lpthread
execution:   % /a.out

Parameters
-----------------------------------------------------------------------------
There are no command line parameters.  If you would like to change the 
number of threads or the number of shared resources go to sem.h.

Misc.
-----------------------------------------------------------------------------
The file sem.c contains the source code which includes: semaphore operations,
pthread operations, i.e., what I believe is required for this program.

The file sem.h contains macros and struct defintions.  This is the file
you should open to change the number of resources or threads in play.  To
test as a binary semaphore, set N to 1.  To test as a counting semaphore, set
N to any number > 1. 
