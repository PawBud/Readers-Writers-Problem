// The following code is Written using the POSIX semaphores which can only be run on Linux Systems.
// A C++ implementation is possible but semaphores have been introduced in C++ 20. Linux ubuntu(20.04 focal fossa) doesn't have a stable gcc(v10.0 + required) compiler yet and hence can cause issues while compiling.
// References:-
// 1. https://www.geeksforgeeks.org/readers-writers-problem-set-1-introduction-and-readers-preference-solution/
// 2. https://www.geeksforgeeks.org/use-posix-semaphores-c/
// 3. https://shivammitra.com/reader-writer-problem-in-c/# (A very similar implementation to mine)
// 4. https://www.geeksforgeeks.org/system-call-in-c/
// 5. http://faculty.cs.niu.edu/~hutchins/csci480/semaphor.htm

// Writer's Priority Solution to the Reader-Writers Problem using Binary Semaphores:-

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

void *reader(void *);
void *writer(void *);
sem_t x, y, wsem, rsem;
int readcount = 0;
int writecount = 0;

void *reader(void *arg)
{
    int c = (int)arg;// this will produce warnings since pointer is being typecasted to integer
    printf("\nreader %d is created", c);
    sleep(1);
    sem_wait(&x);
    readcount++;
    if (readcount == 0)
        sem_wait(&wsem);
    sem_post(&x);
    sem_wait(&rsem);
    printf("\nreader %d is reading\n", c);
    sleep(1);
    printf("\nreader %d finished reading\n", c);
    sem_post(&rsem);
    sem_wait(&x);
    readcount--;
    if (readcount == 0)
        sem_post(&wsem);
    sem_post(&x);
}

void *writer(void *arg)
{
    int c = (int)arg; // this will produce warnings since pointer is being typecasted to integer
    printf("\nwriter %d is created", c);
    sleep(1);
    sem_wait(&wsem);
    sem_wait(&y);
    writecount++;
    if (writecount == 1)
        sem_wait(&rsem);
    sem_post(&wsem);
    sem_post(&y);
    sleep(1);

    /* Critcal Section */

    printf("\n\writer %d is writer\n ", c);
    sleep(1);
    printf("\nwriter%d finished writing\n", c);

    /* Critical section completed */
    sem_wait(&y);
    writecount--;
    if (writecount == 0)
        sem_post(&rsem);
    sem_post(&y);
}

void main()
{
    int a = 1, b = 1;
    //system("clear");
    sem_init(&wsem, 0, 1);
    sem_init(&x, 0, 1);
    sem_init(&rsem, 0, 1);
    sem_init(&y, 0, 1);

    pthread_t r, w, r1, w1, r2, r3, w2, w3;
    pthread_create(&r, NULL, reader, (void *)a);
    a++;
    pthread_create(&r1, NULL, reader, (void *)a);
    a++;
    pthread_create(&w, NULL, writer, (void *)b);
    b++;
    pthread_create(&w1, NULL, writer, (void *)b);
    b++;
    pthread_create(&r2, NULL, reader, (void *)a);
    a++;
    pthread_create(&w2, NULL, writer, (void *)b);
    b++;
    pthread_create(&r3, NULL, reader, (void *)a);
    a++;
    pthread_create(&w3, NULL, writer, (void *)b);
    pthread_join(r, NULL);
    pthread_join(r1, NULL);
    pthread_join(w, NULL);
    pthread_join(w1, NULL);
    pthread_join(r2, NULL);
    pthread_join(w2, NULL);
    pthread_join(r3, NULL);
    pthread_join(w3, NULL);
    printf("Process Terminated\n");
}

// To compiler enter the following command in the terminal:- gcc {fileName}.c -lpthread -lrt