#include <stdio.h>

#include <Utils/threadPool.h>

int proc1(){
    printf("PROC 0 start\n");
    printf("PROC 0 end\n");
    return 0;
} 
int proc2(){
    printf("PROC 1 start\n");
    printf("PROC 1 end\n");
    return 0;
} 


int threadmain(){
    ThreadPool pool;
    int p1 = pool.M_addTask((threadProc)proc1, NULL);
    int p2 = pool.M_addTask((threadProc)proc2, NULL);

    pool.M_awaitProc(p1);

    printf("Proc 0 finished waiting\n");

    int a =0; 
    int &b = a;
    int c = b;
    c = 1;

    pool.M_awaitProc(p2);
    printf("Proc 1 finished waiting\n");

    pool.M_destroyAllThreads();
    pool.M_awaitAllThreads();
    return 0;
}