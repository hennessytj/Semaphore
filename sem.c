#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "sem.h"

int SHARED[N] = {0, 0, 0}; // shared resource controlled by semaphore
pthread_mutex_t lock;
Semaphore *SEM;

Semaphore *makeSemaphore()
{
    int i;
    Semaphore *s = (Semaphore *) malloc(sizeof(Semaphore));
    if (s == NULL) 
    {
        printf("Error: not enough memory\n");
        exit(0);
    }
    s->n = N;
    // waitlist array contains corresponding element for each thread
    s->waitlist = (Element *) malloc(sizeof(Element) * NUM_THREADS);
    if (s->waitlist == NULL)
    {
        printf("Error: not enough memory\n");
        exit(0);
    }
    for (i = 0; i < NUM_THREADS; i++)
    {
        s->waitlist[i].turns = 0;
        s->waitlist[i].bWaiting = FALSE;
    }
    return s;
}

void destroySemaphore(Semaphore *s)
{
    free(s->waitlist);
    free(s);
}

void wait(Semaphore *s, long id)
{
    int ret;
    ret = pthread_mutex_trylock(&lock);
    if (ret == 0 && s->n > 0)
    {   // Must have lock and semaphore
        // must be available
        s->waitlist[id].bWaiting = FALSE;
        s->n--;
        printf("%d dec n, n = %d (wait)\n", id, s->n);
        // Lock and semaphore both available
        pthread_mutex_unlock(&lock);
    }
    else
    {   // Release lock if locked, but semaphore is
        // not available
        if (ret == 0) pthread_mutex_unlock(&lock);
        usleep(rand() % (id * 100000));
        s->waitlist[id].turns++;
        s->waitlist[id].bWaiting = TRUE;
        return wait(s, id);
    }
}

void signal(Semaphore *s, long id)
{
    pthread_mutex_lock(&lock);
    s->n++;
    printf("%d inc n, n = %d (signal)\n", id, s->n);
    pthread_mutex_unlock(&lock);
}

void *entry(void *p)
{
    long pid = (long) p;
    wait(SEM, pid);
    printf("%d in CS, going down for a quick nap\n", pid);
    usleep(1000000);
    printf("%d in CS, just woke up\n", pid);
    signal(SEM, pid);
    pthread_exit(NULL);
}

int main(void)
{
    pthread_t tids[NUM_THREADS];
    long t;
    void *status;
    
    // Init section
    SEM = makeSemaphore();
    pthread_mutex_init(&lock, NULL);
    for (t = 0; t < NUM_THREADS; t++) 
        pthread_create(&tids[t], NULL, entry, (void *) t);

    // Clean up section
    for (t = 0; t < NUM_THREADS; t++)
        pthread_join(tids[t], &status);
    
    for (t = 0; t < NUM_THREADS; t++)
    {
        printf("%d waited %2d times, ", t, SEM->waitlist[t].turns);
        printf("still waiting?  %s\n", 
              !(SEM->waitlist[t].bWaiting) ? "no" : "yes");
    }

    pthread_mutex_destroy(&lock);
    destroySemaphore(SEM);
    
    return 0;
}
