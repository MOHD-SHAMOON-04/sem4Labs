#include <stdio.h>
#include <stdbool.h>

#define MAX 100

typedef struct
{
    int id;
    int arrival;
    int burst;
    int remaining;
    int completion;
    int response;
    bool started;
} Process;

typedef struct
{
    int time;
    int pid;
} GanttEntry;

Process p[MAX];
GanttEntry gantt[MAX * 10]; // Enough for step-by-step timeline
int ganttCount = 0;

// Helper to record Gantt Chart
void recordGantt(int time, int pid)
{
    if (ganttCount == 0 || gantt[ganttCount - 1].pid != pid)
    {
        gantt[ganttCount].time = time;
        gantt[ganttCount].pid = pid;
        ganttCount++;
    }
}

// Print Gantt Chart
void printGantt()
{
    printf("\nGantt Chart:\n|");
    for (int i = 0; i < ganttCount; i++)
    {
        if (gantt[i].pid == -1)
            printf(" idle |");
        else
            printf(" P%d |", gantt[i].pid);
    }
    printf("\n");
}

// Calculate and print final results
void printResults(int n)
{
    float totalTAT = 0, totalWT = 0, totalRT = 0;
    printf("\nP\tAT\tBT\tCT\tTAT\tWT\tRT\n");
    for (int i = 0; i < n; ++i)
    {
        int tat = p[i].completion - p[i].arrival;
        int wt = tat - p[i].burst;
        int rt = p[i].response;
        totalTAT += tat;
        totalWT += wt;
        totalRT += rt;

        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].id, p[i].arrival, p[i].burst, p[i].completion,
               tat, wt, rt);
    }

    printf("\nAverage TAT = %.2f", totalTAT / n);
    printf("\nAverage WT  = %.2f", totalWT / n);
    printf("\nAverage RT  = %.2f\n", totalRT / n);
}

void scaffoldPreemptive(int n)
{
    int time = 0, completed = 0;

    while (completed < n)
    {
        // INSERT your scheduling logic here
        // Example: find shortest remaining time process, etc.

        int idx = -1; // Assume -1 is idle
        for (int i = 0; i < n; ++i)
        {
            // Choose a process to run based on your algorithm
            if (p[i].arrival <= time && p[i].remaining > 0)
            {
                idx = i;
                break; // placeholder, replace this logic
            }
        }

        if (idx != -1)
        {
            if (!p[idx].started)
            {
                p[idx].response = time - p[idx].arrival;
                p[idx].started = true;
            }
            p[idx].remaining--;
            recordGantt(time, p[idx].id);
            time++;
            if (p[idx].remaining == 0)
            {
                p[idx].completion = time;
                completed++;
            }
        }
        else
        {
            recordGantt(time, -1);
            time++;
        }
    }
}

void scaffoldNonPreemptive(int n)
{
    int time = 0, completed = 0;

    while (completed < n)
    {
        int idx = -1;
        for (int i = 0; i < n; ++i)
        {
            if (p[i].arrival <= time && p[i].remaining > 0)
            {
                idx = i;
                break; // placeholder, replace with selection logic
            }
        }

        if (idx != -1)
        {
            if (!p[idx].started)
            {
                p[idx].response = time - p[idx].arrival;
                p[idx].started = true;
            }
            recordGantt(time, p[idx].id);
            time += p[idx].burst;
            p[idx].remaining = 0;
            p[idx].completion = time;
            completed++;
        }
        else
        {
            recordGantt(time, -1);
            time++;
        }
    }
}

int main()
{
    int n, type;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    printf("Enter 1 for Preemptive, 0 for Non-Preemptive: ");
    scanf("%d", &type);

    printf("Enter Arrival and Burst Time for each process:\n");
    for (int i = 0; i < n; ++i)
    {
        p[i].id = i + 1;
        printf("P%d Arrival Time: ", p[i].id);
        scanf("%d", &p[i].arrival);
        printf("P%d Burst Time: ", p[i].id);
        scanf("%d", &p[i].burst);
        p[i].remaining = p[i].burst;
        p[i].started = false;
        p[i].response = -1;
    }

    if (type == 1)
        scaffoldPreemptive(n);
    else
        scaffoldNonPreemptive(n);

    printGantt();
    printResults(n);

    return 0;
}
