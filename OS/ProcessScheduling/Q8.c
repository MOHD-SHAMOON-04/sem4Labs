#include <stdio.h>
#include <stdbool.h>

#define MAX 100

typedef struct
{
    int id;
    int arrival;
    int burst;
    int priority;
    int completion;
    int response;
    int waiting;
    int turnaround;
    bool finished;
    bool started;
} Process;

typedef struct
{
    int time;
    int pid;
} GanttEntry;

Process p[MAX];
GanttEntry gantt[MAX * 10];
int ganttCount = 0;

void recordGantt(int time, int pid)
{
    if (ganttCount == 0 || gantt[ganttCount - 1].pid != pid)
    {
        gantt[ganttCount].time = time;
        gantt[ganttCount].pid = pid;
        ganttCount++;
    }
}

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

void printResults(int n)
{
    float totalTAT = 0, totalWT = 0, totalRT = 0;
    printf("\nP\tAT\tBT\tP\tCT\tTAT\tWT\tRT\n");
    for (int i = 0; i < n; ++i)
    {
        p[i].turnaround = p[i].completion - p[i].arrival;
        p[i].waiting = p[i].turnaround - p[i].burst;
        p[i].response = p[i].response;

        totalTAT += p[i].turnaround;
        totalWT += p[i].waiting;
        totalRT += p[i].response;

        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].id, p[i].arrival, p[i].burst, p[i].priority,
               p[i].completion, p[i].turnaround, p[i].waiting, p[i].response);
    }

    printf("\nAverage TAT = %.2f", totalTAT / n);
    printf("\nAverage WT  = %.2f", totalWT / n);
    printf("\nAverage RT  = %.2f\n", totalRT / n);
}

int main()
{
    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    printf("Enter Arrival Time, Burst Time, and Priority for each process (lower number = higher priority):\n");
    for (int i = 0; i < n; ++i)
    {
        p[i].id = i + 1;
        printf("P%d Arrival Time: ", p[i].id);
        scanf("%d", &p[i].arrival);
        printf("P%d Burst Time: ", p[i].id);
        scanf("%d", &p[i].burst);
        printf("P%d Priority: ", p[i].id);
        scanf("%d", &p[i].priority);
        p[i].finished = false;
        p[i].started = false;
    }

    int time = 0, completed = 0;

    while (completed < n)
    {
        int idx = -1;
        int highestPriority = 1e9;

        for (int i = 0; i < n; ++i)
        {
            if (!p[i].finished && p[i].arrival <= time)
            {
                if (p[i].priority < highestPriority ||
                    (p[i].priority == highestPriority && p[i].arrival < p[idx].arrival))
                {
                    highestPriority = p[i].priority;
                    idx = i;
                }
            }
        }

        if (idx != -1)
        {
            p[idx].response = time - p[idx].arrival;
            recordGantt(time, p[idx].id);
            time += p[idx].burst;
            p[idx].completion = time;
            p[idx].finished = true;
            completed++;
        }
        else
        {
            recordGantt(time, -1);
            time++;
        }
    }

    printGantt();
    printResults(n);

    return 0;
}
