#ifndef _THREAD_ENCAPSULATION_HEADER_
#define _THREAD_ENCAPSULATION_HEADER_
#include <pthread.h>

pthread_t makeThread(void * (*f)(void * arg), void * arg);

#endif /* _THREAD_ENCAPSULATION_HEADER_ */
