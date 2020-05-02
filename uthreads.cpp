//******************************************Includes*******************************************************
#include <iostream>
#include "uthreads.h"
#include "Scheduler.h"
#include "Thread.h"
#include <vector>
#include <queue>
//*********************************************************************************************************
#define SUCCESS 0
#define FAILURE -1

using namespace std;
//*******************************************Variables*****************************************************
priority_queue<int, std::vector<int>, greater<int>> freeIDs; //priority queue for free IDs
Scheduler scheduler;
Thread *threads[MAX_THREADS];
//bool takenIDs[MAX_THREADS]; todo - understand if needed
unsigned int quantumDuration;
struct sigaction sigAction;
struct itimerval timer;
sigset_t alarmSignal;
int *qunatum_time;

//*************************************Function Declarations*************************************************
void generateAllThreadsID();
void initAllthreads();
int getTimeforPriority(int priority);
void cleanAndExit(int exitcode);
//************************************************************************************************************


/**
 * Blocking the alarm signal in critical sections.
 */
void blockAlarmSignal()
{
    sigprocmask(SIG_BLOCK, &alarmSignal, &threads[scheduler.getRunningId()]->mask);
}

/**
 * Set the mask to the running-thread's regular mask.
 */
void setToThreadMask()
{
    sigprocmask(SIG_SETMASK, &threads[scheduler.getRunningId()]->mask, nullptr);
}

/**
 * Handling with the timer signals - switching to next thread
 * @param sig
 */
void timer_handler(int sig)
{
    blockAlarmSignal();

    if (scheduler.checkReadyThreads() == true)
    {
        int runningTid = scheduler.getRunningId();
        if (sigsetjmp(threads[runningTid]->env, 1) == 5) {
            //if the return value of sigsetjump != 0, save save the current signal mask as well
            setToThreadMask();
            return;
        }
        //switch to the next thread in the waiting list
        runningTid = scheduler.quantumChange();
        //sets a new timer for the new thread according to its priority
        scheduler.setTimer(threads[runningTid]->getPriority());

        siglongjmp(threads[runningTid]->env, 5);
    }
    setToThreadMask();
}

/**
 * Sets the timer.
 * @return 0 on success, -1 otherwise.
 */
int setTimer(int quantum_usecs)
{
    sigAction.sa_handler = &timer_handler; //sets a new handler for the alarm signal
    if (sigaction(SIGVTALRM, &sigAction, nullptr) < 0) {
        fprintf(stderr, "system error: Sigaction Error.");
    }
    timer = {0}; //todo - understand wtf
    int sec = quantum_usecs/1000000;
    int usec= quantum_usecs%1000000;

    timer.it_value.tv_sec = sec;
    timer.it_value.tv_usec = usec;

    timer.it_interval.tv_sec = sec;
    timer.it_interval.tv_usec = usec;

    // Start a virtual timer. It counts down whenever this process is executing.
    if (setitimer(ITIMER_VIRTUAL, &timer, nullptr)) {
        cerr << ("system call error: setitimer error.")<< endl;
        cleanAndExit(EXIT_FAILURE);
    }
    return SUCCESS;
}


int uthread_init(int *quantum_usecs, int size)
{
    for(int i =0 ;i<size ;i++)
    {
        if(quantum_usecs[i] <= 0)
        {
            cerr << "thread library error: Wrong input - invalid qunatum duration input" << endl;
            return FAILURE;
        }
    }
    sigaddset(&alarmSignal, SIGALARM); //adding alarm signal to the signal set

    qunatum_time = quantum_usecs;
    generateAllThreadsID();
    //Initializing all threads
    initAllthreads();
    //Setting the Main thread
    threads[0] = new Thread(0, nullptr,STACK_SIZE, 0);
    threads[0]->setState(RUN);
    //threads[0]->incQuantumCounter();
    //Setting the timer
    quantumDuration = getTimeforPriority(0);
    scheduler.addNewThread(threads[0]->getThreadID());
    setTimer(quantumDuration);

    return SUCCESS;
}

/**
 * Genereates all threads IDs
 */
void generateAllThreadsID() {
    for(int i=0;i<MAX_THREADS;i++){
        freeIDs.push(i);
    }
}

/**
 * Initializing all the threads
 */
void initAllthreads(){
    for(auto &thread: threads)
    {
        thread = nullptr;
    }
}
/**
 * Checks if the thread's ID is valid
 * @param threadID
 * @return
 */
int checkID(int threadID){
    if(threadID < 0 || threadID > 99){
        cerr << "thread libarary error: invalid thread id input"<< endl;
        return FAILURE;
    }
    else if (threads[threadID] == nullptr) {
        cerr << "thread library error: no thread with id = "<< threadID << endl;
        return FAILURE;
    }
    return SUCCESS;
}


int generateTid()
{
    int nextFreeId = freeIDs.top();
    freeIDs.pop();
    return nextFreeId;
}


int uthread_spawn(void (*f)(void), int priority)
{
    blockAlarmSignal(); //blocking the alaram signals for the current running thread.
    //Check if number of threads reached the max number
    scheduler.checkMaxNumThread();
    int tid = generateTid();
    if(checkID(tid) != 0)
    {
        //unMaskSignals(); //todo - write this function to unblock all the signals
        return FAILURE;
    }
    threads[tid] = new Thread(tid, f, STACK_SIZE, priority);
    scheduler.addNewThread(tid); //push to ready list
    //setTimer(threads[tid]->getPriority()); think if it's needed here************
    setToThreadMask(); //todo - understand wtf
    return tid;
}

/**
 * Gets the quantum time per priority
 * @param priority
 * @return priority's quantum time
 */
int getTimeforPriority(int priority)
{
    return qunatum_time[priority];
}


void cleanAndExit(int exitcode) {

}


int uthread_change_priority(int tid, int priority){
    if(tid == scheduler.getRunningId()){
        //todo - wait until it terminates and then change the priority
    }
    threads[tid]->setPriority(priority);
}

int uthread_get_tid(){
    return scheduler.getRunningId();
}





