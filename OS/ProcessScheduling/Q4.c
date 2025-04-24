// WAP to implement shortest remaining first non-preemptive scheduling algorithm (shortest
//  job pre-emptive) and find the average turnaround time, waiting time, completion time and
//  response time for overall process. Also print Gantt Chart for it

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct Process
{
    int pid;
    int arrivalTime;
    int burstTime;
    int remainingTime;
    int completionTime;
    int turnaroundTime;
    int waitingTime;
    int responseTime;
    int started;
    int isCompleted;
    struct Process *next;
} Process;

Process *createProcess(int pid, int at, int bt)
{
    Process *p = (Process *)malloc(sizeof(Process));
    p->pid = pid;
    p->arrivalTime = at;
    p->burstTime = bt;
    p->remainingTime = bt;
    p->completionTime = 0;
    p->turnaroundTime = 0;
    p->waitingTime = 0;
    p->responseTime = -1; // -1 means not yet started
    p->started = 0;
    p->isCompleted = 0;
    p->next = NULL;
    return p;
}

void insertProcess(Process **head, int pid, int at, int bt)
{
    Process *newNode = createProcess(pid, at, bt);
    if (*head == NULL)
    {
        *head = newNode;
    }
    else
    {
        Process *temp = *head;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = newNode;
    }
}

void srtfScheduling(Process *head)
{
    int currentTime = 0, completed = 0, totalProcesses = 0;
    float totalTAT = 0, totalWT = 0, totalRT = 0;
    Process *temp = head;
    Process *prevProcess = NULL;

    while (temp)
    {
        totalProcesses++;
        temp = temp->next;
    }

    printf("\nGantt Chart:\n|");
    while (completed != totalProcesses)
    {
        Process *curr = head;
        Process *selected = NULL;
        int minRT = INT_MAX;

        // Select the process with the smallest remaining time
        while (curr != NULL)
        {
            if (curr->arrivalTime <= currentTime && !curr->isCompleted && curr->remainingTime < minRT)
            {
                minRT = curr->remainingTime;
                selected = curr;
            }
            else if (curr->remainingTime == minRT && curr->arrivalTime < selected->arrivalTime)
            {
                selected = curr;
            }
            curr = curr->next;
        }

        if (selected == NULL)
        {
            printf("_ |");
            currentTime++;
        }
        else
        {
            if (selected->responseTime == -1)
            {
                selected->responseTime = currentTime - selected->arrivalTime;
            }

            // Gantt chart display for every switch or first time
            if (prevProcess != selected)
            {
                printf(" P%d |", selected->pid);
                prevProcess = selected;
            }

            selected->remainingTime--;
            currentTime++;

            if (selected->remainingTime == 0)
            {
                selected->completionTime = currentTime;
                selected->turnaroundTime = selected->completionTime - selected->arrivalTime;
                selected->waitingTime = selected->turnaroundTime - selected->burstTime;
                selected->isCompleted = 1;

                totalTAT += selected->turnaroundTime;
                totalWT += selected->waitingTime;
                totalRT += selected->responseTime;
                completed++;
            }
        }
    }

    // Print process table
    printf("\n\nPID\tAT\tBT\tCT\tTAT\tWT\tRT\n");
    temp = head;
    while (temp)
    {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               temp->pid,
               temp->arrivalTime,
               temp->burstTime,
               temp->completionTime,
               temp->turnaroundTime,
               temp->waitingTime,
               temp->responseTime);
        temp = temp->next;
    }

    printf("\nAverage Turnaround Time: %.2f", totalTAT / totalProcesses);
    printf("\nAverage Waiting Time   : %.2f", totalWT / totalProcesses);
    printf("\nAverage Response Time  : %.2f\n", totalRT / totalProcesses);
}

// Main Function
int main()
{
    Process *head = NULL;

    // Sample processes (pid, arrivalTime, burstTime)
    insertProcess(&head, 1, 0, 7);
    insertProcess(&head, 2, 1, 5);
    insertProcess(&head, 3, 2, 3);
    insertProcess(&head, 4, 3, 1);
    insertProcess(&head, 5, 4, 2);
    insertProcess(&head, 6, 5, 1);

    srtfScheduling(head);

    return 0;
}