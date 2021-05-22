#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

enum thread_priority{

    SYSTEM, REAL_LOW, REAL_HIGH
};

enum thread_state{

    RUNNING, WAITING, COMPLETED
};

struct pcb{

    long id;
    int name;
    void* (*task)();
    sem_t* semaphore;
    pthread_t address;
    enum thread_state state;
    enum thread_priority priority;
};

char* get_prio_name(enum thread_priority e){

    if(e == SYSTEM){

        return "SYSTM"; 
    }else if(e == REAL_LOW){
        
        return "R_LOW"; 
    }else if(e == REAL_HIGH){
        
        return "R_HGH"; 
    }
}

char* get_state_name(enum thread_state e){

    if(e == RUNNING){

        return "RUNNING"; 
    }else if(e == WAITING){
        
        return "WAITING"; 
    }else if(e == COMPLETED){
        
        return "COMPLETED"; 
    }
}