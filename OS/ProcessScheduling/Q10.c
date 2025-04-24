#include <stdio.h>
#include <stdbool.h>

#define MAX 10

typedef struct
{
    int id;
    int arrival;
    int burst;
    int completion;
    int waiting;
    int turnaround;
    int response;
    bool done;
} Process;

void sortByArrival(Process p[], int n)
{
    for (int i = 0; i < n - 1; i++)
        for (int j = i + 1; j < n; j++)
            if (p[i].arrival > p[j].arrival)
            {
                Process temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }
}

int main()
{
    int n;
    Process p[MAX];

    printf("Enter number of processes: ");
    scanf("%d", &n);

    printf("Enter Arrival Time and Burst Time for each process:\n");
    for (int i = 0; i < n; ++i)
    {
        p[i].id = i + 1;
        printf("P%d Arrival Time: ", p[i].id);
        scanf("%d", &p[i].arrival);
        printf("P%d Burst Time: ", p[i].id);
        scanf("%d", &p[i].burst);
        p[i].done = false;
    }

    sortByArrival(p, n);

    int complete = 0, time = 0;
    float totalTAT = 0, totalWT = 0, totalRT = 0;

    while (complete < n)
    {
        int idx = -1;
        float maxRR = -1.0;

        for (int i = 0; i < n; ++i)
        {
            if (!p[i].done && p[i].arrival <= time)
            {
                int wt = time - p[i].arrival;
                float rr = (float)(wt + p[i].burst) / p[i].burst;

                if (rr > maxRR)
                {
                    maxRR = rr;
                    idx = i;
                }
            }
        }

        if (idx != -1)
        {
            time += p[idx].burst;
            p[idx].completion = time;
            p[idx].turnaround = p[idx].completion - p[idx].arrival;
            p[idx].waiting = p[idx].turnaround - p[idx].burst;
            p[idx].response = p[idx].waiting; // No preemption means response time == waiting time
            p[idx].done = true;
            complete++;

            totalTAT += p[idx].turnaround;
            totalWT += p[idx].waiting;
            totalRT += p[idx].response;
        }
        else
        {
            time++; // CPU is idle
        }
    }

    printf("\nP\tAT\tBT\tCT\tTAT\tWT\tRT\n");
    for (int i = 0; i < n; ++i)
    {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].id, p[i].arrival, p[i].burst,
               p[i].completion, p[i].turnaround,
               p[i].waiting, p[i].response);
    }

    printf("\nAverage TAT = %.2f", totalTAT / n);
    printf("\nAverage WT  = %.2f", totalWT / n);
    printf("\nAverage RT  = %.2f\n", totalRT / n);

    return 0;
}
