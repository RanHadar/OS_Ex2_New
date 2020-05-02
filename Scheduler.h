#ifndef EX2_SCHEDULER_H
#define EX2_SCHEDULER_H

#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <list>
#include "Thread.h"

#define MAX_THREADS 100

class Scheduler {

public:

    explicit Scheduler();

    ~Scheduler();

    int addNewThread(int threadID);

    int quantumChange();

    int getRunningId() { return runningThreadId; }

    int terminate(int theadID);

    int block(int threadID);

    void resume(int treadID);

    int setTimer(int quantum_usecs);

    void cleanAndExit(int exitCode);

    bool checkMaxNumThread();

    bool checkReadyThreads();

private:

    //Thread *runningThread;
    std::list<int> blockedThreadsList;
    std::list<int> readyThreadsList;
    void popFrontReady();
    int numOfThreads;
    int quantomTimePassed;
    int quantumTime;
    int runningThreadId;
};


#endif //EX2_SCHEDULER_H
