// WAP to implement shortest job first non-preemptive scheduling algorithm and find the
// average turnaround time, waiting time, completion time and response time for
// overall process. Also print Gantt Chart for it

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct Process
{
    int pid;
    int arrivalTime;
    int burstTime;
    int completionTime;
    int turnaroundTime;
    int waitingTime;
    int responseTime;
    int isCompleted;
    struct Process *next;
} Process;

// Create a new process
Process *createProcess(int pid, int at, int bt)
{
    Process *p = (Process *)malloc(sizeof(Process));
    p->pid = pid;
    p->arrivalTime = at;
    p->burstTime = bt;
    p->completionTime = 0;
    p->turnaroundTime = 0;
    p->waitingTime = 0;
    p->responseTime = 0;
    p->isCompleted = 0;
    p->next = NULL;
    return p;
}

// Insert process at end
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

// Function to implement SJF non-preemptive scheduling
void sjfScheduling(Process *head)
{
    int currentTime = 0, completed = 0, totalProcesses = 0;
    float totalTAT = 0, totalWT = 0, totalRT = 0;
    Process *temp = head;

    // Count total number of processes
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
        int minBT = INT_MAX;

        // Select process with shortest burst time among arrived and not completed
        while (curr != NULL)
        {
            if (curr->arrivalTime <= currentTime && !curr->isCompleted)
            {
                if (curr->burstTime < minBT)
                {
                    minBT = curr->burstTime;
                    selected = curr;
                }
                else if (curr->burstTime == minBT && curr->arrivalTime < selected->arrivalTime)
                {
                    selected = curr;
                }
            }
            curr = curr->next;
        }

        if (selected == NULL)
        {
            currentTime++; // No process is ready, advance time
        }
        else
        {
            selected->responseTime = currentTime - selected->arrivalTime;
            currentTime += selected->burstTime;
            selected->completionTime = currentTime;
            selected->turnaroundTime = selected->completionTime - selected->arrivalTime;
            selected->waitingTime = selected->turnaroundTime - selected->burstTime;

            totalTAT += selected->turnaroundTime;
            totalWT += selected->waitingTime;
            totalRT += selected->responseTime;

            selected->isCompleted = 1;
            completed++;

            printf(" P%d |", selected->pid);
        }
    }

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

// Main function
int main()
{
    Process *head = NULL;

    // Example processes (pid, arrivalTime, burstTime)
    insertProcess(&head, 1, 1, 7);
    insertProcess(&head, 2, 2, 5);
    insertProcess(&head, 3, 3, 1);
    insertProcess(&head, 4, 4, 2);
    insertProcess(&head, 5, 5, 8);

    sjfScheduling(head);

    return 0;
}