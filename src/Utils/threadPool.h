#pragma once

/*
TODO:
add returns somehow?
*/

#include <Windows.h>


#ifdef _IA64_
#define MemoryBarrier __mf
#endif


#include <thread>
#include <deque>
#include <mutex>
#include <vector>
#include <stdlib.h>


#define NUM_THREADS 1
#define THREAD_SLEEP_TIME 1//ms

enum ThreadState{
    THREAD_FREE, THREAD_OCCUPIED, 
    THREAD_STATE_COUNT
};

enum ProcState{
    PROC_PENDING, PROC_COMPLETE, PROC_PROCESSING,
    PROC_STATE_COUNT
};


typedef int (*threadProc)(void *args);

struct ProcInfo{
    int id;
    volatile int status;
    threadProc proc;
    void *args;
    size_t argsSize;
};

struct ThreadInfo {
    int id;
    volatile int isFree;
    volatile int quit;
    std::thread thread;
    ProcInfo* current;
};


class ThreadPool;

void W_threadProcLoop(ThreadPool* pool, ThreadInfo* thread);


/*
    M_* -> called from main thread
    W_* -> called from worker thread
*/
class ThreadPool{
private:
    ThreadInfo threads[NUM_THREADS];
    std::deque<ProcInfo> pending;
    std::deque<ProcInfo> processing;
    int32_t idCounter = 0;
public:
    volatile int32_t taskCount;
    std::mutex taskCountMutex;
    std::mutex procStatusMutex;
    ThreadPool(){
        taskCount = 0;
        for (int i=0; i<NUM_THREADS; i++){
            threads[i] = {
                i,
                THREAD_FREE,
                0,
                std::thread(W_threadProcLoop, this, &threads[i]), 
                NULL
            };

        }
    }
    ~ThreadPool(){}

    // pass the function pointer and a struct pointer with args
    // returns id of task 
    int M_addTask(threadProc proc, void *args){
        ProcInfo p = {};
        p.id = idCounter++;
        p.args = args;
        p.proc = proc;
        p.status = PROC_PENDING;
        pending.push_back(p);
        _InterlockedIncrement((volatile long *)&taskCount);
        return p.id;
    }

    // 
    void M_awaitAllThreads(){
        for (int i=0; i<NUM_THREADS; i++){
            // _InterlockedAnd((volatile long *)&threads[i].quit, 0x1);
            threads[i].thread.join();
        }
    }

    // 
    void M_destroyAllThreads(){
        
        for (int i=0; i<NUM_THREADS; i++){
            _InterlockedAnd((volatile long *)&threads[i].quit, 0x1);
            // delete(threads[i].thread);
        }
    }


    // wait till the given proc is 
    void M_awaitProc(int procId){
        ProcInfo *proc = NULL;
        // wait till the proc is in processing queue
        while (!proc){
            for (int i=0; i<processing.size(); i++){
                ProcInfo *p = &processing.at(i);
                if (p->id == procId){
                    proc = p;
                    break;
                }
            }
        }


        // wait till the proc status is complete
        thread_local int procStatus = PROC_PENDING;
        while (procStatus != PROC_COMPLETE){
            procStatusMutex.lock();
            procStatus = proc->status;
            procStatusMutex.unlock();
            printf("Waiting for proc to end...\n");
        }

    }


    // called by worker to get new task 
    void W_getProc(ThreadInfo *worker){
        ProcInfo p = pending.front();
        pending.pop_front();
        p.status = PROC_PROCESSING;
        processing.push_back(p);

        worker->current = &processing.back();
        worker->isFree = THREAD_OCCUPIED;

        // no need f898u
        _InterlockedDecrement((volatile long*)&taskCount);
        
        printf("[Pool] Dispatched proc %d to thread %d\n",p.id, worker->id);
    }

    // called by worker to indicate that proc is finished
    void W_finishProc(ThreadInfo *worker){
        printf("[Pool] Proc %d finished by thread %d\n",worker->current->id, worker->id);
        
        //MemoryBarrier();
        // lock since main thread might read this while waiting 
        procStatusMutex.lock();
        worker->current->status = PROC_COMPLETE;
        procStatusMutex.unlock();

        // free the current worker
        worker->current = NULL;
        worker->isFree = THREAD_FREE;
    }
    
};


void W_threadProcLoop(ThreadPool *pool, ThreadInfo *thread){
    int quit = 0;
    while (!quit){
        thread_local int taskCount = 0;
        // check queue for present work
        while (thread->isFree == THREAD_FREE){
            _sleep(THREAD_SLEEP_TIME);
            // mutex lock when reading count: if task present, the count decreases
            pool->taskCountMutex.lock();
            taskCount = pool->taskCount;
            printf("[Thread %d] Check\n", thread->id);
            if (taskCount > 0){
                pool->W_getProc(thread);
                printf("[Thread %d] Task gotten\n", thread->id);
            }
            pool->taskCountMutex.unlock();

            
            quit = thread->quit;    
            if (quit)
                goto end;    
        }

        // call thread proc
        thread->current->proc(thread->current->args);

        pool->W_finishProc(thread);
        
    }
    end:
    return;
    
}