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
    int n, tq;
    printf("Enter number of processes: ");
    scanf("%d", &n);
    printf("Enter Time Quantum: ");
    scanf("%d", &tq);

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
    bool idle = false;

    while (completed < n)
    {
        bool progress = false;

        for (int i = 0; i < n; ++i)
        {
            if (p[i].arrival <= time && p[i].remaining > 0)
            {
                progress = true;
                recordGantt(time, p[i].id);

                if (!p[i].started)
                {
                    p[i].response = time - p[i].arrival;
                    p[i].started = true;
                }

                int execTime = (p[i].remaining >= tq) ? tq : p[i].remaining;
                time += execTime;
                p[i].remaining -= execTime;

                if (p[i].remaining == 0)
                {
                    p[i].completion = time;
                    p[i].finished = true;
                    completed++;
                }
            }
        }

        if (!progress)
        {
            recordGantt(time, -1); // idle
            time++;
        }
    }

    printGantt();
    printResults(n);

    return 0;
}
