#include "queue.h"
#include <string.h>

void scheduler_start();
void scheduler_stop();
void schedule();
void add_to_scheduler(void* task(), enum thread_priority priority, int command_number);
void* process(void* args);
void safe_execute(void (*f)());
void remove_from_queue();
void list_table();
void print_node_info(struct scheduler_node* node);

int ready_for_pop = 1;
long* pcb_list;
int upid = 0;
struct scheduler_node* running_node;
sem_t sem;
static pthread_mutex_t p_lock = PTHREAD_MUTEX_INITIALIZER;

void safe_execute(void (*f)()){
     (*f)();
}

void* process(void* args){
    
    sem_wait(&sem);
    
    if(args==NULL){
    
        printf("errror\n");
    
    }else{

        ready_for_pop = 0;

        struct scheduler_node* node = args;

        pthread_mutex_lock(&p_lock);

        struct pcb* p = node->p;

        p->state = RUNNING;
        running_node = node;

        safe_execute(p->task);

        p->state = COMPLETED;
        running_node = NULL;

        ready_for_pop = 1;

        free(p);

        pthread_mutex_unlock(&p_lock);

        schedule();
    }

    sem_post(&sem);

    return NULL;
}

void remove_from_queue(){

    pthread_mutex_lock(&lock_fcfs);

    struct scheduler_node* node = head_fcfs;
    
    pthread_mutex_lock(&p_lock);
    
    while(node!=NULL){

        struct pcb* p = node->p;


        if(p->state == COMPLETED){

            pthread_join(p->address,NULL);

            struct scheduler_node* temp = node;
            node = node->next;   
            free(temp);
        }else{

            node = node->next;
        }
   
    }
    
    pthread_mutex_unlock(&p_lock);
    pthread_mutex_unlock(&lock_fcfs);
    pthread_mutex_lock(&lock_pb);

    node = head_pb;
    
    pthread_mutex_lock(&p_lock);

    while(node!=NULL){

        struct pcb* p = node->p;


        if(p->state == COMPLETED){

            pthread_join(p->address,NULL);

            struct scheduler_node* temp = node;
            node = node->next;   
            free(temp);
        }else{

            node = node->next;   
        }

 
    }
    pthread_mutex_unlock(&p_lock);

    pthread_mutex_unlock(&lock_pb);

}

void schedule(){

    if(ready_for_pop == 0){
    
        return;
    }
    
    remove_from_queue();

    struct scheduler_node* node = deque_pb();

    if(node==NULL){

        node = deque_fcfs();
    }

    if(node!=NULL){

        pthread_t thread;

        pthread_create(&thread,NULL,&process,node);

        node->p->address = thread;
    }

}

void add_to_scheduler(void* task(), enum thread_priority priority, int command_number){
    
    struct pcb* p = (struct pcb*)malloc(sizeof(struct pcb));

    p->semaphore = &sem;
    p->state =  WAITING;
    p->id = upid++;
    p->priority = priority;
    p->name = command_number;
    p->task = task;

    if(priority==SYSTEM){

        enque_fcfs(p);
        
    }else  if(priority==REAL_LOW){
        
        enque_pb(p,0);
        
    }else  if(priority==REAL_HIGH){
        
        enque_pb(p,1);
    }

    schedule();
}

void scheduler_start(){
    printf("INFO: scheduler is starting...\n");
    init_queues();
    sem_init(&sem,0,1);
    printf("INFO: scheduler has started.\n\n");
}

void scheduler_stop(){
    printf("INFO: scheduler is closing...\n");
    sem_destroy(&sem);
    deinit_queues();
    printf("INFO: scheduler has closed.\n\n");
}

void list_table(){
    
    struct scheduler_node* node;

    pthread_mutex_lock(&lock_pb);
    printf("\n-------------------------------------------\n");
    printf("- id    : name          : type  : state   -\n");
    printf("-------------------------------------------\n");

    if(running_node!=NULL){

        print_node_info(running_node);
    }

    node = head_pb;
    
    while(node!=NULL){

        print_node_info(node);
        node = node->next;    
    }

    pthread_mutex_unlock(&lock_pb);

    pthread_mutex_lock(&lock_fcfs);

    node = head_fcfs;
    
    while(node!=NULL){

        print_node_info(node);
        node = node->next;    
    }
    
    printf("-------------------------------------------\n\n");
    printf("user: ");
    pthread_mutex_unlock(&lock_fcfs);

}

void print_node_info(struct scheduler_node* node){

    printf("- %ld\t: taskn%d\t: %s\t: %s -\n",node->p->id,node->p->name,get_prio_name(node->p->priority),get_state_name(node->p->state));
}