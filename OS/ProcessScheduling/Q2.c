// WAP to implement first come first served scheduling algorithm and find the
// average turnaround time, waiting time, completion time and response time for
// overall process. Also print Gantt Chart for it

#include <stdio.h>
#include <stdlib.h>

// Structure to represent a process
typedef struct Process
{
    int pid; // Process ID
    int arrivalTime;
    int burstTime;
    int completionTime;
    int turnaroundTime;
    int waitingTime;
    int responseTime;
    struct Process *next;
} Process;

// Function to create a process node
Process *createProcess(int pid, int arrivalTime, int burstTime)
{
    Process *newProcess = (Process *)malloc(sizeof(Process));
    newProcess->pid = pid;
    newProcess->arrivalTime = arrivalTime;
    newProcess->burstTime = burstTime;
    newProcess->completionTime = 0;
    newProcess->turnaroundTime = 0;
    newProcess->waitingTime = 0;
    newProcess->responseTime = 0;
    newProcess->next = NULL;
    return newProcess;
}

// Function to insert a process into the list sorted by arrival time
void insertProcess(Process **head, int pid, int arrivalTime, int burstTime)
{
    Process *newProcess = createProcess(pid, arrivalTime, burstTime);

    if (*head == NULL || (*head)->arrivalTime > arrivalTime)
    {
        newProcess->next = *head;
        *head = newProcess;
    }
    else
    {
        Process *current = *head;
        while (current->next != NULL && current->next->arrivalTime <= arrivalTime)
            current = current->next;
        newProcess->next = current->next;
        current->next = newProcess;
    }
}

// Function to calculate times and print Gantt Chart
void calculateFCFS(Process *head)
{
    int time = 0;
    float totalTAT = 0, totalWT = 0, totalRT = 0;
    Process *current = head;

    printf("\nGantt Chart:\n|");

    while (current != NULL)
    {
        if (time < current->arrivalTime)
        {
            time = current->arrivalTime;
        }

        current->completionTime = time + current->burstTime;
        current->turnaroundTime = current->completionTime - current->arrivalTime;
        current->waitingTime = current->turnaroundTime - current->burstTime;
        current->responseTime = time - current->arrivalTime;

        time = current->completionTime;

        // Gantt Chart block
        printf(" P%d |", current->pid);

        totalTAT += current->turnaroundTime;
        totalWT += current->waitingTime;
        totalRT += current->responseTime;

        current = current->next;
    }

    printf("\n\nPID\tAT\tBT\tCT\tTAT\tWT\tRT\n");

    current = head;
    while (current != NULL)
    {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               current->pid,
               current->arrivalTime,
               current->burstTime,
               current->completionTime,
               current->turnaroundTime,
               current->waitingTime,
               current->responseTime);
        current = current->next;
    }

    int count = 0;
    current = head;
    while (current != NULL)
    {
        count++;
        current = current->next;
    }

    printf("\nAverage Turnaround Time: %.2f", totalTAT / count);
    printf("\nAverage Waiting Time   : %.2f", totalWT / count);
    printf("\nAverage Response Time  : %.2f\n", totalRT / count);
}

// Main Function
int main()
{
    Process *head = NULL;

    // Sample processes (pid, arrivalTime, burstTime)
    insertProcess(&head, 1, 0, 4);
    insertProcess(&head, 2, 1, 3);
    insertProcess(&head, 3, 2, 1);
    insertProcess(&head, 4, 3, 2);
    insertProcess(&head, 5, 4, 5);

    calculateFCFS(head);

    return 0;
}