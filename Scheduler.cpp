//******************************************INCLUDES***********************************************
#include "Scheduler.h"
//*************************************************************************************************

#define EXIT_FAILURE 1
#define SUCCESS 0
#define FAILURE -1

using namespace std;

//*************************************************************************************************
Scheduler::Scheduler(): readyThreadsList(), blockedThreadsList()
{
//todo - understand what's needed here
}

Scheduler::~Scheduler()  = default;

int Scheduler::addNewThread(int threadID)
{
    //allthread.add(threadID); todo
    readyThreadsList.push_back(threadID);//todo - map? thread.getID
    quantumTime =
    numOfThreads++;
    return SUCCESS;
}
/**
 * Switch threads when the Quantum time is over
 * @return running thread.
 */
int Scheduler::quantumChange() {

    readyThreadsList.push_back(runningThreadId);
    runningThreadId = readyThreadsList.front();
    readyThreadsList.pop_front();
    return runningThreadId;
}

void Scheduler::popFrontReady(){
    runningThreadId = readyThreadsList.front();
    readyThreadsList.pop_front();
}


bool Scheduler::checkMaxNumThread(){
    return (numOfThreads == MAX_THREADS);
}

bool Scheduler::checkReadyThreads() {
    return not readyThreadsList.empty();
}

