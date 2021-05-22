#include "scheduler.h"

void printDetail();
void printQuickHelp();
void increase1();
void increase10();
void decrease5();

int counter = 0;
int command_number = 0;
int sleep_us = 700000;

int main(){

    scheduler_start();
  
    int run = 1;

    printDetail();
    printQuickHelp();

    while(run){
        
        scanf("%d",&run);

        printf("-----> incoming command: %d\n",run);
        if(run == 1){

            add_to_scheduler((void*)increase1,SYSTEM,command_number++);
        }else if(run == 2){

            add_to_scheduler((void*)increase10,REAL_LOW,command_number++);
        }else if(run == 3){

            add_to_scheduler((void*)decrease5,REAL_HIGH,command_number++);
        }else if(run==4){

            list_table();
        }else if(run==5){

            printQuickHelp();
        }
    }

    scheduler_stop();
  
    return 0;
}

void printDetail(){

    printf("-----------------------------------------------------------------\n");
    printf("-                        Scheduler  Demo                        -\n");
    printf("-                        201971201 - DAA                        -\n");
    printf("-----------------------------------------------------------------\n");
    printf("-        sys   :  increase counter one by one for 5 sec         -\n");
    printf("-        r_low :  increase counter ten by ten for 5 sec         -\n");
    printf("-        r_high:  decrease counter one by one for 5 sec         -\n");
    printf("-        print :  prints pcb table                              -\n");
};

void printQuickHelp(){

    printf("-----------------------------------------------------------------\n");
    printf("- (0) quit, (1) sys, (2) r_low, (3) r_high, (4) print, (5) help -\n");
    printf("-----------------------------------------------------------------\n");
    printf("user: ");
}

void increase1(){

    for(int i=0;i<5;i++){

        counter++;
        usleep(sleep_us);
        printf("\rcount: %d user:",counter);
        fflush(stdout);
        //printf("%d \n",counter);
    }
}

void increase10(){

    for(int i=0;i<5;i++){

        counter+=10;
        usleep(sleep_us);
        printf("\rcount: %d user:",counter);
        fflush(stdout);
        //printf("%d \n",counter);
    }
}
 
void decrease5(){

    for(int i=0;i<5;i++){

        counter-=5;
        usleep(sleep_us);
        printf("\rcount: %d user:",counter);
        fflush(stdout);
        //printf("%d \n",counter);
    }
}