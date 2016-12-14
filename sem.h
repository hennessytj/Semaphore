#define FALSE 0
#define TRUE  1
#define N 3 // num of shared resources controlled by semaphore
#define NUM_THREADS 5

typedef struct
{
    int turns;
    int bWaiting;
} Element;

typedef struct
{
    int n;
    Element *waitlist;
} Semaphore;
