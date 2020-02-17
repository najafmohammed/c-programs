#include <stdio.h>
#include <pthread.h>
// for rand
#include <stdlib.h>
// for sleep
#include <unistd.h>

pthread_t tId;
pthread_mutex_t readMutex, writeMutex;

int readerCount, writerCount;

void initialize()
{
    // initialize the mutex
    pthread_mutex_init(&readMutex, NULL);
    pthread_mutex_init(&writeMutex, NULL);
    readerCount = writerCount = 0;
}

void *reader(void *args)
{
    int waitTime;
    waitTime = rand() % 5;

    printf("\n[R] : Reader is trying to enter");

    // set read mutex to locked state
    pthread_mutex_lock(&readMutex);

    // increment read count
    ++readerCount;

    // give priority to reader on the first try by locking write mutex
    if (readerCount == 1)
    {
        pthread_mutex_lock(&writeMutex);
    }
    printf("\n--->  %d Reader(s) inside ", readerCount);

    // unlock reader mutex for other readers
    pthread_mutex_unlock(&readMutex);
    sleep(waitTime);

    printf("\n[R] : Reader %d is Leaving", readerCount);
    --readerCount;
    // if all readers left, unlock write mutex
    if (readerCount == 0)
    {
        pthread_mutex_unlock(&writeMutex);
    }
}

void *writer(void *args)
{
    int waitTime;
    waitTime = rand() % 3;

    printf("\n[W] : Writer is trying to enter");
    // lock write mutex
    pthread_mutex_lock(&writeMutex);

    ++writerCount;
    printf("\n--->  Writer %d has entered", writerCount);

    sleep(waitTime);
    // unlock write mutex
    pthread_mutex_unlock(&writeMutex);

    printf("\n[W] : Writer %d is leaving", writerCount);
    sleep(15);
}

void main()
{
    initialize();
    int readers, writers, i;
    printf("No of readers: ");
    scanf("%d", &readers);
    printf("No of writers: ");
    scanf("%d", &writers);
    // create threads
    
    for (i = 0; i < writers; ++i)
    {
        pthread_create(&tId, NULL, writer, NULL);
    }
    for (i = 0; i < readers; ++i)
    {
        pthread_create(&tId, NULL, reader, NULL);
    }
    sleep(15);
    printf("\n");
}
