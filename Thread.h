#ifndef EX2_THREAD_H
#define EX2_THREAD_H

#include <stdio.h>
#include <setjmp.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>


#define SECOND 1000000
#define STACK_SIZE 4096

#define RUN 0
#define READY 1
#define SLEEP 2
#define BLOCKED 3
#define SLEEP_BLOCKED 4

typedef unsigned long address_t;

class Thread {

public:
    sigjmp_buf env;

    Thread(int id, void (*f)(void),int stackSize, int priority);

    ~Thread();

    int getThreadID(){
        return threadID;
    }

    int getQuantumCounter(){
        return quantumCounter;
    }

    int getPriority(){
        return priority;}

    int getState(){
        return state;
    }

    sigjmp_buf* getEnv(){
        return &env;}

    void incQuantumCounter(){
        quantumCounter++;
    }

    void setState(int state);

    void setPriority(int new_priority);

private:

    char stack[STACK_SIZE];
    int threadID;
    int state;
    unsigned int quantumCounter;
    int priority;
};

#endif //EX2_THREAD_H
