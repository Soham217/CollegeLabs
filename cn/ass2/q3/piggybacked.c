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

    int n_frames1, n_frames2, N1, N2;
    printf("Enter the number of frames to be sent by sender 1: ");
    scanf("%d", &n_frames1);
    printf("Enter the sliding window size for sender 1: ");
    scanf("%d", &N1);
    printf("Enter the number of frames in sender 2: ");
    scanf("%d", &n_frames2);
    printf("Enter the sliding window size for sender 2: ");
    scanf("%d", &N2);

    int next_frame1 = 1;
    int next_frame2 = 1;
    int flag = 0;

    int send1_done = 0;
    int send2_done = 0;

    printf("\n");

    while(!(send1_done & send2_done))
    {
        if(!send1_done)
        {
            printf("Frames sent by sender1: ");

            for(int j = next_frame1; j < min(next_frame1+N1, n_frames1+1); j++)
                printf("%da ", j);

            printf("\n");
        }

        flag = 0;

        for(int j = next_frame1; j < min(next_frame1 + N1, n_frames1+1); j++)
        {
            if(damaged_lost())
            {
                flag = j;
                break;
            }
        }

        if(!send1_done)
        {
            if(flag)
            {
                printf("\nSender 2:\nFrame %da was lost or damaged\n", flag);
                next_frame1 = flag;
            }
            else
            {
                printf("\nSender 2:\nAcknowledgment sent for Frames %da to %da\n", next_frame1, min(next_frame1+N1-1, n_frames1));
                next_frame1 += N1;
            }
        }

        if(!send2_done)
        {
            printf("Frames sent by sender2: ");

            for(int j = next_frame2; j < min(next_frame2+N2, n_frames2+1); j++)
                printf("%db ", j);

            printf("\n");
        }

        flag = 0;

        for(int j = next_frame2; j < min(next_frame2 + N2, n_frames2+1); j++)
        {
            if(damaged_lost())
            {
                flag = j;
                break;
            }
        }

        if(!send2_done)
        {
            if(flag)
            {
                printf("\nSender 1:\nFrame %db was lost or damaged\n", flag);
                next_frame2 = flag;
            }
            else
            {
                printf("\nSender 1:\nAcknowledgment sent for Frames %db to %db\n", next_frame2, min(next_frame2+N2-1, n_frames2));
                next_frame2 += N2;
            }
        }


        if(next_frame1 > n_frames1)
            send1_done = 1;

        if(next_frame2 > n_frames2)
            send2_done = 1;

    }
}
