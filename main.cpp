#include "uthreads.h"
#include <iostream>

void f()
{
    int i = 0;
    while(1){
        if (i % 100000000 == 0)
        {
            std::cout << "f: " << uthread_get_quantums(uthread_get_tid()) << std::endl;
        }
        i++;
    }
}

void g()
{
    int i = 0;
    while(1){
        if (i % 100000000 == 0)
        {
            std::cout << "g: " << uthread_get_quantums(uthread_get_tid()) << std::endl;
        }
        i++;

    }
}

int main ()
{
    int q[1] = {1000};
    uthread_init(q,1);
    uthread_spawn(f,0);
    uthread_spawn(g,0);
    int i = 0;
    while(1){
        if (i % 100000000 == 0)
        {
            std::cout << "main: " << uthread_get_quantums(uthread_get_tid()) << std::endl;
        }
        i++;
        if (i == 1500000000)
        {
            std::cout << "BLOCKED" << std::endl;
            uthread_block(2);
        }
        if (i == 2000000000)
        {
            std::cout << "TERMINATED" << std::endl;
            uthread_terminate(1);
        }
        if (i == -1500000000)
        {
            std::cout << "RESUMED" << std::endl;
            uthread_resume(2);
        }
        if (i == -1000000000)
        {
//            uthread_terminate(0);
        }
    }
}
