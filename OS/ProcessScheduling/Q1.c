// Program to implement the priority queue scheduling algorithm using linked list

#include <stdio.h>
#include <stdlib.h>

struct Process
{
    int id;
    int burst_time;
    int priority;
    struct Process *next;
};

struct Process *createProcess(int id, int burst_time, int priority)
{
    struct Process *newProcess = (struct Process *)malloc(sizeof(struct Process));
    newProcess->id = id;
    newProcess->burst_time = burst_time;
    newProcess->priority = priority;
    newProcess->next = NULL;
    return newProcess;
}

void insertProcess(struct Process **head, struct Process *newProcess)
{
    if (*head == NULL || (*head)->priority > newProcess->priority)
    {
        newProcess->next = *head;
        *head = newProcess;
    }
    else
    {
        struct Process *current = *head;
        while (current->next != NULL && current->next->priority <= newProcess->priority)
        {
            current = current->next;
        }
        newProcess->next = current->next;
        current->next = newProcess;
    }
}

void displayQueue(struct Process *head)
{
    struct Process *current = head;
    printf("Process ID \t Burst Time \t Priority \n");
    while (current != NULL)
    {
        printf("%3d \t %3d \t %3d \n", current->id, current->burst_time, current->priority);
        current = current->next;
    }
}

// Function to remove and execute the highest priority process
void executeProcess(struct Process **head)
{
    if (*head == NULL)
    {
        printf("No processes to execute.\n");
        return;
    }

    struct Process *temp = *head;
    *head = (*head)->next;

    printf("Executing Process ID: %d with Burst Time: %d and Priority: %d\n", temp->id, temp->burst_time, temp->priority);
    free(temp); // Free the memory of the executed process
}

// void freeQueue(struct Process* head) {
//     struct Process* current = head;
//     while (current != NULL) {
//         struct Process* temp = current;
//         current = current->next;
//         free(temp);
//     }
// }

int main()
{
    struct Process *queue = NULL;
    int n, id, burst_time, priority;

    printf("Enter the number of processes : ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++)
    {
        printf("Enter Process ID, Burst Time, and Priority : ");
        scanf("%d %d %d", &id, &burst_time, &priority);
        struct Process *newProcess = createProcess(id, burst_time, priority);
        insertProcess(&queue, newProcess);
    }

    printf("\nPriority Queue:\n");
    displayQueue(queue);

    while (queue != NULL)
    {
        executeProcess(&queue);
    }

    // freeQueue(queue);
    return 0;
}