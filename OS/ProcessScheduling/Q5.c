#include <stdio.h>
#include <stdbool.h>

#define MAX 100

typedef struct
{
    int id, arrival, burst, completion, response;
    bool finished;
} Process;

typedef struct
{
    int start;
    int pid;
} Gantt;

Process p[MAX];
Gantt gantt[MAX];
int ganttCount = 0;

void recordGantt(int time, int pid)
{
    gantt[ganttCount].start = time;
    gantt[ganttCount].pid = pid;
    ganttCount++;
}

void printGantt(int endTime)
{
    printf("\nGantt Chart:\n|");
    for (int i = 0; i < ganttCount; ++i)
        printf(" P%d |", gantt[i].pid);
    printf("\n0");

    for (int i = 0; i < ganttCount; ++i)
    {
        int end = (i == ganttCount - 1) ? endTime : gantt[i + 1].start;
        printf("    %d", end);
    }
    printf("\n");
}

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
               p[i].id, p[i].arrival, p[i].burst, p[i].completion, tat, wt, rt);
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

    for (int i = 0; i < n; ++i)
    {
        p[i].id = i + 1;
        printf("P%d Arrival Time: ", p[i].id);
        scanf("%d", &p[i].arrival);
        printf("P%d Burst Time: ", p[i].id);
        scanf("%d", &p[i].burst);
        p[i].finished = false;
    }

    int time = 0, completed = 0;

    while (completed < n)
    {
        int idx = -1;
        int maxBT = -1;

        for (int i = 0; i < n; ++i)
        {
            if (!p[i].finished && p[i].arrival <= time && p[i].burst > maxBT)
            {
                maxBT = p[i].burst;
                idx = i;
            }
        }

        if (idx != -1)
        {
            recordGantt(time, p[idx].id);
            p[idx].response = time - p[idx].arrival;
            time += p[idx].burst;
            p[idx].completion = time;
            p[idx].finished = true;
            completed++;
        }
        else
        {
            time++; // idle
        }
    }

    printGantt(time);
    printResults(n);

    return 0;
}
