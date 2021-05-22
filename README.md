# Linux-Scheduler-Simulation
Scheduling tasks with respect that their priorities in Linux with C language

There are three different tasks for changing a global value. These tasks have different pirorities. 
Whenever the user wants to execute a task, this task is adding to one of two queues.

<b> FCFS Queue: </b> Lowest priorty level task are stored here.

<b> PB Queue: </b> High priority tasks are store at the end of this queue however highest priority level one are stored at the front of the queue

The scheduler pop the most prior task from these queries and execute it.
