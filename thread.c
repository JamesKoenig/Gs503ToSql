#include <pthread.h>
#include <stdio.h>

/* returns a joinable thread */
pthread_t makeThread(void * (*f)(void * arg), void * arg)
{
    /* catches any error returned by pthread_create */
    int err;
    /* the thread we're going to return */
    pthread_t retVal;
    /* the attributes of the thread we're going to return */
    pthread_attr_t attr;

    /* initialize and set the thread attributes */
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    err = pthread_create(&retVal, &attr, f, arg);
    if(err)
    {
        fprintf(stderr, "ERROR: failed to spawn thread, err#: %d\n", err);
        pthread_attr_destroy(&attr);
        return (pthread_t) NULL;
    }
    else
    {
        pthread_attr_destroy(&attr);
        return retVal;
    }
}

