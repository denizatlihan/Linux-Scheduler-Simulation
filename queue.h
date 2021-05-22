#include "stdio.h"
#include "stdlib.h"
#include "pcb.h"

static pthread_mutex_t lock_fcfs = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t lock_pb =  PTHREAD_MUTEX_INITIALIZER;

struct scheduler_node{

    struct pcb* p;
    struct scheduler_node* next;
    struct scheduler_node* prev;
};

struct scheduler_node* head_fcfs;
struct scheduler_node* tail_fcfs;

struct scheduler_node* head_pb;
struct scheduler_node* tail_pb;

void init_queues(){

    pthread_mutex_init(&lock_fcfs, NULL);
    pthread_mutex_init(&lock_pb, NULL);
}

void deinit_queues(){

    pthread_mutex_destroy(&lock_fcfs);
    pthread_mutex_destroy(&lock_pb);
}

void enque_fcfs(struct pcb* p){

    pthread_mutex_lock(&lock_fcfs);

    struct scheduler_node* node = (struct scheduler_node*) malloc(sizeof(struct scheduler_node));
    
    node->p = p;
    
    if(head_fcfs == NULL){

        head_fcfs = node;
        tail_fcfs = node;
    }else{

        tail_fcfs->next = node;
        tail_fcfs = node;
    }
    pthread_mutex_unlock(&lock_fcfs);
}

struct scheduler_node* deque_fcfs(){

    pthread_mutex_lock(&lock_fcfs);
    struct scheduler_node* ret = head_fcfs;

    if(head_fcfs==NULL){
        pthread_mutex_unlock(&lock_fcfs);
        return NULL;
    }

    if(head_fcfs->next!=NULL){

        head_fcfs = head_fcfs->next;
    }else{

        head_fcfs = NULL;
        tail_fcfs = NULL;
    }

    pthread_mutex_unlock(&lock_fcfs);
    return ret;
}

void enque_pb(struct pcb* p, int head){

    pthread_mutex_lock(&lock_pb);

    struct scheduler_node* node = (struct scheduler_node*) malloc(sizeof(struct scheduler_node));
    node->p = p;

    if(head_pb==NULL){

        head_pb = node;
        tail_pb = node;

    }else{

        if(head == 1){

            head_pb->prev = node;
            node->next = head_pb;
            head_pb = node;
        }else{

            tail_pb->next = node;
            node->prev =tail_pb;
            tail_pb = node;
        }
    }
    pthread_mutex_unlock(&lock_pb);
}

struct scheduler_node* deque_pb(){

    pthread_mutex_lock(&lock_pb);

    struct scheduler_node* ret = head_pb;

    if(head_pb==NULL){

        pthread_mutex_unlock(&lock_pb);
        return NULL;
    }

    if(head_pb->next!=NULL){

        head_pb = head_pb->next;
    }else{

        head_pb = NULL;
        tail_pb = NULL;
    }

    pthread_mutex_unlock(&lock_pb);

    return ret;
}
