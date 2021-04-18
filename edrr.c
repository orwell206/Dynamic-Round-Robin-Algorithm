#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

const int BURSTTIME = 0;
const int ARRIVALTIME = 1;
const int WAITINGTIME = 2; 
const int TURNAROUNDTIME = 3; 
const int QUEUED = 4; 
const int INITIALBURSTTIME = 5; 
int MAX = 0; 
bool menu = true;


// Queue Data Structure
struct queue {
    int *processId;  // Process Id
    int maxsize;     // maximum capacity of the queue
    int front;     	// front points to the front element in the queue 
    int rear;       // rear points to the last element in the queue
    int size;       // current capacity of the queue
};


// Initalise the Queue Data Strucure
struct queue* newQueue(int size){
    struct queue *pt = NULL;
    pt = (struct queue*)malloc(sizeof(struct queue));
    pt->processId = (int*)malloc(size * sizeof(int));
    pt->maxsize = size;
    pt->front = 0;
    pt->rear = -1;
    pt->size = 0;
    return pt;
}


int size(struct queue *pt){
	/*
		Params : struct queue *pt

		Desc : Returns the number of elements in the queue.
	*/
    return pt->size;
}
 

int isEmpty(struct queue *pt){
	/*
		Params : struct queue *pt

		Desc : Checks whether the queue is empty. 
	*/
    return !size(pt);
}


int front(struct queue *pt){
	/*
		Params : struct queue *pt

		Desc : Returns the first element of the queue.
	*/
	// Checks whether the queue is empty.
    if (isEmpty(pt))
    {
        printf("\tUnderflow\nProgram Terminated\n");
        exit(EXIT_FAILURE);
    }
    // Returns the first element of the queue if it is not empty. 
    return pt->processId[pt->front];
}
 

void enqueue(struct queue *pt, int pid){
	/*
		Params : struct queue *pt, int pid

		Desc : Enqueues an element at the back of the queue. 
	*/
	// Checks whether the queue is at its maxmimum limit.
    if (size(pt) == pt->maxsize)
    {
        printf("\tOverflow\nProgram Terminated\n");
        exit(EXIT_FAILURE);
    }
    // Enqueues the element to the back of the queue if it is not at its maximum limit. 
 	if (menu == false){
 		 printf("\t[+] Inserting P%d into the ready queue\n", pid + 1);
 	}
    pt->rear = (pt->rear + 1) % pt->maxsize;    
    pt->processId[pt->rear] = pid;
    pt->size++;
}
 

void dequeue(struct queue *pt){
	/*
		Params : struct queue *pt

		Desc : Dequeues the front element from the queue. 
	*/
	// Checks whether the queue is empty.
    if (isEmpty(pt)) 
    {
        printf("\tUnderflow\nProgram Terminated\n");
        exit(EXIT_FAILURE);
    }
 	// Removes the front element of the queue if it is not empty. 
    printf("\t[-] Removing P%d from the ready queue\n", front(pt) + 1);
    pt->front = (pt->front + 1) % pt->maxsize;    
    pt->size--;
}


int getMaxBurstTime(struct queue *pt, int processList[MAX][6]){
	/*
		Params : struct queue *pt, int processList[MAX][6]

		Desc : Finds the process with the maximum bursttime in processList and returns the maximum bursttime. 
	*/
	int processId = front(pt); 
	int maxBurstTime = processList[processId][BURSTTIME];
	for (int i = pt->front; i <= pt->rear; i++){
		processId = pt->processId[i];
		if (maxBurstTime < processList[processId][BURSTTIME]){
			maxBurstTime = processList[processId][BURSTTIME];
		}
	} 
	return maxBurstTime; 
}


void displayReadyQueue(struct queue *pt){
	/*
		Params : struct queue *ptPrints the ready queue structure. 

		Desc : Prints the ready queue structure. 
	*/
	printf("\tProcesses in Ready Queue: \n\t");
	if (pt->front > pt->rear){
		for (int i = pt->front; i < pt->maxsize; i++){
			printf("%d->", pt->processId[i] + 1);
		} 
		for (int i = 0; i <= pt->rear; i++){
			printf("%d->", pt->processId[i] + 1);
		} 
	}
	else {
		for (int i = pt->front; i <= pt->rear; i++){
			printf("%d->", pt->processId[i] + 1);
		} 
	}
	printf("\n\n");
}


void main(){
	/*
		Params : None. 

		Desc : Main function.
	*/
	int maxBurstTime;
	int readyQueueCount = 0;
	int noOfProcesses = 0;
	int remainingProcesses = 0;
	int quantum = 0;
	int exitTime = 0;
	int processId = 0;
	double totalWaitingTime = 0;
	double totalTurnAroundTime = 0;
	bool addedToQueue = false;

	// Prompt the user for the total number of processes.
	printf("Page Replacement Method: Efficient Dynamic Round Robin (EDRR)\n"); 
	printf("[*] Enter the number of processes: "); 
	scanf("%d", &noOfProcesses);

	// Creates the ready queue 
	struct queue *readyQueue = newQueue(noOfProcesses);

	// Creates the process list 
	MAX = noOfProcesses;
	int processList[MAX][6]; 

	// Prompt the user for the burst time for each process and stores them in a list of processes. 
	printf("[*] Enter the burst time sequence for each process (E.g. 45 90 70 38 55): ");  
	for (int i = 0; i < noOfProcesses; i++) {
		scanf("%d", &processList[i][BURSTTIME]);
		processList[i][INITIALBURSTTIME] = processList[i][BURSTTIME];
	}

	// Prompt the user for the arrival time for each process and stores them in a list of processes.
	printf("[*] Enter the arrival time sequence for each process (E.g. 0 5 8 15 20): ");
	for (int i = 0; i < noOfProcesses; i++){
		scanf("%d", &processList[i][ARRIVALTIME]);
		if (processList[i][ARRIVALTIME] == 0) {
			enqueue(readyQueue, i);
			processList[i][QUEUED] = true;
			readyQueueCount++; 			
		}
	}

	menu = false;

	// Find the quantum 
	maxBurstTime = getMaxBurstTime(readyQueue, processList);
	quantum = 0.8 * getMaxBurstTime(readyQueue, processList); 
	int a = 0;
	printf("[*] Initialising EDRR!\n");
	printf("\t[#] Initial Ready Queue\n");
	printf("\t[#] Initial Time Quantum is %d\n", quantum);
	displayReadyQueue(readyQueue);

	while (readyQueueCount != 0){
		if (a == readyQueueCount){
			quantum = getMaxBurstTime(readyQueue, processList);
			a = 0; 
		}

		// Get the current process id from the ready queue 
		processId = front(readyQueue); 

		// If the process burst time is smaller or equals to the quantum, remove from the queue. 
		if (processList[processId][BURSTTIME] <= quantum){
			exitTime = exitTime + processList[processId][BURSTTIME]; 
			processList[processId][TURNAROUNDTIME] = exitTime - processList[processId][ARRIVALTIME];
			processList[processId][WAITINGTIME] = processList[processId][TURNAROUNDTIME] - processList[processId][INITIALBURSTTIME];
			processList[processId][BURSTTIME] = 0;
			readyQueueCount--; 
			a--;
			dequeue(readyQueue);
		}

		// If the process burst time is larger than the quantum, add to the back of the queue. 
		else if (processList[processId][BURSTTIME] > quantum) {
			// If the ready queue only contains one process, there is no need to add it to the back of the queue. 
			if (size(readyQueue) == 1){
				exitTime = exitTime + quantum;
				processList[processId][TURNAROUNDTIME] = exitTime - processList[processId][ARRIVALTIME];
				processList[processId][WAITINGTIME] = processList[processId][TURNAROUNDTIME] - processList[processId][INITIALBURSTTIME];
				processList[processId][BURSTTIME] = processList[processId][BURSTTIME] - quantum;
			}
			// If the ready queue contains multiple processes, add the process to the back of the queue. 
			else{
				dequeue(readyQueue);
				enqueue(readyQueue, processId);
			}
		}

		// Iterate through the list of processes 
		for (int i = 0; i < noOfProcesses; i++){
			// If the processes has not finished executing and has not been added to the ready queue, add it to the queue. 
			if (processList[i][BURSTTIME] != 0 && processList[i][ARRIVALTIME] <= exitTime && processList[i][QUEUED] != true){
				enqueue(readyQueue, i);
				readyQueueCount++;
				processList[i][QUEUED] = true;
				addedToQueue = true;
			}
			// Recalculate the quantum each time a new process has been added to the queue. 
			if (addedToQueue == true){
				maxBurstTime = getMaxBurstTime(readyQueue, processList);
				quantum = 0.8 * maxBurstTime; 
			}
			addedToQueue = false;
		}

		// Display that the ready queue is empty if its empty.
		if (isEmpty(readyQueue)){
			printf("\t[!] Ready Queue is empty!\n");
		}

		// Display the ready queue structure. 
		else{
			printf("\t[#] Time Elapsed is %d seconds\n", exitTime);
			printf("\t[#] Time Quantum is %d seconds\n", quantum);
			displayReadyQueue(readyQueue);	
		}
		a++;
	}

	// Free the memory allocated to the ready queue
	free(readyQueue);

	// Display the waiting time and turnaround time for each process. 
	printf("[!] EDRR completed!\n");
	for (int i = 0; i < noOfProcesses; i++){
		printf("\t[P%d] Process %d\n", i + 1, i + 1); 
		printf("\t\tTurnaround time is %d seconds\n", processList[i][TURNAROUNDTIME]);
		printf("\t\tWaiting time is %d seconds\n", processList[i][WAITINGTIME]);
		totalWaitingTime = totalWaitingTime +  processList[i][WAITINGTIME];
		totalTurnAroundTime = totalTurnAroundTime + processList[i][TURNAROUNDTIME];
	}

	// Display the average waiting time and turnaround time. 
	printf("\n[+] Average waiting time is %.1f seconds\n", (totalWaitingTime/noOfProcesses));
	printf("[+] Average Turnaround time is %.1f seconds", (totalTurnAroundTime/noOfProcesses));
}