#include <stdio.h>
#include <stdbool.h>

#define MAX 100

typedef struct
{
    int id, arrival, burst, remaining, completion, response;
    bool started, finished;
} Process;

typedef struct
{
    int time;
    int pid;
} Gantt;

Process p[MAX];
Gantt gantt[MAX * 10];
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
        p[i].remaining = p[i].burst;
        p[i].started = false;
        p[i].finished = false;
    }

    int time = 0, completed = 0;

    while (completed < n)
    {
        int idx = -1;
        int maxRemaining = -1;

        for (int i = 0; i < n; ++i)
        {
            if (p[i].arrival <= time && !p[i].finished && p[i].remaining > maxRemaining)
            {
                maxRemaining = p[i].remaining;
                idx = i;
            }
        }

        if (idx != -1)
        {
            recordGantt(time, p[idx].id);

            if (!p[idx].started)
            {
                p[idx].response = time - p[idx].arrival;
                p[idx].started = true;
            }

            p[idx].remaining--;
            time++;

            if (p[idx].remaining == 0)
            {
                p[idx].completion = time;
                p[idx].finished = true;
                completed++;
            }
        }
        else
        {
            recordGantt(time, -1); // idle
            time++;
        }
    }

    printGantt();
    printResults(n);

    return 0;
}
