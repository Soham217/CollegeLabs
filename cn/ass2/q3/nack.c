#include <stdio.h>
#include <time.h>
#include <stdlib.h>


int damaged_lost()
{
    return (rand()%5 == 0);
}

int min(int a, int b)
{
    return (a < b)? a : b;
}

int main()
{
    srand(time(NULL));

    int n_frames, N;
    printf("Enter the number of frames: ");
    scanf("%d", &n_frames);
    printf("Enter the sliding window size: ");
    scanf("%d", &N);

    int i = 1;
    int problem_occurred = 0;

    printf("\n");

    while(i <= n_frames)
    {
        printf("Frames sent: ");

        for(int j = i; j < min(i+N, n_frames+1); j++)
            printf("%d ", j);

        printf("\n");

        problem_occurred = 0;

        for(int j = i; j < min(i+N, n_frames+1); j++)
        {
            if(damaged_lost())
            {
                problem_occurred = j;
                break;
            }
        }

        if(problem_occurred)
        {
            printf("Frame %d was lost or damaged\n\n", problem_occurred);
            i = problem_occurred;
            continue;
        }

        printf("Acknowledgment received for Frames %d to %d\n\n", i, min(i+N-1, n_frames));
        i += N;
    }
}
