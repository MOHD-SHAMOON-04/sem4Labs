#include <stdio.h>
#include <stdbool.h>

#define MAX 10

typedef struct
{
    int id;
    int arrival;
    int burst;
    int remaining;
    int priority;
    int completion;
    int response;
    bool started;
} Process;

void sortByArrival(Process p[], int n)
{
    for (int i = 0; i < n - 1; ++i)
        for (int j = i + 1; j < n; ++j)
            if (p[i].arrival > p[j].arrival)
            {
                Process tmp = p[i];
                p[i] = p[j];
                p[j] = tmp;
            }
}

int main()
{
    int n;
    Process p[MAX];

    printf("Enter number of processes: ");
    scanf("%d", &n);

    printf("Enter Arrival Time, Burst Time and Priority (lower number = higher priority):\n");
    for (int i = 0; i < n; ++i)
    {
        p[i].id = i + 1;
        printf("P%d Arrival Time: ", p[i].id);
        scanf("%d", &p[i].arrival);
        printf("P%d Burst Time: ", p[i].id);
        scanf("%d", &p[i].burst);
        printf("P%d Priority: ", p[i].id);
        scanf("%d", &p[i].priority);
        p[i].remaining = p[i].burst;
        p[i].started = false;
        p[i].response = -1;
    }

    sortByArrival(p, n);

    int complete = 0, time = 0;
    float totalTAT = 0, totalWT = 0, totalRT = 0;

    printf("\nGantt Chart:\n|");
    while (complete < n)
    {
        int idx = -1;
        int highestPriority = 9999;

        for (int i = 0; i < n; ++i)
        {
            if (p[i].arrival <= time && p[i].remaining > 0)
            {
                if (p[i].priority < highestPriority)
                {
                    highestPriority = p[i].priority;
                    idx = i;
                }
                else if (p[i].priority == highestPriority)
                {
                    if (p[i].arrival < p[idx].arrival)
                        idx = i;
                }
            }
        }

        if (idx != -1)
        {
            if (!p[idx].started)
            {
                p[idx].response = time - p[idx].arrival;
                p[idx].started = true;
            }

            printf(" P%d |", p[idx].id);

            p[idx].remaining--;
            time++;

            if (p[idx].remaining == 0)
            {
                p[idx].completion = time;
                complete++;
            }
        }
        else
        {
            printf(" idle |");
            time++; // CPU is idle
        }
    }

    printf("\n\nP\tAT\tBT\tPri\tCT\tTAT\tWT\tRT\n");
    for (int i = 0; i < n; ++i)
    {
        int tat = p[i].completion - p[i].arrival;
        int wt = tat - p[i].burst;
        int rt = p[i].response;
        totalTAT += tat;
        totalWT += wt;
        totalRT += rt;

        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].id, p[i].arrival, p[i].burst, p[i].priority,
               p[i].completion, tat, wt, rt);
    }

    printf("\nAverage TAT = %.2f", totalTAT / n);
    printf("\nAverage WT  = %.2f", totalWT / n);
    printf("\nAverage RT  = %.2f\n", totalRT / n);

    return 0;
}
